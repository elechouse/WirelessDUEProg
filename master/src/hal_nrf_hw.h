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
* @brief hal_nrf macros for nRF24LU1+
 *
 * @defgroup hal_nrf24lu1p_hal_nrf_hw hal_nrf_hw
 * @{
 * @ingroup hal_nrf24lu1p
 *
 */

#ifndef HAL_NRF_LU1_H__
#define HAL_NRF_LU1_H__
#include "hardware.h"
#include "usb.h"
#include "t2.h"
#include "master.h"

#define HAL_NRF_SWITCH_DELAY		(1500)
#define HAL_NRF_WAIT_ACK_DONE()		delay(HAL_NRF_SWITCH_DELAY)
//#define HAL_NRF_WAIT_ACK_DONE()		delay_us(HAL_NRF_SWITCH_DELAY)

#define HAL_NRF_RX_USE_BUF

//#define HAL_NRF_TX_USE_STRUCT
//#define HAL_NRF_RX_USE_STRUCT

#ifdef HAL_NRF_TX_USE_STRUCT
//#define HAL_NRF_TX_BUF_LEN			(5)
#else
#define HAL_NRF_TX_BUF_LEN				(255)
#endif

#ifdef HAL_NRF_RX_USE_STRUCT
//#define HAL_NRF_RX_BUF_LEN			(8)
#else
#define HAL_NRF_RX_BUF_LEN				(255)
#endif

#define IRQ_EN()					RF = 1
#define IRQ_DIS()					RF = 0

#define CSN_OUTPUT()
#define CE_OUTPUT()

/** Macro that set radio's CSN line LOW.
 *
 */
#define CSN_LOW() do { RFCSN = 0; } while(false)

/** Macro that set radio's CSN line HIGH.
 *
 */
#define CSN_HIGH() do { RFCSN = 1; } while(false)

/** Macro that set radio's CE line LOW.
 *
 */
#define CE_LOW() do { RFCE = 0; } while(false)

/** Macro that set radio's CE line HIGH.
 *
 */
#define CE_HIGH() do { RFCE = 1; } while(false)

/** Macro for writing the radio SPI data register.
 *
 */
#define HAL_NRF_HW_SPI_WRITE(d) do{RFDAT = d; RFSPIF = 0;} while(false)

/** Macro for reading the radio SPI data register.
 *
 */
#define HAL_NRF_HW_SPI_READ() RFDAT
  
/** Macro specifyng the radio SPI busy flag.
 *
 */
#define HAL_NRF_HW_SPI_BUSY (!RFSPIF)

/**
 * Pulses the CE to nRF24L01 for at least 10 us
 */
#define CE_PULSE() do { \
  uint8_t count; \
  count = 20; \
  CE_HIGH();  \
  while(count--){} \
  CE_LOW();  \
  } while(false)


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
	HAL_NRF_TX_STA_TIMEOUT,
	HAL_NRF_TX_STA_BUF_OVF,
}hal_nrf_tx_sta_t;
//extern u8 hal_nrf_irq_flag;

typedef enum{
	HAL_NRF_STA_RX_IDLE,
	HAL_NRF_STA_RX_BUSY,
	HAL_NRF_STA_TX,
	HAL_NRF_STA_TX_CMD,
//	HAL_NRF_STA_TX_TIMEOUT
}hal_nrf_sta_t;

void spi_init(void);
void hal_nrf_init(void);
void hal_nrf_send_pkt(u8 *buf, u8 len);
void hal_nrf_sta_handle(void);
void hal_nrf_set_sta(u8 sta);
void hal_nrf_send_cmd(u8 *buf, u8 sta);
u8 hal_nrf_get_tx_cnt();

#ifdef MASTER_DEBUG
extern u8 nrf_rx_payload[];
extern u8 hal_nrf_irq_flag;
extern u8 nrf_rx_len;
//extern u8 hal_nrf_tx_cnt_max;
#endif

#endif // HAL_NRF_LU1_H__

/** @} */
