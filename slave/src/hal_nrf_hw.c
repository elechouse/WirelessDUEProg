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
* $LastChangedRevision: 2513 $
*/

/** @file
* @brief Implementation of #hal_nrf_rw.
*
* @details #hal_nrf_rw is an SPI function which is hardware dependent. This file
* contains an implementation for nRF24LE1.
*/

#include <stdint.h>
#include "hal_nrf.h"
#include "sys_tick.h"

u8 hal_nrf_sta=HAL_NRF_STA_RX;
hal_nrf_rx_sta_t hal_nrf_rx_sta;
hal_nrf_tx_sta_t hal_nrf_tx_sta;
u8 hal_nrf_timeout=0;

u8 hal_nrf_tx_busy;
u8 hal_nrf_tx_buf[HAL_NRF_TX_BUF_LEN];
u8 hal_nrf_tx_buf_tmp[32];
u8 hal_nrf_tx_rd_index, hal_nrf_tx_cnt, hal_nrf_tx_wr_index;

u8 hal_nrf_ack_flag;

u8 hal_nrf_rx_buf[HAL_NRF_RX_BUF_LEN];
u8 hal_nrf_rx_rd_index, hal_nrf_rx_cnt, hal_nrf_rx_wr_index;
u8 hal_nrf_tmp[32];

#ifdef SLAVE_DEBUG
u8 hal_nrf_irq_flag;
#endif

u8 hal_nrf_tx_cmd_flag;
u8 hal_nrf_tick_num;
u8 hal_nrf_cnt;

void spi_init()
{
	SPI_SS_OUTPUT();
	SPI_SCK_OUTPUT();
	SPI_MOSI_OUTPUT();
	SPI_MISO_INPUT();
	
	/** bitrate F_CPU/2 */
	SPCR = (1<<SPE)|(1<<MSTR);//|(1<<SPR0);
	SPSR = (1<<SPI2X);
}

void hal_nrf_init(void)
{
	hal_nrf_sta=HAL_NRF_STA_RX;
	hal_nrf_tx_sta = HAL_NRF_TX_STA_IDLE;
		
	hal_nrf_tx_rd_index=0, hal_nrf_tx_cnt=0, hal_nrf_tx_wr_index=0;
	hal_nrf_tx_busy = 0;
	
	hal_nrf_rx_sta = HAL_NRF_RX_STA_IDLE;
	hal_nrf_rx_rd_index=0, hal_nrf_rx_cnt=0, hal_nrf_rx_wr_index=0;
	
	CSN_OUTPUT();
	CE_OUTPUT();
	CSN_HIGH();
	CE_LOW();
	
	IRQ_INPUT()	;
	IRQ_EN();
	
	t2_init();
	spi_init();
	
	hal_nrf_tick_num = sys_tick_apply();
	sys_tick_set(hal_nrf_tick_num, ON);
	hal_nrf_cnt = 0;
}

uint8_t hal_nrf_rw(uint8_t value)
{
	SPDR = value;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

void read_flash_buf(u8 *buf, u8 __flash *fl, u8 len)
{
	u8 i;
	for(i=0; i<len; i++){
		buf[i] = fl[i];
	}
}

void hal_nrf_send_pkt(u8 *buf, u8 len)
{
	u8 i;
	while( (hal_nrf_tx_cnt+len+1) > HAL_NRF_TX_BUF_LEN);

	/** enter Mutex region */
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
	}
	else{
		switch(hal_nrf_sta){
		  case HAL_NRF_STA_TX:
			/** TX modoule idle, data to USART data register */
			hal_nrf_write_tx_payload(buf, len);
//			CE_HIGH();
//			T2_START();
			t2_start_delay();
			hal_nrf_tx_busy = 1;
			break;
		  case HAL_NRF_STA_RX:
			/* FRIST TX PAYLOAD */
//			hal_nrf_ack_flag = 1;
//			read_flash_buf(slave_cmd, slave_cmd_request, CMD_LENGTH);
//			hal_nrf_write_ack_payload(0, slave_cmd, CMD_LENGTH);
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
		}
	}
	/** exit Mutex region */
	IRQ_EN();
}

u8 hal_nrf_receive_pkt(u8 *buf)
{
	u8 i,len=0;
	//ENTER_MUTEX();
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
	hal_nrf_rx_cnt = hal_nrf_rx_cnt - len -1;
	IRQ_EN();
	//EXIT_MUTEX();
	return len;
}

void hal_nrf_set_sta(hal_nrf_sta_t sta)
{
	u8 len, i;
	if(hal_nrf_tx_cnt){
		hal_nrf_tx_cmd_flag = 0;
		HAL_NRF_WAIT_ACK_DONE();
		IRQ_DIS();
		
		CE_LOW();
		hal_nrf_flush_tx();
		hal_nrf_flush_rx();
		
		hal_nrf_sta = HAL_NRF_STA_TX;
		hal_nrf_tx_busy = 1;
		len = hal_nrf_tx_buf[hal_nrf_tx_rd_index];
		hal_nrf_tx_rd_index++;
		if(hal_nrf_tx_rd_index == HAL_NRF_TX_BUF_LEN){
			hal_nrf_tx_rd_index = 0;
		}
		hal_nrf_tx_cnt = hal_nrf_tx_cnt-len-1;
		
//		hal_nrf_write_tx_payload(hal_nrf_tx_buf_tmp, len);
		CSN_LOW();
		HAL_NRF_HW_SPI_WRITE(W_TX_PAYLOAD);
		while(HAL_NRF_HW_SPI_BUSY) {}
		HAL_NRF_HW_SPI_READ();
		for(i=0; i<len; i++){
			HAL_NRF_HW_SPI_WRITE(hal_nrf_tx_buf[hal_nrf_tx_rd_index]);
			hal_nrf_tx_rd_index++;
			if(hal_nrf_tx_rd_index == HAL_NRF_TX_BUF_LEN){
				hal_nrf_tx_rd_index = 0;
			}
			while(HAL_NRF_HW_SPI_BUSY) {} /* wait for byte transfer finished */
			HAL_NRF_HW_SPI_READ();
		}
		CSN_HIGH();
		hal_nrf_set_operation_mode(HAL_NRF_PTX);
//		CE_HIGH();
//		T2_START();
		t2_start_delay();
		IRQ_EN();
		
	}else{
		read_flash_buf(slave_cmd, slave_cmd_end, CMD_LENGTH);
		hal_nrf_send_cmd(slave_cmd, HAL_NRF_STA_RX);
		hal_nrf_tx_cmd_flag = 2;
	}
}


void hal_nrf_send_cmd(u8 *buf, u8 sta)
{
	/** auto ack delay */
	HAL_NRF_WAIT_ACK_DONE();
	IRQ_DIS();
	CE_LOW();
	hal_nrf_flush_tx();
	hal_nrf_flush_rx();
	hal_nrf_sta = HAL_NRF_STA_TX_CMD;
//	hal_nrf_sta_next = sta;
	hal_nrf_write_tx_payload(buf, CMD_LENGTH);
//	hal_nrf_write_tx_payload_noack(buf, CMD_LENGTH);
	hal_nrf_set_operation_mode(HAL_NRF_PTX);	
//	CE_HIGH();
//	T2_START();	
	t2_start_delay();
	IRQ_EN();
}

void hal_nrf_send_ack(void)
{
	if(hal_nrf_tx_cnt){
		hal_nrf_tx_cmd_flag = 1;
		read_flash_buf(slave_cmd, slave_cmd_request, CMD_LENGTH);
		hal_nrf_send_cmd(slave_cmd, HAL_NRF_STA_RX);
	}
//	else{
//		hal_nrf_tx_cmd_flag = 2;
//		read_flash_buf(slave_cmd, slave_cmd_end, CMD_LENGTH);
//		hal_nrf_send_cmd(slave_cmd, HAL_NRF_STA_RX);
//	}
}

void hal_nrf_clear_tx()
{
	/** discard all data in buffer */
	hal_nrf_tx_busy = 0;
	hal_nrf_tx_cnt = 0;
	hal_nrf_tx_rd_index =0;
	hal_nrf_tx_wr_index = 0;
}

void hal_nrf_sta_handle(void)
{
	u8 len=0;
	if(hal_nrf_rx_cnt){
		/** turn on connect LED */
		hal_nrf_cnt = 0;
		LED_A5_L();
		
#ifdef SLAVE_DEBUG
		len = hal_nrf_receive_pkt(hal_nrf_tmp);
		puthex_buf(hal_nrf_tmp, len);
		putstring("\r\n");
		if(len == CMD_LENGTH){
			putstring("CMD receive\r\n");
			slave_parse_cmd(hal_nrf_tmp);
		}
		
#else	/** SLAVE_DEBUG */

		len = hal_nrf_receive_pkt(hal_nrf_tmp);
		
		if(len == CMD_LENGTH){
//			putbuf(hal_nrf_tmp, len);
			slave_parse_cmd(hal_nrf_tmp);
		}else{
			putbuf(hal_nrf_tmp, len);
		}
#endif	/** SLAVE_DEBUG */
	}
	
	if(hal_nrf_timeout){
		hal_nrf_timeout = 0;
		if(hal_nrf_tx_cmd_flag == 1){
			LED_D4_L();
		}else if(hal_nrf_tx_cmd_flag == 2){
			LED_D5_L();
		}else{
			LED_D6_L();
		}
		hal_nrf_tx_cmd_flag = 0;
//		putstring("TX TIMEOUT\r\n");
	}
	
	switch(hal_nrf_rx_sta){
	  case HAL_NRF_RX_STA_BUF_OVF:
//		putstring("NRF_RX_BUF_OVF\r\n");
		LED_D7_L();
		hal_nrf_rx_sta = HAL_NRF_RX_STA_IDLE;
		break;
	  case HAL_NRF_RX_STA_COM_ERROR:
//		putstring("HAL_NRF_RX_STA_COM_ERROR\r\n");
		LED_D8_L();
		hal_nrf_rx_sta = HAL_NRF_RX_STA_IDLE;
		break;
	}
	
	switch(hal_nrf_sta){
	  case HAL_NRF_STA_RX:
		break;
	  case HAL_NRF_STA_TX:
		break;
	  case HAL_NRF_STA_TX_CMD:
		break;
	}
	
	if(sys_tick_get(hal_nrf_tick_num)>HAL_NRF_TICK){
		sys_tick_clear(hal_nrf_tick_num);
		hal_nrf_cnt++;
		if(hal_nrf_cnt == HAL_NRF_TIMEOUT){
			/** turn off connectted led */
			hal_nrf_cnt = 0;
			LED_A5_H();
		}
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

void hal_nrf_ack_payload(u8 pipe, u8 *buf, u8 length)
{
	u8 i;
	CSN_LOW();
	HAL_NRF_HW_SPI_WRITE(W_ACK_PAYLOAD | pipe);
	while(HAL_NRF_HW_SPI_BUSY) {}
	 HAL_NRF_HW_SPI_READ();
	for(i=0; i<length; i++){
		HAL_NRF_HW_SPI_WRITE(buf[i]);
		while(HAL_NRF_HW_SPI_BUSY){} 
		HAL_NRF_HW_SPI_READ(); 
	}
	CSN_HIGH();
}

void hal_nrf_write_payload(u8 *buf, u8 length)
{
	u8 i;
	CSN_LOW();
  	HAL_NRF_HW_SPI_WRITE(W_TX_PAYLOAD);
	while(HAL_NRF_HW_SPI_BUSY) {}
	 HAL_NRF_HW_SPI_READ();
	for(i=0; i<length; i++){
		HAL_NRF_HW_SPI_WRITE(buf[i]);
		while(HAL_NRF_HW_SPI_BUSY) {} /* wait for byte transfer finished */
		HAL_NRF_HW_SPI_READ();
	}
	CSN_HIGH();
}


#pragma vector = IRQ_VECT
__interrupt void HAL_NRF_ISR(void)
{
	u8 status, len, i;
	if(!IRQ) {
		/**  */
		// Read and clear IRQ flags from radio
		status = hal_nrf_nop();
//		if(hal_nrf_ack_flag){
//			if((status&0x60) == 0x60){
//				hal_nrf_ack_flag = 0;
//			}else{
//				read_flash_buf(slave_cmd, slave_cmd_request, CMD_LENGTH);
//				hal_nrf_write_ack_payload(0, slave_cmd, CMD_LENGTH);
//			}
//		}
#ifdef SLAVE_DEBUG
		hal_nrf_irq_flag = status;
#endif
		if(status & (1<< (uint8_t)HAL_NRF_RX_DR)){

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
					HAL_NRF_HW_SPI_READ();
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
			hal_nrf_write_reg (STATUS,  (1<< (uint8_t)HAL_NRF_TX_DS));
			switch(hal_nrf_sta){
			  case HAL_NRF_STA_TX:
				hal_nrf_tx_cmd_flag = 0;
				if(hal_nrf_tx_cnt){
					hal_nrf_tx_busy = 1;
					len = hal_nrf_tx_buf[hal_nrf_tx_rd_index];
					hal_nrf_tx_rd_index++;
					if(hal_nrf_tx_rd_index == HAL_NRF_TX_BUF_LEN){
						hal_nrf_tx_rd_index = 0;
					}
					hal_nrf_tx_cnt = hal_nrf_tx_cnt-len-1;
//					hal_nrf_write_tx_payload(hal_nrf_tx_buf_tmp, len);
					CSN_LOW();
					HAL_NRF_HW_SPI_WRITE(W_TX_PAYLOAD);
					while(HAL_NRF_HW_SPI_BUSY) {}
					HAL_NRF_HW_SPI_READ();
					for(i=0; i<len; i++){
						HAL_NRF_HW_SPI_WRITE(hal_nrf_tx_buf[hal_nrf_tx_rd_index]);
						hal_nrf_tx_rd_index++;
						if(hal_nrf_tx_rd_index == HAL_NRF_TX_BUF_LEN){
							hal_nrf_tx_rd_index = 0;
						}
						while(HAL_NRF_HW_SPI_BUSY) {} /* wait for byte transfer finished */
						HAL_NRF_HW_SPI_READ();
					}
					CSN_HIGH();
//					CE_HIGH();
//					T2_START();
					t2_start_delay();
				}else{
					if(hal_nrf_tx_sta == HAL_NRF_TX_STA_IDLE){
						/** send end pkt */
						hal_nrf_tx_sta = HAL_NRF_TX_STA_DONE;
						read_flash_buf(slave_cmd, slave_cmd_end, CMD_LENGTH);
						hal_nrf_write_tx_payload(slave_cmd, CMD_LENGTH);
						hal_nrf_tx_cmd_flag = 2;
//						CE_HIGH();
//						T2_START();
						t2_start_delay();
					}else{
						hal_nrf_tx_sta = HAL_NRF_TX_STA_IDLE;
						hal_nrf_sta = HAL_NRF_STA_RX;
						CE_LOW();
						hal_nrf_flush_tx();
						hal_nrf_flush_rx();
						/** return to RX mode */
						hal_nrf_set_operation_mode(HAL_NRF_PRX);
						CE_HIGH();
						hal_nrf_tx_busy = 0;
					}
				}
				break;
			  case HAL_NRF_STA_RX:
				/** ack payload send */
				break;
			  case HAL_NRF_STA_TX_CMD:
				hal_nrf_sta = HAL_NRF_STA_RX;
				
				CE_LOW();
				hal_nrf_flush_tx();
				hal_nrf_flush_rx();
				/** return to RX mode */
				hal_nrf_set_operation_mode(HAL_NRF_PRX);
				CE_HIGH();
				hal_nrf_tx_busy = 0;
				hal_nrf_tx_cmd_flag = 0;
				break;
			}
		}
		
		if(status & (1 << (uint8_t)HAL_NRF_MAX_RT)){
#if 0
			// When a MAX_RT interrupt occurs the TX payload will not be removed from the TX FIFO.
			// If the packet is to be discarded this must be done manually by flushing the TX FIFO.
			// Alternatively, CE_PULSE() can be called re-starting transmission of the payload.
			// (Will only be possible after the radio irq flags are cleared)
			hal_nrf_flush_tx();
			hal_nrf_flush_rx();
			
			hal_nrf_set_operation_mode(HAL_NRF_PRX);
			CE_HIGH();
			hal_nrf_sta=HAL_NRF_STA_RX;
			
			/** discard all data in buffer */
			hal_nrf_tx_busy = 0;
			hal_nrf_tx_cnt = 0;
			hal_nrf_tx_rd_index =0;
			hal_nrf_tx_wr_index = 0;
			
//			hal_nrf_flush_rx();			
//			hal_nrf_rx_cnt = 0;
//			hal_nrf_rx_rd_index =0;
//			hal_nrf_rx_wr_index = 0;
			
			/** set timeout flag */
			hal_nrf_timeout = 1;
			
			hal_nrf_write_reg (STATUS,  (1<< (uint8_t)HAL_NRF_MAX_RT));
#else
            hal_nrf_write_reg (STATUS,  (1<< (uint8_t)HAL_NRF_MAX_RT));
            
            switch(hal_nrf_sta){
			  case HAL_NRF_STA_TX:
				hal_nrf_tx_cmd_flag = 0;
				if(hal_nrf_tx_cnt){
					hal_nrf_tx_busy = 1;
					len = hal_nrf_tx_buf[hal_nrf_tx_rd_index];
					hal_nrf_tx_rd_index++;
					if(hal_nrf_tx_rd_index == HAL_NRF_TX_BUF_LEN){
						hal_nrf_tx_rd_index = 0;
					}
					hal_nrf_tx_cnt = hal_nrf_tx_cnt-len-1;
//					hal_nrf_write_tx_payload(hal_nrf_tx_buf_tmp, len);
					CSN_LOW();
					HAL_NRF_HW_SPI_WRITE(W_TX_PAYLOAD);
					while(HAL_NRF_HW_SPI_BUSY) {}
					HAL_NRF_HW_SPI_READ();
					for(i=0; i<len; i++){
						HAL_NRF_HW_SPI_WRITE(hal_nrf_tx_buf[hal_nrf_tx_rd_index]);
						hal_nrf_tx_rd_index++;
						if(hal_nrf_tx_rd_index == HAL_NRF_TX_BUF_LEN){
							hal_nrf_tx_rd_index = 0;
						}
						while(HAL_NRF_HW_SPI_BUSY) {} /* wait for byte transfer finished */
						HAL_NRF_HW_SPI_READ();
					}
					CSN_HIGH();
//					CE_HIGH();
//					T2_START();
					t2_start_delay();
				}else{
					if(hal_nrf_tx_sta == HAL_NRF_TX_STA_IDLE){
						/** send end pkt */
						hal_nrf_tx_sta = HAL_NRF_TX_STA_DONE;
						read_flash_buf(slave_cmd, slave_cmd_end, CMD_LENGTH);
						hal_nrf_write_tx_payload(slave_cmd, CMD_LENGTH);
						hal_nrf_tx_cmd_flag = 2;
//						CE_HIGH();
//						T2_START();
						t2_start_delay();
					}else{
						hal_nrf_tx_sta = HAL_NRF_TX_STA_IDLE;
						hal_nrf_sta = HAL_NRF_STA_RX;
						CE_LOW();
						hal_nrf_flush_tx();
						hal_nrf_flush_rx();
						/** return to RX mode */
						hal_nrf_set_operation_mode(HAL_NRF_PRX);
						CE_HIGH();
						hal_nrf_tx_busy = 0;
					}
				}
				break;
			  case HAL_NRF_STA_RX:
				/** ack payload send */
				break;
			  case HAL_NRF_STA_TX_CMD:
				hal_nrf_sta = HAL_NRF_STA_RX;
				
				CE_LOW();
				hal_nrf_flush_tx();
				hal_nrf_flush_rx();
				/** return to RX mode */
				hal_nrf_set_operation_mode(HAL_NRF_PRX);
				CE_HIGH();
				hal_nrf_tx_busy = 0;
				hal_nrf_tx_cmd_flag = 0;
				break;
			}
#endif
		}
	}
}