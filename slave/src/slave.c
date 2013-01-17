#include "slave.h"

__flash u8 slave_cmd_request[CMD_LENGTH] = {
	SLAVE_CMD_TX_REQUEST,  CMD_FILL, CMD_FILL, CMD_FILL, CMD_FILL,
	CMD_FILL, CMD_FILL, CMD_FILL, CMD_FILL, CMD_FILL,
};

__flash u8 slave_cmd_end[CMD_LENGTH] = {
	SLAVE_CMD_TX_END, CMD_FILL, CMD_FILL, CMD_FILL, CMD_FILL,
	CMD_FILL, CMD_FILL, CMD_FILL, CMD_FILL, CMD_FILL,
};

u8 slave_cmd[CMD_LENGTH];
usb_cdc_line_coding_t slave_line_coding;

__root bool CurrentDTRState = false;
__root bool PreviousDTRState = false;
__root bool CurrentRTSState = false;
__root bool PreviousRTSState = false;
__root bool Selected1200BPS = false;
u8 ResetTimer=0;

void setResetPin(bool v) {
	/* Target /RESET line  */
	if (v) {
		/* ACTIVE   => OUTPUT LOW (0v on target /RESET) */
		SLAVE_RESET_OUTPUT();
		SLAVE_RESET_L();
#ifdef RE_DEBUG
		LED0_OUTPUT();
		LED0_L();
#endif
	} else {
	 	/* INACTIVE => set as INPUT (internal pullup on target /RESET keep it at 3.3v) */
		SLAVE_RESET_INPUT();
		SLAVE_RESET_L();
		
#ifdef RE_DEBUG
		LED0_INPUT();
		LED0_L();
#endif
	}
}

void setErasePin(bool v) {
	if (v) {
		SLAVE_ERASE_L();
#ifdef RE_DEBUG
		LED1_L();
#endif
	} else {
		SLAVE_ERASE_H();
#ifdef RE_DEBUG
		LED1_H();
#endif
	}
}

void slave_io_init()
{
	/** reset input */
	SLAVE_RESET_INPUT();
	SLAVE_RESET_L();
	
	/* Target /ERASE line is active HIGH: there is a mosfet that inverts logic */
	SLAVE_ERASE_H();
	SLAVE_ERASE_OUTPUT();
	
#ifdef RE_DEBUG
	LED0_INPUT();
	LED0_L();
	
	LED1_H();
	LED1_OUTPUT();
#endif
	
	TCCR1A = 0;
	TCCR1B = 0;
	OCR1A = 20000;
	TIMSK1 = 0x02;
}

void slave_erase_start()
{
	TCCR1B = 0x0A;
	ResetTimer = 40;
}

void slave_reset_start()
{
	TCCR1B = 0x0A;
	ResetTimer = 10;
}

void slave_parse_cmd(u8 *buf)
{
	switch(buf[0]){
	  case MASTER_CMD_ACK:
		hal_nrf_send_ack();
		break;
	  case MASTER_CMD_TX_ALLOW:
		hal_nrf_set_sta(HAL_NRF_STA_TX);
		break;
	  case MASTER_CMD_SET_CONTROL_LINE_STATE:
		PreviousDTRState = CurrentDTRState;
		PreviousRTSState = CurrentRTSState;
		CurrentDTRState = (buf[3] & CDC_CONTROL_LINE_OUT_DTR);
		CurrentRTSState = (buf[3] & CDC_CONTROL_LINE_OUT_RTS);
		if (Selected1200BPS) {
//			putstring("erase\r\n");
			/* Start Erase / Reset procedure when receiving the magic "1200" baudrate */
			slave_erase_start();
		} else if (!PreviousDTRState && CurrentDTRState) {
//			putstring("reset\r\n");
			/* Reset on rising edge of DTR */
			slave_reset_start();
			com_reconfig(slave_line_coding.dwDTERate);
			hal_nrf_clear_tx();
		}
		
		break;
	  case MASTER_CMD_SET_LINE_CODING:
//		putstring("line coding:");
//		puthex_buf(buf, CMD_LENGTH);
//		putstring("\r\n");
		
		slave_line_coding.dwDTERate = buf[4];
		slave_line_coding.dwDTERate <<= 8;
		slave_line_coding.dwDTERate += buf[3];
		slave_line_coding.dwDTERate <<= 8;
		slave_line_coding.dwDTERate += buf[2];
		slave_line_coding.dwDTERate <<= 8;
		slave_line_coding.dwDTERate += buf[1];
		slave_line_coding.bCharFormat = buf[5];
		slave_line_coding.bParityType = buf[6];
		slave_line_coding.bDataBits = buf[7];
		
		Selected1200BPS=(slave_line_coding.dwDTERate == 1200);
		com_reconfig(slave_line_coding.dwDTERate);
		
		/** reset all led */
		LED_D4_H();
		LED_D5_H();
		LED_D6_H();
		LED_D7_H();
		LED_D8_H();
		LED_A0_H();
		LED_A1_H();
		LED_A4_H();
		LED_A5_H();
		
		break;
	  case MASTER_CMD_GET_LINE_CODING:
		break;
	}
}


#pragma vector = TIMER1_COMPA_vect
__interrupt void t1_irq(void)
{
	if(ResetTimer>0){
		if (ResetTimer == 40) {
			setErasePin(true);		// low, enable erase
			setResetPin(false);		// input
		}
		if (ResetTimer == 15) {
			setErasePin(false);		// high, erase done
			setResetPin(false);		// input
		}
		if (ResetTimer == 10) {	
			setErasePin(false);		// high
			setResetPin(true);		// enabel reset
		}
		if (ResetTimer == 1) {
			setErasePin(false);		// high
			setResetPin(false);		// input
		}
		ResetTimer--;
	}else{
		setErasePin(false);		// high
		setResetPin(false);		// input
		TCCR1B=0;
		TCNT1 = 0;
	}
}