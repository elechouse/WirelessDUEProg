#ifndef __PRINT_H
#define __PRINT_H

#include "hardware.h"
#include "rs485.h"

void putstring(u8 *str);
void puthex(u8 c);
void puthex_buf(u8 *buf, u8 len);
void putbuf(u8 *buf, u8 len);
#endif