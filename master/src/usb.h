#ifndef __USB_H
#define __USB_H

#include "hardware.h"
#include "usb_desc.h"
#include "descriptors.h"
#include "print.h"
#include "hal_nrf.h"
#include "string.h"

#define MAX_PACKET_SIZE_EP0 32
#define USB_EP1_SIZE        64
#define USB_EP_DEFAULT_BUF_SIZE 64

#define USB_EP0_HSNAK() do {ep0cs = 0x02; } while(0)
#define USB_EP0_STALL() do {ep0cs = 0x11; } while(0) // Set both DSTALL and STALL when we want to stall a request during a SETUP transaction
#define USB_EP0_ACK() do {out0bc = 0xFF; } while(0)
/**
sudav		usbirq(0)		0x00
sof			usbirq(1)		0x04
sutok		usbirq(2)		0x08
suspend		usbirq(3)		0x0C
usbreset	usbirq(4)		0x10
ep0in		In_irq(0)		0x18
ep0out		out07irq(0)		0x1C
ep1in		In_irq(1)		0x20
ep1out		out07irq(1)		0x24
ep2in		In_irq(2)		0x28
ep2out		out07irq(2)		0x2C
ep3in		In_irq(3)		0x30
ep3out		out07irq(3)		0x34
ep4in		In_irq(4)		0x38
ep4out		out07irq(4)		0x3C
ep5in		In_irq(5)		0x40
ep5out		out07irq(5)		0x44
*/
#define INT_SUDAV    	0x00
#define INT_SOF      	0x04
#define INT_SUTOK    	0x08
#define INT_SUSPEND  	0x0C
#define INT_USBRESET 	0x10
#define INT_EP0IN    	0x18
#define INT_EP0OUT   	0x1C
#define INT_EP1IN    	0x20
#define INT_EP1OUT   	0x24
#define INT_EP2IN		0x28
#define INT_EP2OUT		0x2C
#define INT_EP3IN		0x30
#define INT_EP3OUT		0x34
#define INT_EP4IN		0x38
#define INT_EP4OUT		0x3C
#define INT_EP5IN		0x40
#define INT_EP5OUT		0x44

#define USB_BM_STATE_CONFIGURED           0x01
#define USB_BM_STATE_ALLOW_REMOTE_WAKEUP  0x02

#define USB_USE_INTERRUPT
//#define USB_DEBUG
//#define USB_CDC_DEBUG

/** An enum describing the USB state
 * 
 *  The states described in this enum are found in Chapter 9 of the USB 2.0 specification
 */

typedef enum 
{
    ATTACHED,
    POWERED,
    DEFAULT,
    ADDRESSED,
    CONFIGURED,
    SUSPENDED
} usb_state_t;

typedef struct{
	u8 bmRequestType;
	u8 bRequest;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
}usb_device_request_t;

void usb_init(void);
void usb_irq(void);
void su_handle(void);
void usb_ep0_send(void);
void usb_ep0_receive(void);
void usb_ep2_send(void);
void usb_ep2_receive(void);
void usb_ep2_write_buf(u8 *buf, u8 len);
void usb_sta_handle(void);

extern void put_dev_req(usb_device_request_t *req);

// #ifdef USB_DEBUG
// extern void putstring(s8 *);
// extern void puthex(u8 c);
// extern void putbuf(u8 *buf, u8 len);
// #endif

#endif