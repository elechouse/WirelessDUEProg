#include "sys_tick.h"

#define SYS_TICK_NUM      8
/**
	
*/
u8 sys_tick_ctl=0;
u8 sys_tick_use_sta=0;
u16 sys_tick[SYS_TICK_NUM]={0};

/**
	@brief timer0 initialize function. timer base 100us.
*/
void sys_tick_init(void)
{
	/** set mode and prescaler*/
	TCCR0A = 0x02;
	TCCR0B = 0x03;
	/** 16MHz count 500us */
	OCR0A = 125;
	/** enable interrupt */
	TIMSK0 = 0x02;
}

/**
	@brief system tick flag set ON/OFF
	@param num: specifies the tick flag number to set
		@arg 0~8
	@param status: new status for the specified tick flag
		parameter value: ON, OFF. 
	@retval 0-successful, 1-failed
*/
u8 sys_tick_set(u8 num, sys_tick_sta_type status)
{
	if(num>SYS_TICK_NUM){
		return 1;
	}
	if(status == ON){
		sys_tick_ctl |= (0x80>>num);
	}else{
		sys_tick_ctl &= ~(0x80>>num);
		sys_tick[num]=0;
	}
	return 0;
}

/**
	@brief clear specified system tick flag
	@para num: specifies the tick flag number to set
		@arg 0~8
	@retval 0-successful, 1-failed
*/
u8 sys_tick_clear(u8 num)
{
	if(num>SYS_TICK_NUM){
		return 1;
	}
	sys_tick[num]=0;
	return 0;
}

/**
	@brief get specified system tick flag value
	@para num: specifies the tick flag number to set
		@arg 0~8
	@retval system tick flag value
*/
u16 sys_tick_get(u8 num)
{
	if(num>SYS_TICK_NUM){
		return 0;
	}
	return sys_tick[num];
}

/**
	@brief apply system tick flag
	@retval applied system tick flag number
*/
u8 sys_tick_apply(void)
{
	u8 i;
	for(i=0;i<SYS_TICK_NUM;i++){
		if( (sys_tick_use_sta&(0x80>>i)) == 0){
			sys_tick_use_sta |= (0x80>>i);
			return i;
		}
	}
	/** no idle tick */
	return 0xFF;
}

/**
	@brief avr timer0 compare A interrupt. when system clock is 8MHz
		interrupt cycle 100us.
*/
#pragma vector=TIMER0_COMPA_vect
__interrupt void sys_tick_irq(void)
{
	u8 i;
	/** check every sys_tick flag */
	for(i=0;i<SYS_TICK_NUM;i++){
		/** see if sys tick is open */
		if(sys_tick_ctl&(0x80>>i)){
			/** opened sys_tick flag plus 1 */
			sys_tick[i]++;
		}
	}
}