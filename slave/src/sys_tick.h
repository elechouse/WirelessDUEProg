#ifndef __SYS_TIME_H
#define __SYS_TIME_H
#include "hardware.h"

#define SYS_TICK_UNIT			500

typedef enum{
	OFF=0,
	ON=1,
}sys_tick_sta_type;

void sys_tick_init(void);
u8 sys_tick_set(u8 num, sys_tick_sta_type status);
u8 sys_tick_clear(u8 num);
u16 sys_tick_get(u8 num);
u8 sys_tick_apply(void);

#endif