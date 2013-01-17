#ifndef __SLAVE_H
#define __SLAVE_H

#include "hardware.h"
#include "hal_nrf_hw.h"
#include "print.h"

//#define RE_DEBUG

#define CMD_FILL								(0xAA)
#define CMD_LENGTH								(10)
#define SLAVE_CMD_TX_REQUEST					(0x00)
#define SLAVE_CMD_TX_END						(0x7F)

#define MASTER_CMD_TX_ALLOW						(0x80)
#define MASTER_CMD_ACK							(0xFF)
#define MASTER_CMD_SET_CONTROL_LINE_STATE		(0x81)
#define MASTER_CMD_GET_LINE_CODING				(0x82)
#define MASTER_CMD_SET_LINE_CODING				(0x83)
#define MASTER_CMD_ 

#ifdef USE_UNO
#define SLAVE_RESET_BIT							(0x04)
#define SLAVE_RESET_PORT						PORTD
#define SLAVE_RESET_OUTPUT()					DDRD |= SLAVE_RESET_BIT
#define SLAVE_RESET_INPUT()						DDRD &= ~SLAVE_RESET_BIT
#define SLAVE_RESET_H()							SLAVE_RESET_PORT |= SLAVE_RESET_BIT
#define SLAVE_RESET_L()							SLAVE_RESET_PORT &= ~SLAVE_RESET_BIT
#define SLAVE_RESET_V()							SLAVE_RESET_PORT ^= SLAVE_RESET_BIT

#define SLAVE_ERASE_BIT							(0x08)
#define SLAVE_ERASE_PORT						PORTD
#define SLAVE_ERASE_OUTPUT()					DDRD |= SLAVE_ERASE_BIT
#define SLAVE_ERASE_INPUT()						DDRD &= ~SLAVE_ERASE_BIT
#define SLAVE_ERASE_H()							SLAVE_ERASE_PORT |= SLAVE_ERASE_BIT
#define SLAVE_ERASE_L()							SLAVE_ERASE_PORT &= ~SLAVE_ERASE_BIT
#define SLAVE_ERASE_V()							SLAVE_ERASE_PORT ^= SLAVE_ERASE_BIT

#define CDC_CONTROL_LINE_OUT_DTR				(0x01)
#define CDC_CONTROL_LINE_OUT_RTS				(0x02)

#define LED0_BIT								(0x10)
#define LED0_PORT								PORTD
#define LED0_OUTPUT()							DDRD |= LED0_BIT
#define LED0_INPUT()							DDRD &= ~LED0_BIT
#define LED0_H()								LED0_PORT |= LED0_BIT
#define LED0_L()								LED0_PORT &= ~LED0_BIT
#define LED0_V()								LED0_PORT ^= LED0_BIT

#define LED1_BIT								(0x20)
#define LED1_PORT								PORTD
#define LED1_OUTPUT()							DDRD |= LED1_BIT
#define LED1_INPUT()							DDRD &= ~LED1_BIT
#define LED1_H()								LED1_PORT |= LED1_BIT
#define LED1_L()								LED1_PORT &= ~LED1_BIT
#define LED1_V()								LED1_PORT ^= LED1_BIT

#define LED_D4_BIT								(0x10)
#define LED_D4_PORT								PORTD
#define LED_D4_OUTPUT()							DDRD |= LED_D4_BIT
#define LED_D4_INPUT()							DDRD &= ~LED_D4_BIT
#define LED_D4_H()								LED_D4_PORT |= LED_D4_BIT
#define LED_D4_L()								LED_D4_PORT &= ~LED_D4_BIT
#define LED_D4_V()								LED_D4_PORT ^= LED_D4_BIT

#define LED_D5_BIT								(0x20)
#define LED_D5_PORT								PORTD
#define LED_D5_OUTPUT()							DDRD |= LED_D5_BIT
#define LED_D5_INPUT()							DDRD &= ~LED_D5_BIT
#define LED_D5_H()								LED_D5_PORT |= LED_D5_BIT
#define LED_D5_L()								LED_D5_PORT &= ~LED_D5_BIT
#define LED_D5_V()								LED_D5_PORT ^= LED_D5_BIT

#define LED_D6_BIT								(0x40)
#define LED_D6_PORT								PORTD
#define LED_D6_OUTPUT()							DDRD |= LED_D6_BIT
#define LED_D6_INPUT()							DDRD &= ~LED_D6_BIT
#define LED_D6_H()								LED_D6_PORT |= LED_D6_BIT
#define LED_D6_L()								LED_D6_PORT &= ~LED_D6_BIT
#define LED_D6_V()								LED_D6_PORT ^= LED_D6_BIT

#define LED_D7_BIT								(0x80)
#define LED_D7_PORT								PORTD
#define LED_D7_OUTPUT()							DDRD |= LED_D7_BIT
#define LED_D7_INPUT()							DDRD &= ~LED_D7_BIT
#define LED_D7_H()								LED_D7_PORT |= LED_D7_BIT
#define LED_D7_L()								LED_D7_PORT &= ~LED_D7_BIT
#define LED_D7_V()								LED_D7_PORT ^= LED_D7_BIT

#define LED_D8_BIT								(0x01)
#define LED_D8_PORT								PORTB
#define LED_D8_OUTPUT()							DDRB |= LED_D8_BIT
#define LED_D8_INPUT()							DDRB &= ~LED_D8_BIT
#define LED_D8_H()								LED_D8_PORT |= LED_D8_BIT
#define LED_D8_L()								LED_D8_PORT &= ~LED_D8_BIT
#define LED_D8_V()								LED_D8_PORT ^= LED_D8_BIT

#define LED_A0_BIT								(0x01)
#define LED_A0_PORT								PORTC
#define LED_A0_OUTPUT()							DDRC |= LED_A0_BIT
#define LED_A0_INPUT()							DDRC &= ~LED_A0_BIT
#define LED_A0_H()								LED_A0_PORT |= LED_A0_BIT
#define LED_A0_L()								LED_A0_PORT &= ~LED_A0_BIT
#define LED_A0_V()								LED_A0_PORT ^= LED_A0_BIT

#define LED_A1_BIT								(0x02)
#define LED_A1_PORT								PORTC
#define LED_A1_OUTPUT()							DDRC |= LED_A1_BIT
#define LED_A1_INPUT()							DDRC &= ~LED_A1_BIT
#define LED_A1_H()								LED_A1_PORT |= LED_A1_BIT
#define LED_A1_L()								LED_A1_PORT &= ~LED_A1_BIT
#define LED_A1_V()								LED_A1_PORT ^= LED_A1_BIT

#define LED_A4_BIT								(0x10)
#define LED_A4_PORT								PORTC
#define LED_A4_OUTPUT()							DDRC |= LED_A4_BIT
#define LED_A4_INPUT()							DDRC &= ~LED_A4_BIT
#define LED_A4_H()								LED_A4_PORT |= LED_A4_BIT
#define LED_A4_L()								LED_A4_PORT &= ~LED_A4_BIT
#define LED_A4_V()								LED_A4_PORT ^= LED_A4_BIT

#define LED_A5_BIT								(0x20)
#define LED_A5_PORT								PORTC
#define LED_A5_OUTPUT()							DDRC |= LED_A5_BIT
#define LED_A5_INPUT()							DDRC &= ~LED_A5_BIT
#define LED_A5_H()								LED_A5_PORT |= LED_A5_BIT
#define LED_A5_L()								LED_A5_PORT &= ~LED_A5_BIT
#define LED_A5_V()								LED_A5_PORT ^= LED_A5_BIT

#else/** USE_UNO */

#define SLAVE_RESET_BIT							(0x04)
#define SLAVE_RESET_PORT						PORTD
#define SLAVE_RESET_OUTPUT()					DDRD |= SLAVE_RESET_BIT
#define SLAVE_RESET_INPUT()						DDRD &= ~SLAVE_RESET_BIT
#define SLAVE_RESET_H()							SLAVE_RESET_PORT |= SLAVE_RESET_BIT
#define SLAVE_RESET_L()							SLAVE_RESET_PORT &= ~SLAVE_RESET_BIT
#define SLAVE_RESET_V()							SLAVE_RESET_PORT ^= SLAVE_RESET_BIT

#define SLAVE_ERASE_BIT							(0x10)
#define SLAVE_ERASE_PORT						PORTD
#define SLAVE_ERASE_OUTPUT()					DDRD |= SLAVE_ERASE_BIT
#define SLAVE_ERASE_INPUT()						DDRD &= ~SLAVE_ERASE_BIT
#define SLAVE_ERASE_H()							SLAVE_ERASE_PORT |= SLAVE_ERASE_BIT
#define SLAVE_ERASE_L()							SLAVE_ERASE_PORT &= ~SLAVE_ERASE_BIT
#define SLAVE_ERASE_V()							SLAVE_ERASE_PORT ^= SLAVE_ERASE_BIT

#define CDC_CONTROL_LINE_OUT_DTR				(0x01)
#define CDC_CONTROL_LINE_OUT_RTS				(0x02)

#define LED_D4_BIT								(0x04)
#define LED_D4_PORT								PORTC
#define LED_D4_OUTPUT()							DDRC |= LED_D4_BIT
#define LED_D4_INPUT()							DDRC &= ~LED_D4_BIT
#define LED_D4_H()								LED_D4_PORT |= LED_D4_BIT
#define LED_D4_L()								LED_D4_PORT &= ~LED_D4_BIT
#define LED_D4_V()								LED_D4_PORT ^= LED_D4_BIT

#define LED_D5_BIT								(0x20)
#define LED_D5_PORT								PORTD
#define LED_D5_OUTPUT()							DDRD |= LED_D5_BIT
#define LED_D5_INPUT()							DDRD &= ~LED_D5_BIT
#define LED_D5_H()								LED_D5_PORT |= LED_D5_BIT
#define LED_D5_L()								LED_D5_PORT &= ~LED_D5_BIT
#define LED_D5_V()								LED_D5_PORT ^= LED_D5_BIT

#define LED_D6_BIT								(0x40)
#define LED_D6_PORT								PORTD
#define LED_D6_OUTPUT()							DDRD |= LED_D6_BIT
#define LED_D6_INPUT()							DDRD &= ~LED_D6_BIT
#define LED_D6_H()								LED_D6_PORT |= LED_D6_BIT
#define LED_D6_L()								LED_D6_PORT &= ~LED_D6_BIT
#define LED_D6_V()								LED_D6_PORT ^= LED_D6_BIT

#define LED_D7_BIT								(0x80)
#define LED_D7_PORT								PORTD
#define LED_D7_OUTPUT()							DDRD |= LED_D7_BIT
#define LED_D7_INPUT()							DDRD &= ~LED_D7_BIT
#define LED_D7_H()								LED_D7_PORT |= LED_D7_BIT
#define LED_D7_L()								LED_D7_PORT &= ~LED_D7_BIT
#define LED_D7_V()								LED_D7_PORT ^= LED_D7_BIT

#define LED_D8_BIT								(0x01)
#define LED_D8_PORT								PORTB
#define LED_D8_OUTPUT()							DDRB |= LED_D8_BIT
#define LED_D8_INPUT()							DDRB &= ~LED_D8_BIT
#define LED_D8_H()								LED_D8_PORT |= LED_D8_BIT
#define LED_D8_L()								LED_D8_PORT &= ~LED_D8_BIT
#define LED_D8_V()								LED_D8_PORT ^= LED_D8_BIT

#define LED_A0_BIT								(0x04)
#define LED_A0_PORT								PORTC
#define LED_A0_OUTPUT()							DDRC |= LED_A0_BIT
#define LED_A0_INPUT()							DDRC &= ~LED_A0_BIT
#define LED_A0_H()								LED_A0_PORT |= LED_A0_BIT
#define LED_A0_L()								LED_A0_PORT &= ~LED_A0_BIT
#define LED_A0_V()								LED_A0_PORT ^= LED_A0_BIT

#define LED_A1_BIT								(0x08)
#define LED_A1_PORT								PORTC
#define LED_A1_OUTPUT()							DDRC |= LED_A1_BIT
#define LED_A1_INPUT()							DDRC &= ~LED_A1_BIT
#define LED_A1_H()								LED_A1_PORT |= LED_A1_BIT
#define LED_A1_L()								LED_A1_PORT &= ~LED_A1_BIT
#define LED_A1_V()								LED_A1_PORT ^= LED_A1_BIT

#define LED_A4_BIT								(0x10)
#define LED_A4_PORT								PORTC
#define LED_A4_OUTPUT()							DDRC |= LED_A4_BIT
#define LED_A4_INPUT()							DDRC &= ~LED_A4_BIT
#define LED_A4_H()								LED_A4_PORT |= LED_A4_BIT
#define LED_A4_L()								LED_A4_PORT &= ~LED_A4_BIT
#define LED_A4_V()								LED_A4_PORT ^= LED_A4_BIT

#define LED_A5_BIT								(1<<3)
#define LED_A5_PORT								PORTD
#define LED_A5_OUTPUT()							DDRD |= LED_A5_BIT
#define LED_A5_INPUT()							DDRD &= ~LED_A5_BIT
#define LED_A5_H()								LED_A5_PORT |= LED_A5_BIT
#define LED_A5_L()								LED_A5_PORT &= ~LED_A5_BIT
#define LED_A5_V()								LED_A5_PORT ^= LED_A5_BIT
#endif
typedef struct{
	u32 dwDTERate;
	u8 bCharFormat;
	u8 bParityType;
	u8 bDataBits;
}usb_cdc_line_coding_t;


extern u8 slave_cmd[];
extern __flash u8 slave_cmd_end[10];
extern __flash u8 slave_cmd_request[10];

void slave_parse_cmd(u8 *buf);
void slave_io_init();

#endif