/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic 
 * Semiconductor ASA.Terms and conditions of usage are described in detail 
 * in NORDIC SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT. 
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRENTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *              
 * $LastChangedRevision: 133 $
 */

/** @file
 * @brief Implementation of #hal_nrf_rw for nRF24LU1+
 *
 * #hal_nrf_rw is an SPI function which is hardware dependent. This file
 * contains an implementation for nRF24LU1.
 */

#include "stdint.h"
#include "hal_nrf.h"
#include "reg24lu1.h"

u8 hal_nrf_sta=HAL_NRF_STA_RX_IDLE;
u8 hal_nrf_sta_next=HAL_NRF_STA_RX_IDLE;
hal_nrf_rx_sta_t hal_nrf_rx_sta;
hal_nrf_tx_sta_t hal_nrf_tx_sta;

u8 hal_nrf_tx_busy;
u8 hal_nrf_tx_buf[HAL_NRF_TX_BUF_LEN];
u8 data hal_nrf_tx_rd_index, hal_nrf_tx_cnt, hal_nrf_tx_wr_index;

u8 hal_nrf_rx_buf[HAL_NRF_RX_BUF_LEN];
u8 data hal_nrf_rx_rd_index, hal_nrf_rx_cnt, hal_nrf_rx_wr_index;
u8 hal_nrf_tmp[32];

#ifdef MASTER_DEBUG
u8 hal_nrf_irq_flag;
#endif

u16 hal_nrf_rx_busy_cnt=0;
u8 hal_nrf_tx_cmd_flag=0;

#define MAX(a, b) ((a) > (b) ? (a) : (b))

u8 hal_nrf_connect=0;
u8 hal_nrf_connect_previous=0;

void spi_init()
{
	// Enable radio SPI
	RFCTL = 0x10;
}

void hal_nrf_init(void)
{
	hal_nrf_rx_sta = HAL_NRF_RX_STA_IDLE;
	hal_nrf_tx_sta = HAL_NRF_TX_STA_IDLE;
	
	hal_nrf_tx_rd_index=0, hal_nrf_tx_cnt=0, hal_nrf_tx_wr_index=0;
	hal_nrf_tx_busy = 0;
	
	hal_nrf_rx_rd_index=0, hal_nrf_rx_cnt=0, hal_nrf_rx_wr_index=0;
	
	// Enable the radio clock
	RFCKEN = 1;
	// Enable RF interrupt
	RF = 1;
	spi_init();
	t2_init();
}

uint8_t hal_nrf_rw(uint8_t value)
{
  RFDAT = value;
  RFSPIF = 0;     // ! IMPORTANT ! Clear RF SPI ready flag
                  // after data written to RFDAT..
  while(!RFSPIF) {}; // wait for byte transfer finished
  return RFDAT;   // return SPI read value
}

void hal_nrf_send_pkt(u8 *buf, u8 len)
{
	u8 i;
	while( (hal_nrf_tx_cnt+len+1) > HAL_NRF_TX_BUF_LEN){
		putchar('*');
	};
	IRQ_DIS();
	if(hal_nrf_tx_cnt || hal_nrf_tx_busy){
		hal_nrf_tx_buf[hal_nrf_tx_wr_index] = len;
		hal_nrf_tx_wr_index++;
		if(hal_nrf_tx_wr_index == HAL_NRF_TX_BUF_LEN){
			hal_nrf_tx_wr_index = 0;
		}
		hal_nrf_tx_cnt++;
		
		for(i=0; i<len; i++){
			hal_nrf_tx_buf[hal_nrf_tx_wr_index] = buf[i];
			hal_nrf_tx_wr_index++;
			if(hal_nrf_tx_wr_index == HAL_NRF_TX_BUF_LEN){
				hal_nrf_tx_wr_index = 0;
			}
			hal_nrf_tx_cnt++;
		}
	}else{
		switch(hal_nrf_sta){
			case HAL_NRF_STA_RX_IDLE:
			case HAL_NRF_STA_RX_BUSY:
			case HAL_NRF_STA_TX_CMD:
				hal_nrf_tx_buf[hal_nrf_tx_wr_index] = len;
				hal_nrf_tx_wr_index++;
				if(hal_nrf_tx_wr_index == HAL_NRF_TX_BUF_LEN){
					hal_nrf_tx_wr_index = 0;
				}
				hal_nrf_tx_cnt++;
				for(i=0; i<len; i++){
					hal_nrf_tx_buf[hal_nrf_tx_wr_index] = buf[i];
					hal_nrf_tx_wr_index++;
					if(hal_nrf_tx_wr_index == HAL_NRF_TX_BUF_LEN){
						hal_nrf_tx_wr_index = 0;
					}
					hal_nrf_tx_cnt++;
				}
				break;
			case HAL_NRF_STA_TX:
				putstring("Write TX Directly\r\n");
				/** TX modoule idle, data to USART data register */
				hal_nrf_write_tx_payload(buf, len);
// 				CE_HIGH();
// 				T2_START();
				t2_start_delay();
				hal_nrf_tx_busy = 1;
				break;
		}
	}
	IRQ_EN();
}

u8 hal_nrf_receive_pkt(u8 *buf)
{
	u8 i,len=0;
	IRQ_DIS();
	len = hal_nrf_rx_buf[hal_nrf_rx_rd_index];
	hal_nrf_rx_rd_index++;
	if(hal_nrf_rx_rd_index == HAL_NRF_RX_BUF_LEN){
		hal_nrf_rx_rd_index = 0;
	}
	for(i=0; i<len; i++){
		buf[i] = hal_nrf_rx_buf[hal_nrf_rx_rd_index];
		hal_nrf_rx_rd_index++;
		if(hal_nrf_rx_rd_index == HAL_NRF_RX_BUF_LEN){
			hal_nrf_rx_rd_index = 0;
		}
	}
	hal_nrf_rx_cnt = (hal_nrf_rx_cnt - len -1);
	IRQ_EN();
	return len;
}

void hal_nrf_set_sta(u8 sta)
{
	u8 len, i, dummy;
	switch(sta){
		case HAL_NRF_STA_TX:
			// RX->TX delay
			HAL_NRF_WAIT_ACK_DONE();
			hal_nrf_sta = HAL_NRF_STA_TX;
			IRQ_DIS();
			CE_LOW();
			if(hal_nrf_tx_cnt){
				hal_nrf_tx_busy = 1;
				len = hal_nrf_tx_buf[hal_nrf_tx_rd_index];
				hal_nrf_tx_rd_index++;
				if(hal_nrf_tx_rd_index == HAL_NRF_TX_BUF_LEN){
					hal_nrf_tx_rd_index = 0;
				}
				hal_nrf_tx_cnt = hal_nrf_tx_cnt-len-1;
				CSN_LOW();
				HAL_NRF_HW_SPI_WRITE(W_TX_PAYLOAD);
				while(HAL_NRF_HW_SPI_BUSY) {}
				dummy = HAL_NRF_HW_SPI_READ();
				for(i=0; i<len; i++){
					HAL_NRF_HW_SPI_WRITE(hal_nrf_tx_buf[hal_nrf_tx_rd_index]);
					hal_nrf_tx_rd_index++;
					if(hal_nrf_tx_rd_index == HAL_NRF_TX_BUF_LEN){
						hal_nrf_tx_rd_index = 0;
					}
					while(HAL_NRF_HW_SPI_BUSY) {} /* wait for byte transfer finished */
					dummy = HAL_NRF_HW_SPI_READ();
				}
				CSN_HIGH();
				hal_nrf_set_operation_mode(HAL_NRF_PTX);
// 				CE_HIGH();
// 				T2_START();
				t2_start_delay();
			}
			IRQ_EN();
			break;
		case HAL_NRF_STA_RX_IDLE:
// 			CE_LOW();
// 			/** return to RX mode */
// 			hal_nrf_set_operation_mode(HAL_NRF_PRX);
// 			CE_HIGH();
			hal_nrf_sta = HAL_NRF_STA_RX_IDLE;
			break;
		case HAL_NRF_STA_RX_BUSY:
// 			if(hal_nrf_sta == HAL_NRF_STA_TX){
// 				CE_LOW();
// 				/** return to RX mode */
// 				hal_nrf_set_operation_mode(HAL_NRF_PRX);
// 				CE_HIGH();
// 			}
			hal_nrf_sta = HAL_NRF_STA_RX_BUSY;
			break;
		default:
			/** error state */
			break;
	}
}

void hal_nrf_send_cmd(u8 *buf, u8 sta)
{
	while(hal_nrf_sta == HAL_NRF_STA_TX_CMD);
//	if(hal_nrf_sta != HAL_NRF_STA_RX_BUSY){
	/** auto ack delay */
		switch(buf[0]){
			case MASTER_CMD_TX_ALLOW:
				hal_nrf_tx_cmd_flag = 2;
				break;
			case MASTER_CMD_ACK:
				hal_nrf_tx_cmd_flag = 1;
				break;
			case MASTER_CMD_SET_CONTROL_LINE_STATE:
				hal_nrf_tx_cmd_flag = 4;
				break;
			case MASTER_CMD_SET_LINE_CODING:
				hal_nrf_tx_cmd_flag = 3;
				break;
		}
		HAL_NRF_WAIT_ACK_DONE();
		IRQ_DIS();
		CE_LOW();
			
		hal_nrf_sta = HAL_NRF_STA_TX_CMD;
		hal_nrf_sta_next = sta;
		if(buf[0] != MASTER_CMD_TX_ALLOW){
			hal_nrf_write_tx_payload(buf, CMD_LENGTH);
		}else{
			hal_nrf_write_tx_payload_noack(buf, CMD_LENGTH);
//			hal_nrf_write_tx_payload(buf, CMD_LENGTH);
		}
		hal_nrf_set_operation_mode(HAL_NRF_PTX);
// 		CE_HIGH();
// 		T2_START();	
		t2_start_delay();
		IRQ_EN();
// 	}else{
// 		hal_nrf_tx_cmd_flag = 1;
// 	}
	
}

void hal_nrf_sta_handle(void)
{
	u8 len;
	if(hal_nrf_rx_cnt){
		hal_nrf_rx_busy_cnt=0;
#ifdef MASTER_DEBUG
//		hal_nrf_count++;
		len = hal_nrf_receive_pkt(hal_nrf_tmp);
		puthex_buf(hal_nrf_tmp, len);
		if(len == CMD_LENGTH){
			master_parse_cmd(hal_nrf_tmp);
			usb_ep2_write_buf(hal_nrf_tmp, len);
		}else{
			usb_ep2_write_buf(hal_nrf_tmp, len);
		}
#else	/** NO MASTER_DEBUG */
		len = hal_nrf_receive_pkt(hal_nrf_tmp);
		if(len == CMD_LENGTH){
			master_parse_cmd(hal_nrf_tmp);
//			putstring("CMD RECEIVE\r\n");
//			usb_ep2_write_buf(hal_nrf_tmp, len);
		}else{
// 			puthex(len);
// 			putstring("\r\n");
			usb_ep2_write_buf(hal_nrf_tmp, len);
		}
#endif	/** MASTER_DEBUG */
	}

	switch(hal_nrf_rx_sta){
	  case HAL_NRF_RX_STA_BUF_OVF:
		putstring("HAL_NRF_RX_BUF_OVF\r\n");
		hal_nrf_rx_sta = HAL_NRF_RX_STA_IDLE;
		break;
	  case HAL_NRF_RX_STA_COM_ERROR:
		putstring("HAL_NRF_RX_COM_ERROR\r\n");
		hal_nrf_rx_sta = HAL_NRF_RX_STA_IDLE;
		break;
	}
		
	switch(hal_nrf_tx_sta){
	  case HAL_NRF_TX_STA_BUF_OVF:
		putstring("HAL_NRF_TX_BUF_OVF\r\n");
		hal_nrf_tx_sta = HAL_NRF_TX_STA_IDLE;
		break;
	  case HAL_NRF_TX_STA_TIMEOUT:
		puthex(hal_nrf_tx_cmd_flag);
		switch(hal_nrf_tx_cmd_flag){
			case 1:
				putstring("ACK ");
				break;
			case 2:
				putstring("TX ALLOW ");
				break;
			case 3:
				putstring("SetLineCoding ");
				break;
			case 4:
				putstring("ControlLineState ");
				break;
		}
		hal_nrf_tx_cmd_flag = 0;
		putstring("TX_STA_TIMEOUT\r\n");
		hal_nrf_tx_sta = HAL_NRF_TX_STA_IDLE;
		hal_nrf_connect_previous = hal_nrf_connect;
		hal_nrf_connect = 0;
		break;
	}
	
	if(hal_nrf_connect && !hal_nrf_connect_previous){
		hal_nrf_connect_previous = hal_nrf_connect;
		putstring("Connected\r\n");
		P05=0;		// LED_ON
		P03 = 1; 	// LED_TEST_ON();
	}else if(!hal_nrf_connect && hal_nrf_connect_previous){
		hal_nrf_connect_previous = hal_nrf_connect;
		putstring("Disconnected\r\n");
		P05=1;		// LED_OFF
		P03=0;		// LED_TEST_OFF();
	}
	
	switch(hal_nrf_sta){
		case HAL_NRF_STA_RX_IDLE:
//			if(!hal_nrf_tx_cmd_flag){
				if(hal_nrf_tx_cnt){
//					hal_nrf_tx_cmd_flag = 0;
					hal_nrf_set_sta(HAL_NRF_STA_TX);
				}else{
					hal_nrf_rx_busy_cnt = 0;
					hal_nrf_send_cmd(master_cmd_ack, HAL_NRF_STA_RX_BUSY);	
					hal_nrf_tx_cmd_flag = 1;	
//					hal_nrf_send_cmd(master_cmd_tx_allow, HAL_NRF_STA_RX_BUSY);
				}
// 			}else{
// 				hal_nrf_send_cmd(master_cmd, HAL_NRF_STA_RX_BUSY);
// 			}
			break;
		case HAL_NRF_STA_RX_BUSY:
//			putstring("BUSY error\r\n");
			hal_nrf_rx_busy_cnt++;
			if(hal_nrf_rx_busy_cnt>1000){
				hal_nrf_rx_busy_cnt=0;
//				putstring("BUSY error\r\n");
				hal_nrf_sta = HAL_NRF_STA_RX_IDLE;
			}
			break;
		case HAL_NRF_STA_TX:
//			putstring("TX\r\n");
			break;
		case HAL_NRF_STA_TX_CMD:
//			putstring("TX_CMD\r\n");
			break;
		default:
			break;
	}
}

void hal_nrf_read_payload(u8 *buf, u8 length)
{
	u8 i;
	CSN_LOW();
	hal_nrf_rw(R_RX_PAYLOAD);
	for(i=0; i<length; i++){
		HAL_NRF_HW_SPI_WRITE(0U);
		while(HAL_NRF_HW_SPI_BUSY){} 
      	buf[i] =  HAL_NRF_HW_SPI_READ(); 
	}
	
	CSN_HIGH();
}

u8 nrf_rx_payload[32];

u8 nrf_rx_len;
// Radio interrupt
NRF_ISR()
{
	u8 status,len,i,dummy;
	// Read and clear IRQ flags from radio
	status = hal_nrf_nop();
#ifdef MASTER_DEBUG
	hal_nrf_irq_flag = status;
#endif	
	if(status & (1<< (uint8_t)HAL_NRF_RX_DR)){
		hal_nrf_rx_busy_cnt = 0;
		do{
			len  = hal_nrf_read_rx_payload_width();
			if(len > 32){
				hal_nrf_write_reg (STATUS,  (1<< (uint8_t)HAL_NRF_RX_DR));
				hal_nrf_rx_sta = HAL_NRF_RX_STA_COM_ERROR;
				return;
			}
			if((hal_nrf_rx_cnt + len + 1) < HAL_NRF_RX_BUF_LEN){
				hal_nrf_rx_buf[hal_nrf_rx_wr_index] = len;
				hal_nrf_rx_wr_index++;
				if(hal_nrf_rx_wr_index == HAL_NRF_RX_BUF_LEN){
					hal_nrf_rx_wr_index=0;
				}
//					hal_nrf_read_payload((u8*)hal_nrf_rx_buf[hal_nrf_rx_wr_index].buf, len);
				CSN_LOW();
				HAL_NRF_HW_SPI_WRITE(R_RX_PAYLOAD);
				while(HAL_NRF_HW_SPI_BUSY) {}
				dummy=HAL_NRF_HW_SPI_READ();
				for(i=0; i<len; i++){
					HAL_NRF_HW_SPI_WRITE(0U);
					while(HAL_NRF_HW_SPI_BUSY){} 
					hal_nrf_rx_buf[hal_nrf_rx_wr_index] =  HAL_NRF_HW_SPI_READ(); 
					hal_nrf_rx_wr_index++;
					if(hal_nrf_rx_wr_index == HAL_NRF_RX_BUF_LEN){
						hal_nrf_rx_wr_index=0;
					}
				}
				CSN_HIGH();
				hal_nrf_rx_cnt = hal_nrf_rx_cnt+len+1;					
			}else{
				hal_nrf_flush_rx();
				hal_nrf_rx_sta = HAL_NRF_RX_STA_BUF_OVF;
				/** clear RX_DR */
				hal_nrf_write_reg (STATUS,  (1<< (uint8_t)HAL_NRF_RX_DR));
				break;
			}
			/** clear RX_DR */
			hal_nrf_write_reg (STATUS,  (1<< (uint8_t)HAL_NRF_RX_DR));
		}while(!hal_nrf_rx_fifo_empty());
	}
	
	
	if(status & (1 << (uint8_t)HAL_NRF_TX_DS)){
		hal_nrf_connect_previous = hal_nrf_connect;
		hal_nrf_connect = 1;
		switch(hal_nrf_sta){
			case HAL_NRF_STA_TX:
				if(hal_nrf_tx_cnt){
					hal_nrf_tx_busy = 1;
					len = hal_nrf_tx_buf[hal_nrf_tx_rd_index];
					hal_nrf_tx_rd_index++;
					if(hal_nrf_tx_rd_index == HAL_NRF_TX_BUF_LEN){
						hal_nrf_tx_rd_index = 0;
					}
					hal_nrf_tx_cnt = hal_nrf_tx_cnt-len-1;
					CSN_LOW();
					HAL_NRF_HW_SPI_WRITE(W_TX_PAYLOAD);
					while(HAL_NRF_HW_SPI_BUSY) {}
					dummy = HAL_NRF_HW_SPI_READ();
					for(i=0; i<len; i++){
						HAL_NRF_HW_SPI_WRITE(hal_nrf_tx_buf[hal_nrf_tx_rd_index]);
						hal_nrf_tx_rd_index++;
						if(hal_nrf_tx_rd_index == HAL_NRF_TX_BUF_LEN){
							hal_nrf_tx_rd_index = 0;
						}
						while(HAL_NRF_HW_SPI_BUSY) {} /* wait for byte transfer finished */
						dummy = HAL_NRF_HW_SPI_READ();
					}
					CSN_HIGH();
// 					CE_HIGH();
// 					T2_START();
					t2_start_delay();
				}else{
					CE_LOW();
					/** return to RX mode */
					hal_nrf_set_operation_mode(HAL_NRF_PRX);
					CE_HIGH();
					hal_nrf_tx_busy = 0;
					hal_nrf_sta = HAL_NRF_STA_RX_IDLE;
				}
				break;
			case HAL_NRF_STA_TX_CMD:
				hal_nrf_sta = hal_nrf_sta_next;
				CE_LOW();
				/** return to RX mode */
				hal_nrf_set_operation_mode(HAL_NRF_PRX);
				CE_HIGH();
				hal_nrf_tx_busy = 0;
				break;
			case HAL_NRF_STA_RX_IDLE:
				hal_nrf_sta = HAL_NRF_STA_RX_IDLE;
				CE_LOW();
				/** return to RX mode */
				hal_nrf_set_operation_mode(HAL_NRF_PRX);
				CE_HIGH();
				hal_nrf_tx_busy = 0;
				break;
			case HAL_NRF_STA_RX_BUSY:
				/** never get here */
				hal_nrf_sta = HAL_NRF_STA_RX_IDLE;
				CE_LOW();
				/** return to RX mode */
				hal_nrf_set_operation_mode(HAL_NRF_PRX);
				CE_HIGH();
				hal_nrf_tx_busy = 0;
				break;
		}
		hal_nrf_write_reg (STATUS,  (1<< (uint8_t)HAL_NRF_TX_DS));
	}
	
	
	if(status & (1 << (uint8_t)HAL_NRF_MAX_RT)){
#if 1
		// When a MAX_RT interrupt occurs the TX payload will not be removed from the TX FIFO.
		// If the packet is to be discarded this must be done manually by flushing the TX FIFO.
		// Alternatively, CE_PULSE() can be called re-starting transmission of the payload.
		// (Will only be possible after the radio irq flags are cleared)
		hal_nrf_flush_tx();
		hal_nrf_tx_busy = 0;
		hal_nrf_set_operation_mode(HAL_NRF_PRX);
		CE_HIGH();
		
		/** discard all data in buffer */
		hal_nrf_tx_cnt = 0;
		hal_nrf_tx_rd_index =0;
		hal_nrf_tx_wr_index = 0;
		
		hal_nrf_tx_sta = HAL_NRF_TX_STA_TIMEOUT;
		hal_nrf_sta = HAL_NRF_STA_RX_IDLE;
		hal_nrf_write_reg (STATUS,  (1<< (uint8_t)HAL_NRF_MAX_RT));
#else
		switch(hal_nrf_sta){
			case HAL_NRF_STA_TX:
				if(hal_nrf_tx_cnt){
					hal_nrf_tx_busy = 1;
					len = hal_nrf_tx_buf[hal_nrf_tx_rd_index];
					hal_nrf_tx_rd_index++;
					if(hal_nrf_tx_rd_index == HAL_NRF_TX_BUF_LEN){
						hal_nrf_tx_rd_index = 0;
					}
					hal_nrf_tx_cnt = hal_nrf_tx_cnt-len-1;
					CSN_LOW();
					HAL_NRF_HW_SPI_WRITE(W_TX_PAYLOAD);
					while(HAL_NRF_HW_SPI_BUSY) {}
					dummy = HAL_NRF_HW_SPI_READ();
					for(i=0; i<len; i++){
						HAL_NRF_HW_SPI_WRITE(hal_nrf_tx_buf[hal_nrf_tx_rd_index]);
						hal_nrf_tx_rd_index++;
						if(hal_nrf_tx_rd_index == HAL_NRF_TX_BUF_LEN){
							hal_nrf_tx_rd_index = 0;
						}
						while(HAL_NRF_HW_SPI_BUSY) {} /* wait for byte transfer finished */
						dummy = HAL_NRF_HW_SPI_READ();
					}
					CSN_HIGH();
// 					CE_HIGH();
// 					T2_START();
					t2_start_delay();
				}else{
					CE_LOW();
					/** return to RX mode */
					hal_nrf_set_operation_mode(HAL_NRF_PRX);
					CE_HIGH();
					hal_nrf_tx_busy = 0;
					hal_nrf_sta = HAL_NRF_STA_RX_IDLE;
				}
				break;
			case HAL_NRF_STA_TX_CMD:
				hal_nrf_sta = hal_nrf_sta_next;
				CE_LOW();
				/** return to RX mode */
				hal_nrf_set_operation_mode(HAL_NRF_PRX);
				CE_HIGH();
				hal_nrf_tx_busy = 0;
				break;
			case HAL_NRF_STA_RX_IDLE:
				hal_nrf_sta = HAL_NRF_STA_RX_IDLE;
				CE_LOW();
				/** return to RX mode */
				hal_nrf_set_operation_mode(HAL_NRF_PRX);
				CE_HIGH();
				hal_nrf_tx_busy = 0;
				break;
			case HAL_NRF_STA_RX_BUSY:
				/** never get here */
				hal_nrf_sta = HAL_NRF_STA_RX_IDLE;
				CE_LOW();
				/** return to RX mode */
				hal_nrf_set_operation_mode(HAL_NRF_PRX);
				CE_HIGH();
				hal_nrf_tx_busy = 0;
				break;
		}
#endif
	}
}

