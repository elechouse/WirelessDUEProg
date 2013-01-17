#ifndef __PRINT_H
#define __PRINT_H

#include "hardware.h"
#include "hal_uart.h"
#include "usb.h"

#define putchar(c)	hal_uart_putchar(c)

void putstring(char *s);
void puthex(u8 c);
void putbuf(u8 *buf, u8 len);
void puthex_buf(u8 *buf, u8 len);

#endif