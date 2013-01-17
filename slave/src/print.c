
#include "print.h"

void putstring(u8 *str)
{
	while(*str){
		com_putchar(*str++);
	}
}

__flash u8 hextab[17]="0123456789ABCDEF";
void puthex(u8 c)
{
	com_putchar(hextab[(c>>4)&0x0F]);
	com_putchar(hextab[(c&0x0F)]);
}

void puthex_buf(u8 *buf, u8 len)
{
	u8 i;
	for(i=0; i<len; i++){
		puthex(buf[i]);
		com_putchar(' ');
	}
}

void putbuf(u8 *buf, u8 len)
{
	u8 i;
	for(i=0; i<len; i++){
		com_putchar(buf[i]);
	}
}