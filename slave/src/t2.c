#include "t2.h"

u8 t2_sta;

void t2_init(void)
{
/** set mode and prescaler*/
	TCCR2A = 0x02;
//	TCCR2B = 0x01;
	/** 16MHz count 15us */
	TCNT2 = 0;
	OCR2A = 240;
	/** enable interrupt */
	TIMSK2 = 0x02;
}

void t2_start_delay()
{
	t2_sta = 1;
	TCNT2 = 0;
	OCR2A = 100;
	TCCR2B = 0x03;
}

void t2_start()
{
	t2_sta = 2;
	TCNT2 = 0;
	OCR2A = 240;
	TCCR2B = 0x01;
}

#pragma vector = TIMER2_COMPA_vect
__interrupt void t2_irq(void)
{
	if(t2_sta == 1){
		t2_sta = 2;
		CE_HIGH();
		TCCR2B = 0;
		TCNT2 = 0;
		OCR2A = 240;
		TCCR2B = 0x01;
	}else if(t2_sta == 2){
		T2_STOP();
		TCNT2 = 0;
		CE_LOW();
	}
}