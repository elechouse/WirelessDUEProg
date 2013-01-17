#ifndef __T2_H
#define __T2_H
#include "hardware.h"
#include "hal_nrf.h"

#define T2_STOP()			TCCR2B = 0x00
//#define T2_START()			TCCR2B = 0x01

void t2_init(void);
void t2_start_delay();
void t2_start();

#endif