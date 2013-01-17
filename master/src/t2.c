#include "t2.h"

u8 t2_sta;

void t2_init(void)
{
	CRC = (65536-16*TIME/12);
	T2 = CRC;
	T2CON = 0x10;
	ET2 = 1;
}

void t2_start_delay()
{
	t2_sta = 1;
	T2 = (65536-16*TIME_DELAY/12);
	T2_START();
}

void t2_start()
{
	t2_sta = 2;
	
}

T2_ISR()
{
	T2_STOP();
	TF2 = 0;
	if(t2_sta==1){
		CE_HIGH();
		t2_sta = 2;
		T2 = (65536-16*TIME/12);
		T2_START();
	}else if(t2_sta == 2){
		CE_LOW();
		T2_STOP();
	}
}