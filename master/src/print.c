#include "print.h"

// Repeated putchar to print a string
void putstring(char *s)
{
  while(*s != 0)
    putchar(*s++);
}
code u8 hextab[16]="0123456789ABCDEF";
void puthex(u8 c)
{
	putchar(hextab[c>>4]);
	putchar(hextab[c&0x0F]);
}
void putbuf(u8 *buf, u8 len)
{
	u8 i;
	for(i=0; i<len; i++){
		putchar(buf[i]);
	}
}

void puthex_buf(u8 *buf, u8 len)
{
	u8 i;
	for(i=0; i<len; i++){
		puthex(buf[i]);
		putchar(' ');
	}
}

void put_dev_req(usb_device_request_t *req)
{
	puthex(req->bmRequestType);
	putchar(' ');
	puthex(req->bRequest);
	putchar(' ');
	puthex((u8)req->wValue);
	putchar(' ');
	puthex((u8)(req->wValue>>8));
	putchar(' ');
	puthex((u8)req->wIndex);
	putchar(' ');
	puthex((u8)(req->wIndex>>8));
	putchar(' ');
	puthex((u8)req->wLength);
	putchar(' ');
	puthex((u8)(req->wLength>>8));
	putstring("\r\n");
}
