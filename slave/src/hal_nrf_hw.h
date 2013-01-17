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
 * @brief Macros and hardware includes for nRF24LE1
 * @ingroup hal_nrf24le1
 *
 * @{
 * @name Hardware dependencies
 * @{
 *
 */

#ifndef HAL_NRF_LE1_H__
#define HAL_NRF_LE1_H__

#include "hardware.h"
#include "print.h"
#include "slave.h"
#include "string.h"
#include "t2.h"

#define xdata
#define pdata
//#define data

#define HAL_NRF_DEBUG
#define HAL_NRF_RX_USE_BUF
#define HAL_NRF_SWITCH_DELAY		(800*16UL)
#define HAL_NRF_WAIT_ACK_DONE()		__delay_cycles(HAL_NRF_SWITCH_DELAY)

#define HAL_NRF_TICK				(100000/SYS_TICK_UNIT)
#define HAL_NRF_TIMEOUT				(10)

//#define HAL_NRF_TX_USE_STRUCT
//#define HAL_NRF_RX_USE_STRUCT

#ifdef HAL_NRF_TX_USE_STRUCT
#define HAL_NRF_TX_BUF_LEN			(5)
#else
#define HAL_NRF_TX_BUF_LEN			(255)
#endif

#ifdef HAL_NRF_RX_USE_STRUCT
#define HAL_NRF_RX_BUF_LEN			(8)
#else
#define HAL_NRF_RX_BUF_LEN			(255)
#endif

#define SPI_SS_BIT					(0x04)
#define SPI_SS_OUTPUT()				DDRB |= SPI_SS_BIT
#define SPI_MISO_BIT				(0x10)
#define SPI_MISO_INPUT()			DDRB &= ~SPI_MISO_BIT
#define SPI_MOSI_BIT				(0x08)
#define SPI_MOSI_OUTPUT()			DDRB |= SPI_MOSI_BIT
#define SPI_SCK_BIT					(0x20)
#define SPI_SCK_OUTPUT()			DDRB |= SPI_SCK_BIT


#ifdef USE_UNO
#define CSN_BIT				(0x08)
#define CSN_PORT			PORTC
#define CSN_OUTPUT()		DDRC |= CSN_BIT
#else
#define CSN_BIT				(0x01)
#define CSN_PORT			PORTC
#define CSN_OUTPUT()		DDRC |= CSN_BIT
#endif
#define CSN_L()				CSN_PORT &= ~CSN_BIT
#define CSN_H()				CSN_PORT |= CSN_BIT
#define CSN_V()				CSN_PORT ^= CSN_BIT

/** Macro that set radio's CSN line LOW. */
#define CSN_LOW() do { CSN_L(); } while(false)
/** Macro that set radio's CSN line HIGH.**/
#define CSN_HIGH() do { CSN_H(); } while(false)


#ifdef USE_UNO
#define CE_BIT				(0x04)
#define CE_PORT				PORTC
#define CE_OUTPUT()			DDRC |= CE_BIT
#else
#define CE_BIT				(0x02)
#define CE_PORT				PORTC
#define CE_OUTPUT()			DDRC |= CE_BIT
#endif
#define CE_L()				CE_PORT &= ~CE_BIT
#define CE_H()				CE_PORT |= CE_BIT
#define CE_V()				CE_PORT ^= CE_BIT
/** Macro that set radio's CE line LOW. */
#define CE_LOW() do { CE_L(); } while(false)
/** Macro that set radio's CE line HIGH. */
#define CE_HIGH() do { CE_H(); } while(false)

#define IRQ_BIT				(0x02)
#define IRQ_PORT			PORTB
#define IRQ					(PINB&IRQ_BIT)
#define IRQ_INPUT()			DDRB &= ~IRQ_BIT; IRQ_PORT |= IRQ_BIT
#define IRQ_EN()			do{PCICR |= 0x01; PCMSK0 |= 0x02;}while(0)
//#define IRQ_DIS()			do{PCMSK0 &= ~0x02;}while(0)
#define IRQ_DIS()			do{PCICR &= ~0x01;}while(0)
#define IRQ_VECT			PCINT0_vect


/** Macro for writing the radio SPI data register. */
#define HAL_NRF_HW_SPI_WRITE(d) do{SPDR = (d);} while(false)
/** Macro for reading the radio SPI data register. */
#define HAL_NRF_HW_SPI_READ() (SPDR)

/** Macro specifyng the radio SPI busy flag. */
#define HAL_NRF_HW_SPI_BUSY (!(SPSR & (1<<SPIF)))

/**
 * Pulses the CE to nRF24L01p for at least 10 us
 */
//#define CE_PULSE() do { \
//  uint8_t count; \
//  count = 20U; \
//  CE_HIGH();  \
//    while(count--) {} \
//  CE_LOW();  \
//  } while(false)

#define CE_PULSE() do{CE_HIGH(); __delay_cycles(16*15); CE_LOW();}while(0)

typedef struct{
	u8 len;
	u8 buf[32];
}hal_nrf_pkt_t;

typedef enum{
	HAL_NRF_RX_STA_IDLE,
	HAL_NRF_RX_STA_ABOUT_FULL,
	HAL_NRF_RX_STA_FULL,
	HAL_NRF_RX_STA_TIMEOUT,
	HAL_NRF_RX_STA_COM_ERROR,
	HAL_NRF_RX_STA_BUF_OVF,
}hal_nrf_rx_sta_t;

typedef enum{
	HAL_NRF_TX_STA_IDLE,
	HAL_NRF_TX_STA_DONE,
}hal_nrf_tx_sta_t;

typedef enum{
	HAL_NRF_STA_RX,
	HAL_NRF_STA_TX,
	HAL_NRF_STA_TX_CMD,
//	HAL_NRF_STA_TX_TIMEOUT
}hal_nrf_sta_t;

#ifdef SLAVE_DEBUG
extern u8 hal_nrf_irq_flag;
extern u8 hal_nrf_rx_rd_index, hal_nrf_rx_cnt, hal_nrf_rx_wr_index;
extern hal_nrf_rx_sta_t hal_nrf_rx_sta;
#endif

void spi_init(void);
void hal_nrf_init(void);
void hal_nrf_send_pkt(u8 *buf, u8 len);
void hal_nrf_sta_handle(void);
u8 hal_nrf_receive_pkt(u8 *buf);
void read_flash_buf(u8 *buf, u8 __flash *fl, u8 len);
void hal_nrf_send_start(u8 *buf, u8 len);
void hal_nrf_set_sta(hal_nrf_sta_t sta);
void hal_nrf_send_cmd(u8 *buf, u8 sta);
void hal_nrf_send_ack(void);
void hal_nrf_clear_tx();

#endif // HAL_NRF_LE1_H__

/** @} */
/** @} */
