#ifndef __T2_H
#define __T2_H
#include "hardware.h"
#include "hal_nrf.h"

#define TIME				(50)
#define TIME_DELAY			(200)

#define T2_STOP()			T2CON = 0x10;
#define T2_START()			T2CON = 0x11;

void t2_init(void);
void t2_start_delay();

#endif