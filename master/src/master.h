#ifndef __MASTER_H
#define __MASTER_H

#include "hardware.h"
#include "hal_nrf_hw.h"
#include "print.h"

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

extern code u8 master_cmd_ack[CMD_LENGTH];
extern code u8 master_cmd_tx_allow[CMD_LENGTH];
extern u8 master_cmd[CMD_LENGTH];

void master_parse_cmd(u8 *buf);

#endif