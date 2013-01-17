#include "hardware.h"
#include "usb.h"
#include "hal_uart.h"
#include "nrf_app.h"
#include "print.h"

#define LED 			P05
#define LED_ON()		LED=0
#define LED_OFF()		LED=1

#define LED_TEST P03
#define LED_TEST_ON()	LED_TEST = 0
#define LED_TEST_OFF()	LED_TEST = 1
void main()
{
	/** test code */
	P0EXP &= ~0x30;
	P0ALT &= ~0x10;
	P0DIR &= ~0x10;
	P04 = 1;
	
	/** LED config */
	P0EXP &= ~0xC0;
	P0ALT &= ~0x20;
	P0DIR &= ~0x20;
	P05 = 1;
	
	/** LED config for black board P03*/
	P0ALT &= ~0x08;
	P0DIR &= ~0x08;
	P03 = 1;
	
	delay_init();
	usb_init();
	hal_uart_init(UART_BAUD_57K6);
	hal_nrf_init();
	IP0 = 0x10;
	IP1 = 0x11;
	EA = 1;
#ifdef MASTER_DEBUG
	putstring("Master initial...\r\n");
#endif
	
	/** nRF register initial */
	nrf_app_init();
	delay_ms(5);
	// Configure radio as primary receiver (PRX)
	hal_nrf_set_operation_mode(HAL_NRF_PRX);
	// Enable receiver
	CE_HIGH();
	delay(300);	
	while(1){
#ifndef USB_USE_INTERRUPT
		usb_irq();
#endif
#ifdef MASTER_DEBUG
// 		if(hal_nrf_irq_flag){
// 			putstring("nRF IRQ:");
// 			puthex(hal_nrf_irq_flag);
// 			putstring("\r\n");
// 			hal_nrf_irq_flag = 0;
// 		}
#endif	
		usb_sta_handle();
		hal_nrf_sta_handle();
	}
}
