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
 * $LastChangedRevision: 2519 $
 */

/** @file
@brief Register definitions for nRF24L01+

@details Header file defining register mapping with bit definitions.
This file is radio-chip dependent, and are included with the hal_nrf.h

@ingroup hal_nrf24l01p_hal_nrf
 */

#ifndef HAL_NRF_REG_H__
#define HAL_NRF_REG_H__

/** @name - Instruction Set - */
//@{
/* nRF24L01 Instruction Definitions */
#define W_REGISTER         0x20U  /**< Register write command */
#define R_RX_PAYLOAD       0x61U  /**< Read RX payload command */
#define W_TX_PAYLOAD       0xA0U  /**< Write TX payload command */
#define FLUSH_TX           0xE1U  /**< Flush TX register command */
#define FLUSH_RX           0xE2U  /**< Flush RX register command */
#define REUSE_TX_PL        0xE3U  /**< Reuse TX payload command */
#define ACTIVATE           0x50U  /**< Activate features */
#define R_RX_PL_WID        0x60U  /**< Read RX payload command */
#define W_ACK_PAYLOAD      0xA8U  /**< Write ACK payload command */
#define W_TX_PAYLOAD_NOACK 0xB0U  /**< Write ACK payload command */
#define NOP                0xFFU  /**< No Operation command, used for reading status register */
//@}

/** @name  - Register Memory Map - */
//@{
/* nRF24L01 * Register Definitions * */
#define CONFIG        0x00U  /**< nRF24L01 config register */
#define EN_AA         0x01U  /**< nRF24L01 enable Auto-Acknowledge register */
#define EN_RXADDR     0x02U  /**< nRF24L01 enable RX addresses register */
#define SETUP_AW      0x03U  /**< nRF24L01 setup of address width register */
#define SETUP_RETR    0x04U  /**< nRF24L01 setup of automatic retransmission register */
#define RF_CH         0x05U  /**< nRF24L01 RF channel register */
#define RF_SETUP      0x06U  /**< nRF24L01 RF setup register */
#define STATUS        0x07U  /**< nRF24L01 status register */
#define OBSERVE_TX    0x08U  /**< nRF24L01 transmit observe register */
#define CD            0x09U  /**< nRF24L01 carrier detect register */
#define RX_ADDR_P0    0x0AU  /**< nRF24L01 receive address data pipe0 */
#define RX_ADDR_P1    0x0BU  /**< nRF24L01 receive address data pipe1 */
#define RX_ADDR_P2    0x0CU  /**< nRF24L01 receive address data pipe2 */
#define RX_ADDR_P3    0x0DU  /**< nRF24L01 receive address data pipe3 */
#define RX_ADDR_P4    0x0EU  /**< nRF24L01 receive address data pipe4 */
#define RX_ADDR_P5    0x0FU  /**< nRF24L01 receive address data pipe5 */
#define TX_ADDR       0x10U  /**< nRF24L01 transmit address */
#define RX_PW_P0      0x11U  /**< nRF24L01 \# of bytes in rx payload for pipe0 */
#define RX_PW_P1      0x12U  /**< nRF24L01 \# of bytes in rx payload for pipe1 */
#define RX_PW_P2      0x13U  /**< nRF24L01 \# of bytes in rx payload for pipe2 */
#define RX_PW_P3      0x14U  /**< nRF24L01 \# of bytes in rx payload for pipe3 */
#define RX_PW_P4      0x15U  /**< nRF24L01 \# of bytes in rx payload for pipe4 */
#define RX_PW_P5      0x16U  /**< nRF24L01 \# of bytes in rx payload for pipe5 */
#define FIFO_STATUS   0x17U  /**< nRF24L01 FIFO status register */
#define DYNPD         0x1CU  /**< nRF24L01 Dynamic payload setup */
#define FEATURE       0x1DU  /**< nRF24L01 Exclusive feature setup */

//@}

/* nRF24L01 related definitions */
/* Interrupt definitions */
/* Operation mode definitions */

/** An enum describing the radio's irq sources.
 *
 */
typedef enum {
    HAL_NRF_MAX_RT = 4,     /**< Max retries interrupt */
    HAL_NRF_TX_DS,          /**< TX data sent interrupt */
    HAL_NRF_RX_DR           /**< RX data received interrupt */
} hal_nrf_irq_source_t;

/* Operation mode definitions */
/** An enum describing the radio's power mode.
 *
 */
typedef enum {
    HAL_NRF_PTX,            /**< Primary TX operation */
    HAL_NRF_PRX             /**< Primary RX operation */
} hal_nrf_operation_mode_t;

/** An enum describing the radio's power mode.
 *
 */
typedef enum {
    HAL_NRF_PWR_DOWN,       /**< Device power-down */
    HAL_NRF_PWR_UP          /**< Device power-up */
} hal_nrf_pwr_mode_t;

/** An enum describing the radio's output power mode's.
 *
 */
typedef enum {
    HAL_NRF_18DBM,          /**< Output power set to -18dBm */
    HAL_NRF_12DBM,          /**< Output power set to -12dBm */
    HAL_NRF_6DBM,           /**< Output power set to -6dBm  */
    HAL_NRF_0DBM            /**< Output power set to 0dBm   */
} hal_nrf_output_power_t;

/** An enum describing the radio's on-air datarate.
 *
 */
typedef enum {
    HAL_NRF_1MBPS,          /**< Datarate set to 1 Mbps  */
    HAL_NRF_2MBPS,          /**< Datarate set to 2 Mbps  */
    HAL_NRF_250KBPS         /**< Datarate set to 250 kbps*/
} hal_nrf_datarate_t;

/** An enum describing the radio's CRC mode.
 *
 */
typedef enum {
    HAL_NRF_CRC_OFF,    /**< CRC check disabled */
    HAL_NRF_CRC_8BIT,   /**< CRC check set to 8-bit */
    HAL_NRF_CRC_16BIT   /**< CRC check set to 16-bit */
} hal_nrf_crc_mode_t;

/** An enum describing the read/write payload command.
 *
 */
typedef enum {
    HAL_NRF_TX_PLOAD = 7,   /**< TX payload definition */
    HAL_NRF_RX_PLOAD,        /**< RX payload definition */
    HAL_NRF_ACK_PLOAD
} hal_nrf_pload_command_t;

/** Structure containing the radio's address map.
 * Pipe0 contains 5 unique address bytes,
 * while pipe[1..5] share the 4 MSB bytes, set in pipe1.
 * <p><b> - Remember that the LSB byte for all pipes have to be unique! -</b>
 */
// nRF24L01 Address struct


//typedef struct {
//   uint8_t p0[5];            /**< Pipe0 address, 5 bytes */
//    uint8_t p1[5];            /**< Pipe1 address, 5 bytes, 4 MSB bytes shared for pipe1 to pipe5 */
//    uint8_t p2[1];            /**< Pipe2 address, 1 byte */
//    uint8_t p3[1];            /**< Pipe3 address, 1 byte */
//   uint8_t p4[1];            /**< Pipe3 address, 1 byte */
//   uint8_t p5[1];            /**< Pipe3 address, 1 byte */
//   uint8_t tx[5];            /**< TX address, 5 byte */
//} hal_nrf_l01_addr_map;


/** An enum describing the nRF24L01 pipe addresses and TX address.
 *
 */
typedef enum {
    HAL_NRF_PIPE0 = 0,          /**< Select pipe0 */
    HAL_NRF_PIPE1,              /**< Select pipe1 */
    HAL_NRF_PIPE2,              /**< Select pipe2 */
    HAL_NRF_PIPE3,              /**< Select pipe3 */
    HAL_NRF_PIPE4,              /**< Select pipe4 */
    HAL_NRF_PIPE5,              /**< Select pipe5 */
    HAL_NRF_TX,                 /**< Refer to TX address*/
    HAL_NRF_ALL = 0xFF          /**< Close or open all pipes*/
                                /**< @see hal_nrf_set_address @see hal_nrf_get_address
                                 @see hal_nrf_open_pipe  @see hal_nrf_close_pipe */
} hal_nrf_address_t;

/** An enum describing the radio's address width.
 *
 */
typedef enum {
    HAL_NRF_AW_3BYTES = 3,      /**< Set address width to 3 bytes */
    HAL_NRF_AW_4BYTES,          /**< Set address width to 4 bytes */
    HAL_NRF_AW_5BYTES           /**< Set address width to 5 bytes */
} hal_nrf_address_width_t;


/** @name CONFIG register bit definitions */
//@{

#define MASK_RX_DR    6     /**< CONFIG register bit 6 */
#define MASK_TX_DS    5     /**< CONFIG register bit 5 */
#define MASK_MAX_RT   4     /**< CONFIG register bit 4 */
#define EN_CRC        3     /**< CONFIG register bit 3 */
#define CRCO          2     /**< CONFIG register bit 2 */
#define PWR_UP        1     /**< CONFIG register bit 1 */
#define PRIM_RX       0     /**< CONFIG register bit 0 */
//@}

/** @name RF_SETUP register bit definitions */
//@{
#define PLL_LOCK      4     /**< RF_SETUP register bit 4 */
#define RF_DR         3     /**< RF_SETUP register bit 3 */
#define RF_PWR1       2     /**< RF_SETUP register bit 2 */
#define RF_PWR0       1     /**< RF_SETUP register bit 1 */
#define LNA_HCURR     0     /**< RF_SETUP register bit 0 */
//@}

/* STATUS 0x07 */
/** @name STATUS register bit definitions */
//@{
#define RX_DR         6     /**< STATUS register bit 6 */
#define TX_DS         5     /**< STATUS register bit 5 */
#define MAX_RT        4     /**< STATUS register bit 4 */
#define TX_FULL       0     /**< STATUS register bit 0 */
//@}

/* FIFO_STATUS 0x17 */
/** @name FIFO_STATUS register bit definitions */
//@{
#define TX_REUSE      6     /**< FIFO_STATUS register bit 6 */
#define TX_FIFO_FULL  5     /**< FIFO_STATUS register bit 5 */
#define TX_EMPTY      4     /**< FIFO_STATUS register bit 4 */
#define RX_FULL       1     /**< FIFO_STATUS register bit 1 */
#define RX_EMPTY      0     /**< FIFO_STATUS register bit 0 */
//@}

/* register CONFIG, addr: 0x00 */
//#define MASK_RX_DR      (0x40)
//#define MASK_TX_DS      (0x20)
//#define MASK_MAX_RT     (0x10)
//#define EN_CRC          (0x08)
//#define CRCO            (0x04)
//#define PWR_UP          (0x02)
//#define PRIM_RX         (0x01)

/* register EN_AA, addr: 0x01 */
#define ENAA_P5         (0x20)
#define ENAA_P4         (0x10)
#define ENAA_P3         (0x08)
#define ENAA_P2         (0x04)
#define ENAA_P1         (0x02)
#define ENAA_P0         (0x01) 

/* register EN_RXADDR, addr: 0x02 */
#define ERX_P5          (0x20)
#define ERX_P4          (0x10)
#define ERX_P3          (0x08)
#define ERX_P2          (0x04)
#define ERX_P1          (0x02)
#define ERX_P0          (0x01)   

/* register SETUP_AW, addr: 0x03 */
#define AW1             (0x02)   
#define AW0             (0x01)

/* register SETUP_RETR, addr: 0x04 */
#define ARD3            (0x80)   
#define ARD2            (0x40
#define ARD1            (0x20)   
#define ARD0            (0x10)   
#define ARC3            (0x08
#define ARC2            (0x04) 
#define ARC1            (0x02) 
#define ARC0            (0x01) 

/* register RF_CH, addr 0x05*/
#define RF_CH6          (0x40)
#define RF_CH5          (0x20)
#define RF_CH4          (0x10)
#define RF_CH3          (0x08)
#define RF_CH2          (0x04)
#define RF_CH1          (0x02)
#define RF_CH0          (0x01)

/* register RF_SETUP, addr 0x06*/
#define CONT_WAVE       (0x80)
#define RF_DR_LOW       (0x40)
//#define PLL_LOCK        (0x10)    
#define RF_DR_HIGH      (0x08)    
//#define RF_PWR1         (0x04)    
//#define RF_PWR0         (0x02)    
//#define LNA_HCURR       (0x01)    //obsolete

/* register STATUS, addr 0x07*/
//#define RX_DR           (0x40)
//#define TX_DS           (0x20)
//#define MAX_RT          (0x10)
#define RX_P_NO2        (0x08)
#define RX_P_NO1        (0x04)
#define RX_P_NO0        (0x02) 
#define STATUS_TX_FULL  (0x01)

/* register OBSERVE_TX, addr 0x08 */
#define PLOS_CNT3       (0x80)    
#define PLOS_CNT2       (0x40)
#define PLOS_CNT1       (0x20)
#define PLOS_CNT0       (0x10)
#define ARC_CNT3        (0x08)
#define ARC_CNT2        (0x04)
#define ARC_CNT1        (0x02)
#define ARC_CNT0        (0x01)

/* register RPD, addr 0x09 */
//#define CD              (0x01)

/* register RX_ADDR_P0, addr 0x0a */
#define RX_PW_P05       (0x20)  
#define RX_PW_P04       (0x10)  
#define RX_PW_P03       (0x08)
#define RX_PW_P02       (0x04)
#define RX_PW_P01       (0x02)
#define RX_PW_P00       (0x01)

/* register */
#define RX_PW_P15       (0x20)  
#define RX_PW_P14       (0x10)  
#define RX_PW_P13       (0x08)
#define RX_PW_P12       (0x04)
#define RX_PW_P11       (0x02)
#define RX_PW_P10       (0x01)

/* register */
#define RX_PW_P25       (0x20)  
#define RX_PW_P24       (0x10)  
#define RX_PW_P23       (0x08)
#define RX_PW_P22       (0x04)
#define RX_PW_P21       (0x02)
#define RX_PW_P20       (0x01)

/* register */
#define RX_PW_P35       (0x20)  
#define RX_PW_P34       (0x10)  
#define RX_PW_P33       (0x08)
#define RX_PW_P32       (0x04)
#define RX_PW_P31       (0x02)
#define RX_PW_P30       (0x01)

/* register */
#define RX_PW_P45       (0x20)  
#define RX_PW_P44       (0x10)  
#define RX_PW_P43       (0x08)
#define RX_PW_P42       (0x04)
#define RX_PW_P41       (0x02)
#define RX_PW_P40       (0x01)

/* register */
#define RX_PW_P55       (0x20)  
#define RX_PW_P54       (0x10)  
#define RX_PW_P53       (0x08)
#define RX_PW_P52       (0x04)
#define RX_PW_P51       (0x02)
#define RX_PW_P50       (0x01)

/* register FIFO_STATUS, addr 0x17 */
//#define TX_REUSE        (0x40)  
//#define TX_FULL         (0x20)  
//#define TX_EMPTY        (0x10)  
//#define RX_FULL         (0x02)  
//#define RX_EMPTY        (0x01)  

/* register */
#define DPL_P5          (0x20)
#define DPL_P4          (0x10)
#define DPL_P3          (0x08)
#define DPL_P2          (0x04)
#define DPL_P1          (0x02)
#define DPL_P0          (0x01)

/* register FEATURE, addr 0x1D */
#define EN_DPL          (0x04)
#define EN_ACK_PAY      (0x02)
#define EN_DYN_ACK      (0x01)

#endif // HAL_NRF_REG_H__
/** @} */
