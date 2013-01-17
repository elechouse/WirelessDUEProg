#ifndef __RS485_H
#define __RS485_H


#include "hardware.h"
#include "print.h"
#include "hal_nrf.h"

#define COM_BRAUD					(19200)
#define COM_RX_BUF_LEN 				(64)
#define COM_TX_BUF_LEN 				(32)
#define COM_TIMEOUT					(500/SYS_TICK_UNIT)		// unit us

#define COM_RX_INT_DIS()			UCSR0B &= ~(1<<RXCIE0)
#define COM_TX_INT_DIS()			UCSR0B &= ~(1<<TXCIE0)
#define COM_RX_INT_EN()				UCSR0B |= (1<<RXCIE0)
#define COM_TX_INT_EN()				UCSR0B |= (1<<TXCIE0)

//#define RS485_PORT_OUT     			(DDRD  |= 0x10)
//#define RS485_SEND_EN      			(PORTD |= 0x10)
//#define RS485_SEND_DIS     			(PORTD &=~ 0x10)
//#define COM_RS485_CP				(0x10)
//#define COM_RS485_CP_OUTPUT()		DDRD |= COM_RS485_CP
//#define COM_RS485_TX_DIS_RX_EN()	PORTD &= ~COM_RS485_CP
//#define COM_RS485_RX_DIS_TX_EN()	PORTD |= COM_RS485_CP


#define RXB8 						1
#define TXB8 						0
#define UPE 						2
#define DOR 						3
#define FE 							4
#define UDRE 						5
#define RXC 						7

#define FRAMING_ERROR 				(1<<FE)
#define PARITY_ERROR 				(1<<UPE)
#define DATA_OVERRUN 				(1<<DOR)

#define DATA_REGISTER_EMPTY 		(1<<UDRE)
#define RX_COMPLETE 				(1<<RXC)

typedef enum{
	COM_RX_STA_IDLE,
	COM_RX_STA_ABOUT_FULL,
	COM_RX_STA_FULL,
	COM_RX_STA_TIMEOUT,
	COM_RX_STA_COM_ERROR,
	COM_RX_STA_BUF_OVF,
//	COM_RX_STA_RVS_ERROR,
//	COM_RX_STA_PACKET_OVF,
//	COM_RX_STA_TIMEOUT
}com_rx_sta_type;


void com_init(u32 br);
void com_reconfig(u32 baud);
void com_putchar(u8 c);
void com_rvs_putchar(u8 c);
void com_send_pkt(u8 *buf,u8 len);
void com_sta_handle(void);
u8 com_available(void);
u8 com_getchar(void);
void com_set_timeout(u8 to);

extern __root u8 com_rx_buf[COM_RX_BUF_LEN];

#endif

/** 
	0x55 -> 0x56 0x56
	0x56 -> 0x56 0x57
	CRC cover all data area except frame head and CRC itself
*/