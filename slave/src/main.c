#include "hardware.h"
#include "sys_tick.h"
#include "rs485.h"
#include "hal_nrf.h"
#include "print.h"
#include "nrf_app.h"

#ifdef SLAVE_DEBUG
u8 buf[32];
u8 ack=33;
#else
u8 buf[30];
#endif

void main()
{   
	slave_io_init();
	sys_tick_init();
	com_init(115200);
	hal_nrf_init();
	__enable_interrupt();
	
#ifdef SLAVE_DEBUG
	putstring("SLAVE Initial...\r\n");
	{
		u8 i;
		for(i=0; i<0x17; i++){
			buf[i]=hal_nrf_read_reg(i);
		}
		puthex_buf(buf, 0x17);
		puthex(hal_nrf_read_reg(0x1C));
		putstring(" ");
		puthex(hal_nrf_read_reg(0x1D));
		putstring("\r\n");
		
	}
#endif
	
	nrf_app_init();
	__delay_cycles(F_CPU/1000000*4500);
	hal_nrf_set_operation_mode(HAL_NRF_PRX);
	CE_HIGH();

	LED_D4_OUTPUT();
	LED_D5_OUTPUT();
	LED_D6_OUTPUT();
	LED_D7_OUTPUT();
	LED_D8_OUTPUT();
	LED_A0_OUTPUT();
	LED_A1_OUTPUT();
	LED_A4_OUTPUT();
	LED_A5_OUTPUT();
	
	LED_D4_H();
	LED_D5_H();
	LED_D6_H();
	LED_D7_H();
	LED_D8_H();
	LED_A0_H();
	LED_A1_H();
	LED_A4_H();
	LED_A5_H();
    
//	LED0_OUTPUT();
//	LED1_OUTPUT();
//	LED0_H();
//	LED1_H();
//	
//	SLAVE_RESET_OUTPUT();
//	SLAVE_RESET_L();
	
	while(1){
#ifdef SLAVE_DEBUG
		if(hal_nrf_irq_flag){
			putstring("nRF IRQ:");
			puthex(hal_nrf_irq_flag);
			putstring("\r\n");
			hal_nrf_irq_flag=0;
		}
#endif
		com_sta_handle();
		hal_nrf_sta_handle();
	}
}