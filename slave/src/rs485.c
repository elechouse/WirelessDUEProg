#include "rs485.h"
#include "sys_tick.h"

__root u8 com_rx_buf[COM_RX_BUF_LEN]={0};
__root u8 com_tx_buf[COM_TX_BUF_LEN]={0};

u8 com_rx_rd_index, com_rx_cnt, com_rx_wr_index;
//u8 com_rx_wr_index;
u8 com_tx_rd_index, com_tx_cnt, com_tx_wr_index;

com_rx_sta_type 	com_rx_sta = COM_RX_STA_IDLE;

u8 com_tick_num;
u8 com_timeout;


#ifdef COM_DEBUG
u8 debug_status;
#endif

/**
USCR0A	USCR0B	USCR0C	UBRR0	UDR0
*/
void com_init(u32 baud)
{	
	u16 baud_setting;
	u8 use_u2x = 1;
	
	UCSR0B = 0;
	UCSR0A = 0;
	UCSR0C = 0;
	
	com_rx_rd_index=0, com_rx_cnt=0, com_rx_wr_index=0;
	com_tx_rd_index=0, com_tx_cnt=0, com_tx_wr_index=0;	
	com_rx_sta = COM_RX_STA_IDLE;
	
  try_again:
	if (use_u2x) {
		UCSR0A = 1 << U2X0;
		baud_setting = (F_CPU / 4 / baud - 1) / 2;
	} else {
		UCSR0A = 0;
		baud_setting = (F_CPU / 8 / baud - 1) / 2;
	}
	
	if ((baud_setting > 4095) && use_u2x)
	{
		use_u2x = 0;
		goto try_again;
	}
	UBRR0  = baud_setting;
	UCSR0B = 0xD8;
	UCSR0C = 0x06;
		
	if(baud<115200){
		com_timeout = 115200/baud;
	}else{
		com_timeout = 1;
	}

	/** get system tick */
	com_tick_num = sys_tick_apply();
}

void com_reconfig(u32 baud)
{
	u16 baud_setting;
	u8 use_u2x = 1;
	
	UCSR0B = 0;
	UCSR0A = 0;
	UCSR0C = 0;
	
	com_rx_rd_index=0, com_rx_cnt=0, com_rx_wr_index=0;
	com_tx_rd_index=0, com_tx_cnt=0, com_tx_wr_index=0;	
	com_rx_sta = COM_RX_STA_IDLE;
	
try_again:
	if (use_u2x) {
		UCSR0A = 1 << U2X0;
		baud_setting = (F_CPU / 4 / baud - 1) / 2;
	} else {
		UCSR0A = 0;
		baud_setting = (F_CPU / 8 / baud - 1) / 2;
	}
	
	if ((baud_setting > 4095) && use_u2x)
	{
		use_u2x = 0;
		goto try_again;
	}
	UBRR0  = baud_setting;
	UCSR0B = 0xD8;
	UCSR0C = 0x06;
	
	if(baud<115200){
		com_timeout = 115200/baud;
	}else{
		com_timeout = 1;
	}
	
	sys_tick_set(com_tick_num, OFF);
}

void com_putchar(u8 c)
{
	/** wait until TX buffer is not full */
	while(com_tx_cnt == COM_TX_BUF_LEN);
	//asm("cli");
//	ENTER_MUTEX();
	COM_TX_INT_DIS();
	/** TX buffer is not empty or TX modoule is not busy */
	if(com_tx_cnt || ((UCSR0A & DATA_REGISTER_EMPTY)==0)){
		/** data to buffer */
		com_tx_buf[com_tx_wr_index++]=c;
		/** TX write pointer point to head */
		if (com_tx_wr_index == COM_TX_BUF_LEN){
			com_tx_wr_index=0;
		}
		/** remain data in TX buffer plus one */
		++com_tx_cnt;
	}
	else{
		/** TX modoule idle, data to USART data register */
		UDR0=c;
	}
	COM_TX_INT_EN();
	//	asm("sei");
//	EXIT_MUTEX();
}
u8 com_available(void)
{
	return com_rx_cnt;
}

u8 com_getchar()
{
	u8 ch;
	if(com_rx_cnt == 0){
		/** ERROR */
		return 0xFF;
	}
//	ENTER_MUTEX();
	COM_RX_INT_DIS();
	ch = com_rx_buf[com_rx_rd_index++];
	if(com_rx_rd_index == COM_RX_BUF_LEN){
		com_rx_rd_index = 0;
	}
	com_rx_cnt--;
//	EXIT_MUTEX();
	COM_RX_INT_EN();
	return ch;
}

/** dangerous function */
u8 com_getbuf(u8 *buf, u8 len)
{
//	u8 i;
//	for(i=0; i<len; i++){
//		if(com_rx_cnt){
//			buf[i] = com_getchar();
//		}else{
//			return(i);
//		}
//	}
//	return len;
	u8 i;
	if(com_rx_cnt == 0){
		/** ERROR */
		return 0;
	}
	if(com_rx_cnt < len){
		len = com_rx_cnt;
	}
//	ENTER_MUTEX();
	COM_RX_INT_DIS();
	for(i=0; i<len; i++){
		buf[i] = com_rx_buf[com_rx_rd_index++];
		if(com_rx_rd_index == COM_RX_BUF_LEN){
			com_rx_rd_index = 0;
		}
	}
	com_rx_cnt-=len;
	COM_RX_INT_EN();
//	EXIT_MUTEX();
	return len;
}

void com_flush_rx()
{
	
}

void com_send_pkt(u8 *buf,u8 len)
{
	while(len--){
		com_putchar(*buf++);
	}
}
u8 test_buf[32];

void com_set_timeout(u8 to)
{
	com_timeout = to;
}

void com_sta_handle(void)
{
	u8 len;
	if(com_rx_cnt>(31)){
//		putstring("UART About Full.\r\n");
		len = com_getbuf(test_buf,32);
		if(len == CMD_LENGTH){
//			puthex(len);
//			putstring("\r\n");
			hal_nrf_send_pkt(test_buf, CMD_LENGTH/2);
			hal_nrf_send_pkt(test_buf+CMD_LENGTH/2, CMD_LENGTH-CMD_LENGTH/2);
		}else{
//			puthex(len);
//			putstring("\r\n");
			hal_nrf_send_pkt(test_buf, len);
		}
	}
	if(sys_tick_get(com_tick_num)>com_timeout){
//	if(sys_tick_get(com_tick_num)>COM_TIMEOUT){
		sys_tick_set(com_tick_num, OFF);
//		putstring("UART Timeout.\r\n");
		len = com_getbuf(test_buf, 32);
		if(len == CMD_LENGTH){
//			puthex(len);
//			putstring("\r\n");
			hal_nrf_send_pkt(test_buf, CMD_LENGTH/2);
			hal_nrf_send_pkt(test_buf+CMD_LENGTH/2, CMD_LENGTH-CMD_LENGTH/2);
		}else if(len){
//			puthex(len);
//			putstring("\r\n");
			hal_nrf_send_pkt(test_buf, len);
		}
	}
	switch(com_rx_sta){
		case COM_RX_STA_COM_ERROR:
//		putstring("UART Error.\r\n");
		LED_A0_L();
		break;
	  case COM_RX_STA_BUF_OVF:
//		putstring("UART BUFOVF.\r\n");
		LED_A1_L();
		break;
	  default:
		break;
	}
	com_rx_sta = COM_RX_STA_IDLE;
}

/**
@brief serial port recieve interrupt, recieve state machine.
*/
#pragma vector=USART_RX_vect
__interrupt void com_rx_irq(void)
{
	u8 status,data;
	//	u16 len_tmp;
	/** get recieve status, 
	read recieve buffer(this can also clear interrupt) */
	status = UCSR0A;
	data = UDR0;
	
	/** check recieve status 0x1C*/
	if((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0){
		if(com_rx_cnt<COM_RX_BUF_LEN){
			sys_tick_set(com_tick_num, ON);
			sys_tick_clear(com_tick_num);
			com_rx_buf[com_rx_wr_index++] = data;
			if(com_rx_wr_index == COM_RX_BUF_LEN){
				com_rx_wr_index = 0;
			}
			com_rx_cnt++;
		}else{
			/** data overflow */
			com_rx_sta = COM_RX_STA_BUF_OVF;
			LED_A0_L();
		}
	}else{
		/** hardware fault */
		com_rx_sta=COM_RX_STA_COM_ERROR;
		LED_A1_L();
	}
}

/**
@brief serial port transmit completely interrupt
*/
#pragma vector=USART_TX_vect
__interrupt void com_tx_irq(void)
{
	if(com_tx_cnt)
	{
		/** TX buffer is not empty, transmit next byte */
		--com_tx_cnt;
		UDR0=com_tx_buf[com_tx_rd_index++];
		if (com_tx_rd_index == COM_TX_BUF_LEN){
			com_tx_rd_index=0;
		}
	}else{
		/** clear interrupt */
		// avr hardware auto clear interrupt  
	}
}

