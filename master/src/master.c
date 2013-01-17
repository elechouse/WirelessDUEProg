#include "master.h"

code u8 master_cmd_tx_allow[CMD_LENGTH]={
	MASTER_CMD_TX_ALLOW,CMD_FILL, CMD_FILL, CMD_FILL, CMD_FILL,
	CMD_FILL, CMD_FILL, CMD_FILL, CMD_FILL, CMD_FILL,
};

code u8 master_cmd_ack[CMD_LENGTH]={
	MASTER_CMD_ACK, CMD_FILL, CMD_FILL, CMD_FILL, CMD_FILL,
	CMD_FILL, CMD_FILL, CMD_FILL, CMD_FILL, CMD_FILL,
};
u8 master_cmd[CMD_LENGTH];
void master_parse_cmd(u8 *buf)
{
	switch(buf[0]){
		case SLAVE_CMD_TX_REQUEST:
			/** send command MASTER_CMD_TX_ALLOW */
			hal_nrf_send_cmd(master_cmd_tx_allow, HAL_NRF_STA_RX_BUSY);
			break;
		case SLAVE_CMD_TX_END:
			/** set rx idle */
			hal_nrf_set_sta(HAL_NRF_STA_RX_IDLE);
			break;
		default:
			break;
	}
}