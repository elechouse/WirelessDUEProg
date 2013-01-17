
#include "usb.h"

#define _AT_ _at_

#define MIN(a, b) ((a) < (b) ? (a) : (b))
// #define MAX(a, b) ((a) > (b) ? (a) : (b))

// USB map:
xdata volatile uint8_t out5buf[USB_EP_DEFAULT_BUF_SIZE] _AT_ 0xC440;
xdata volatile uint8_t in5buf[USB_EP_DEFAULT_BUF_SIZE]  _AT_ 0xC480;
xdata volatile uint8_t out4buf[USB_EP_DEFAULT_BUF_SIZE] _AT_ 0xC4C0;
xdata volatile uint8_t in4buf[USB_EP_DEFAULT_BUF_SIZE]  _AT_ 0xC500;
xdata volatile uint8_t out3buf[USB_EP_DEFAULT_BUF_SIZE] _AT_ 0xC540;
xdata volatile uint8_t in3buf[USB_EP_DEFAULT_BUF_SIZE]  _AT_ 0xC580;
xdata volatile uint8_t out2buf[USB_EP_DEFAULT_BUF_SIZE] _AT_ 0xC5C0;
xdata volatile uint8_t in2buf[USB_EP_DEFAULT_BUF_SIZE]  _AT_ 0xC600;
xdata volatile uint8_t out1buf[USB_EP_DEFAULT_BUF_SIZE] _AT_ 0xC640;
xdata volatile uint8_t in1buf[USB_EP_DEFAULT_BUF_SIZE]  _AT_ 0xC680;
xdata volatile uint8_t out0buf[USB_EP_DEFAULT_BUF_SIZE] _AT_ 0xC6C0;
xdata volatile uint8_t in0buf[USB_EP_DEFAULT_BUF_SIZE]  _AT_ 0xC700;
xdata volatile uint8_t out8data                         _AT_ 0xC760;
xdata volatile uint8_t in8data                          _AT_ 0xC768;
xdata volatile uint8_t out8bch                          _AT_ 0xC770;
xdata volatile uint8_t out8bcl                          _AT_ 0xC771;
xdata volatile uint8_t bout1addr                        _AT_ 0xC781;
xdata volatile uint8_t bout2addr                        _AT_ 0xC782;
xdata volatile uint8_t bout3addr                        _AT_ 0xC783;
xdata volatile uint8_t bout4addr                        _AT_ 0xC784;
xdata volatile uint8_t bout5addr                        _AT_ 0xC785;
xdata volatile uint8_t binstaddr                        _AT_ 0xC788;
xdata volatile uint8_t bin1addr                         _AT_ 0xC789;
xdata volatile uint8_t bin2addr                         _AT_ 0xC78A;
xdata volatile uint8_t bin3addr                         _AT_ 0xC78B;
xdata volatile uint8_t bin4addr                         _AT_ 0xC78C;
xdata volatile uint8_t bin5addr                         _AT_ 0xC78D;
xdata volatile uint8_t isoerr                           _AT_ 0xC7A0;
xdata volatile uint8_t zbcout                           _AT_ 0xC7A2;
xdata volatile uint8_t ivec                             _AT_ 0xC7A8;
xdata volatile uint8_t in_irq                           _AT_ 0xC7A9;
xdata volatile uint8_t out_irq                          _AT_ 0xC7AA;
xdata volatile uint8_t usbirq                           _AT_ 0xC7AB;
xdata volatile uint8_t in_ien                           _AT_ 0xC7AC;
xdata volatile uint8_t out_ien                          _AT_ 0xC7AD;
xdata volatile uint8_t usbien                           _AT_ 0xC7AE;
xdata volatile uint8_t usbbav                           _AT_ 0xC7AF;
xdata volatile uint8_t ep0cs                            _AT_ 0xC7B4;
xdata volatile uint8_t in0bc                            _AT_ 0xC7B5;
xdata volatile uint8_t in1cs                            _AT_ 0xC7B6;
xdata volatile uint8_t in1bc                            _AT_ 0xC7B7;
xdata volatile uint8_t in2cs                            _AT_ 0xC7B8;
xdata volatile uint8_t in2bc                            _AT_ 0xC7B9;
xdata volatile uint8_t in3cs                            _AT_ 0xC7BA;
xdata volatile uint8_t in3bc                            _AT_ 0xC7BB;
xdata volatile uint8_t in4cs                            _AT_ 0xC7BC;
xdata volatile uint8_t in4bc                            _AT_ 0xC7BD;
xdata volatile uint8_t in5cs                            _AT_ 0xC7BE;
xdata volatile uint8_t in5bc                            _AT_ 0xC7BF;
xdata volatile uint8_t out0bc                           _AT_ 0xC7C5;
xdata volatile uint8_t out1cs                           _AT_ 0xC7C6;
xdata volatile uint8_t out1bc                           _AT_ 0xC7C7;
xdata volatile uint8_t out2cs                           _AT_ 0xC7C8;
xdata volatile uint8_t out2bc                           _AT_ 0xC7C9;
xdata volatile uint8_t out3cs                           _AT_ 0xC7CA;
xdata volatile uint8_t out3bc                           _AT_ 0xC7CB;
xdata volatile uint8_t out4cs                           _AT_ 0xC7CC;
xdata volatile uint8_t out4bc                           _AT_ 0xC7CD;
xdata volatile uint8_t out5cs                           _AT_ 0xC7CE;
xdata volatile uint8_t out5bc                           _AT_ 0xC7CF;
xdata volatile uint8_t usbcs                            _AT_ 0xC7D6;
xdata volatile uint8_t togctl                           _AT_ 0xC7D7;
xdata volatile uint8_t usbfrml                          _AT_ 0xC7D8;
xdata volatile uint8_t usbfrmh                          _AT_ 0xC7D9;
xdata volatile uint8_t fnaddr                           _AT_ 0xC7DB;
xdata volatile uint8_t usbpair                          _AT_ 0xC7DD;
xdata volatile uint8_t inbulkval                        _AT_ 0xC7DE;
xdata volatile uint8_t outbulkval                       _AT_ 0xC7DF;
xdata volatile uint8_t inisoval                         _AT_ 0xC7E0;
xdata volatile uint8_t outisoval                        _AT_ 0xC7E1;
xdata volatile uint8_t isostaddr                        _AT_ 0xC7E2;
xdata volatile uint8_t isosize                          _AT_ 0xC7E3;
xdata volatile uint8_t setupbuf[8]                      _AT_ 0xC7E8;
xdata volatile uint8_t out8addr                         _AT_ 0xC7F0;
xdata volatile uint8_t in8addr                          _AT_ 0xC7F8;

u8* usb_buf;
u8 usb_buf_size;
usb_state_t usb_state;
u8 usb_current_config;
usb_device_request_t usb_dev_req;
usb_device_request_t usb_dev_req_bak;
u8 ep2_irq_flag=0;
u16 usb_delay_count;
u16 ep2_irq_delay_count;
usb_cdc_line_coding_t usb_cdc_line_coding={
	19200,
	0,
	0,
	8
};
u8 usb_tmp[64];
u8 usb_cdc_irq_flag=0;

static void delay_ms(uint16_t ms)
{
    uint16_t i, j;
    
    for(i = 0; i < ms; i++ )
    {
        for( j = 0; j < 1403; j++)
        {
            _nop_();
        }
    }
}


void usb_init(void)
{
	usb_current_config = 0;
	usb_state = DEFAULT;
	ep2_irq_flag=0;
	
	// Disconnect from USB-bus since we are in this routine from a power on and not a soft reset:
    usbcs |= 0x08;
    delay_ms(50);
    usbcs &= ~0x08;
#ifdef USB_USE_INTERRUPT
	USB = 1;
#endif
	usbien = 0x1d;
    in_ien = 0x01;
    in_irq = 0x1f;
    out_ien = 0x01;
    out_irq = 0x1f;

    // Setup the USB RAM with some OK default values:
    bout1addr = MAX_PACKET_SIZE_EP0/2;
    bout2addr = MAX_PACKET_SIZE_EP0/2 + USB_EP1_SIZE/2;
    bout3addr = MAX_PACKET_SIZE_EP0/2 + 2*USB_EP1_SIZE/2;
    bout4addr = MAX_PACKET_SIZE_EP0/2 + 3*USB_EP1_SIZE/2;
    bout5addr = MAX_PACKET_SIZE_EP0/2 + 4*USB_EP1_SIZE/2;
    binstaddr = 0xc0;
    bin1addr = MAX_PACKET_SIZE_EP0/2;
    bin2addr = MAX_PACKET_SIZE_EP0/2 + USB_EP1_SIZE/2;
    bin3addr = MAX_PACKET_SIZE_EP0/2 + 2*USB_EP1_SIZE/2;
    bin4addr = MAX_PACKET_SIZE_EP0/2 + 3*USB_EP1_SIZE/2;
    bin5addr = MAX_PACKET_SIZE_EP0/2 + 4*USB_EP1_SIZE/2;

    // Set all endpoints to not valid (except EP0IN and EP0OUT)
    inbulkval = 0x01;
    outbulkval = 0x01;
    inisoval = 0x00;
    outisoval = 0x00;

	/** enable endp1 */
    in_ien |= 0x02;; 
    inbulkval |= 0x02;
    out_ien |= 0x02;
    outbulkval |= 0x02;
    out1bc = 0xff;
	
	/** enable endp2 */
    in_ien |= 0x04;; 
    inbulkval |= 0x04;
    out_ien |= 0x04;
    outbulkval |= 0x04;
    out2bc = 0xff;
}

void usb_ep2_write_buf(u8 *buf, u8 len)
{
	u8 i;
	u16	cnt;
	cnt = 0;
	while(in2cs&0x02){
		cnt++;
		if(cnt==500){
			return ;
		}
	}
	for(i=0; i<len; i++){
		in2buf[i] = buf[i];
	}
	in2bc = len;
}

void usb_ep2_send(void)
{
	in2cs = 0x02;
//	in2bc =0;
}

void usb_ep2_receive(void)
{
	ep2_irq_flag |= 0x01;
#ifdef USB_DEBUG
	putstring("Receive data:");
	puthex_buf(out2buf, len);
	putstring("LEN:");
	puthex(len);
	putstring("\r\n");
	putstring("out2cs:");
	puthex(out2cs);
	putstring("\r\n");
	usb_ep2_write_buf(out2buf, len);
#endif
	
#ifdef USB_DEBUG
	putstring("out2cs:");
	puthex(out2cs);
	putstring("\r\n");
#endif
}

void usb_ep0_send(void)
{
	u8 len,i;
	if(usb_buf_size == 0){
		in0bc = 0;
        USB_EP0_HSNAK();
        return;
	}
	len = MIN(usb_buf_size, MAX_PACKET_SIZE_EP0);
#ifdef USB_DEBUG
	putstring("EP0 Send:");
#endif
	for(i=0; i<len; i++){
		in0buf[i] = usb_buf[i];
#ifdef USB_DEBUG
		puthex(usb_buf[i]);
		putstring(" ");
#endif
	}
#ifdef USB_DEBUG
	putstring("\r\n");
#endif	
	in0bc = len;
	
	usb_buf_size -= len;
	usb_buf += len;
}

void usb_ep0_receive(void)
{
	if(usb_dev_req.bmRequestType==0x21 && usb_dev_req.bRequest == SET_LINE_CODING){
		usb_cdc_irq_flag |= 0x02;
	}else{
		usb_buf_size = 0;
		USB_EP0_HSNAK();
	}
}

void usb_cdc_request_handle(void)
{
	if(usb_dev_req.bmRequestType & 0x80){
		/** class in */
		switch(usb_dev_req.bRequest){
			case GET_LINE_CODING:
				usb_tmp[0] = (u8)usb_cdc_line_coding.dwDTERate;
				usb_tmp[1] = (u8)(usb_cdc_line_coding.dwDTERate>>8);
				usb_tmp[2] = (u8)(usb_cdc_line_coding.dwDTERate>>16);
				usb_tmp[3] = (u8)(usb_cdc_line_coding.dwDTERate>>24);
				usb_tmp[4] = usb_cdc_line_coding.bCharFormat;
				usb_tmp[5] = usb_cdc_line_coding.bParityType;
				usb_tmp[6] = usb_cdc_line_coding.bDataBits;
				usb_buf_size = MIN(usb_dev_req.wLength, 7);
				usb_buf=usb_tmp;
				usb_ep0_send();
#ifdef USB_CDC_DEBUG
				putstring("USB GET_LINE_CODING\r\n");
				put_dev_req(&usb_dev_req);
#endif
				break;
			case SERIAL_STATE:
				usb_buf_size = 0;
				usb_buf=NULL;
				usb_ep0_send();
#ifdef USB_CDC_DEBUG
				putstring("USB SERIAL_STATE\r\n");
				put_dev_req(&usb_dev_req);
#endif
				break;
			default:
				usb_buf_size = 0;
				usb_buf=NULL;
				usb_ep0_send();
#ifdef USB_CDC_DEBUG
				putstring("UNKNOWN CLASS IN REQUEST\r\n");
				put_dev_req(&usb_dev_req);
#endif
				break;
		}
	}else{
		/** class out */
		switch(usb_dev_req.bRequest){
			case SET_CONTROL_LINE_STATE:
				master_cmd[0] = MASTER_CMD_SET_CONTROL_LINE_STATE;
				memcpy(master_cmd+1, setupbuf, 8);
				master_cmd[9] = CMD_FILL;
				hal_nrf_send_cmd(master_cmd, HAL_NRF_STA_RX_IDLE);
#ifdef USB_CDC_DEBUG
				putstring("USB SET_CONTROL_LINE_STATE..\r\n");
				put_dev_req(&usb_dev_req);
				puthex_buf(master_cmd, CMD_LENGTH);
				putstring("\r\n");
#endif			
				
#if 1
				usb_cdc_irq_flag |= 0x08;
				usb_delay_count=0;
// 				usb_buf_size = 0;
// 				usb_buf=NULL;
// 				usb_ep0_send();
#elif 0
				out0bc = 0xff;
				USB_EP0_HSNAK();	
#else
				//USB_EP0_STALL();
				out0bc = 0xff;			
#endif


				break;
			case SET_LINE_CODING:
				/** set serial attributes, don't send data */
				USB_EP0_ACK();
#ifdef USB_CDC_DEBUG
				putstring("USB SET_LINE_CODING\r\n");
				put_dev_req(&usb_dev_req);
#endif
				break;
			default:
#ifdef USB_CDC_DEBUG
				putstring("UNKNOWN CLASS OUT REQUEST\r\n");
				put_dev_req(&usb_dev_req);
#endif
				usb_buf_size = 0;
				usb_buf=NULL;
				usb_ep0_send();
				break;
		}
	}
}

void usb_process_get_descriptor()
{
	switch((u8)(usb_dev_req.wValue>>8)){
		case USB_DESC_DEVICE:
			usb_buf = DeviceDescriptor;
			usb_buf_size = MIN(usb_dev_req.wLength,sizeof(DeviceDescriptor));
#ifdef USB_DEBUG
			putstring("Device Descriptor:");
			puthex_buf(usb_buf, usb_buf[0]);
			putstring("Length:");
			puthex(usb_buf_size);
			putstring("\r\n");
#endif
            usb_ep0_send();
			break;
		case USB_DESC_CONFIGURATION:
			usb_buf = ConfigurationDescriptor;
			usb_buf_size = MIN(usb_dev_req.wLength,sizeof(ConfigurationDescriptor));
#ifdef USB_DEBUG
			putstring("Configuration Descriptor:");
			puthex_buf(usb_buf, usb_buf_size);
			putstring("Length:");
			puthex(usb_buf_size);
			putstring("\r\n");
#endif
			usb_ep0_send();
			break;
		case USB_DESC_STRING:
#ifdef USB_DEBUG
			putstring("String Descriptor:");
#endif
			switch((u8)usb_dev_req.wValue){
				case 0:
					/** language ID */
					usb_buf = LanguageString;
					usb_buf_size = MIN(usb_dev_req.wLength, sizeof(LanguageString));
					usb_ep0_send();
					break;
				case 1:
					usb_buf = ManufacturerString;
					usb_buf_size = MIN(usb_dev_req.wLength, sizeof(ManufacturerString));
					usb_ep0_send();
					break;
				case 2:
					usb_buf = ProductString;
					usb_buf_size = MIN(usb_dev_req.wLength, sizeof(ProductString));
					usb_ep0_send();
					break;
				case 3:
					usb_buf = SerialNumberString;
					usb_buf_size = MIN(usb_dev_req.wLength, sizeof(SerialNumberString));
					usb_ep0_send();
					break;
				default:
					USB_EP0_STALL();
					break;
			}
			break;
		case USB_DESC_INTERFACE:
        case USB_DESC_ENDPOINT:
        case USB_DESC_DEVICE_QUAL:
        case USB_DESC_OTHER_SPEED_CONF:
        case USB_DESC_INTERFACE_POWER:
            USB_EP0_STALL();
            break;
        default:
            USB_EP0_HSNAK();
            break;
	}
}
void su_handle(void)
{
//	u8 bmRequestType;
//	bmRequestType = setupbuf[0];
	usb_dev_req.bmRequestType = setupbuf[0];
	usb_dev_req.bRequest = setupbuf[1];
	usb_dev_req.wValue = setupbuf[3];
	usb_dev_req.wValue <<=8;
	usb_dev_req.wValue |= setupbuf[2];
	usb_dev_req.wIndex = setupbuf[5];
	usb_dev_req.wIndex <<=8;
	usb_dev_req.wIndex |= setupbuf[4];
	usb_dev_req.wLength = setupbuf[7];
	usb_dev_req.wLength <<=8;
	usb_dev_req.wLength |= setupbuf[6];

	switch(usb_dev_req.bmRequestType&0x60){
		case 0x00:	// standard request
			switch(usb_dev_req.bRequest){
				case USB_REQ_GET_DESCRIPTOR:
#ifdef USB_DEBUG	
					putstring("USB Get Descriptor\r\n");
#endif
					usb_process_get_descriptor();
					break;
				case USB_REQ_GET_STATUS:
					break;
				case USB_REQ_SET_ADDRESS:
#ifdef USB_DEBUG	
					putstring("USB Set Address\r\n");
#endif
					usb_state = ADDRESSED;
					usb_current_config = 0x00;
					break;
				case USB_REQ_GET_CONFIGURATION:
#ifdef USB_DEBUG	
					putstring("USB Get Configuration\r\n");
#endif
					break;
				case USB_REQ_SET_CONFIGURATION:
#ifdef USB_DEBUG	
					putstring("USB Set Configuration\r\n");
#endif
					switch((u8)usb_dev_req.wValue)
					{
						case 0x00:
							usb_state = ADDRESSED;
							usb_current_config = 0x00;
							USB_EP0_HSNAK();
							break;
						case 0x01:
							/** enable endpoint here? */
							usb_state = CONFIGURED;
//							usb_bm_state |= USB_BM_STATE_CONFIGURED;
							usb_current_config = 0x01;
							USB_EP0_HSNAK();
							break;
						default:
							USB_EP0_STALL();
							break;
					}
					break;
				case USB_REQ_GET_INTERFACE:
					break;
				case USB_REQ_SET_DESCRIPTOR:
				case USB_REQ_SET_INTERFACE: // SET_INTERFACE
				case USB_REQ_SYNCH_FRAME:   // SYNCH_FRAME
				default:
					USB_EP0_STALL();
					break;
			}
			break;
		case 0x20:	// class request
			usb_cdc_irq_flag |= 0x01;
			break;
		case 0x40:	// manufacture request
			break;
		case 0x60:	// reserved
			break;
	}
#ifdef USB_DEBUG	
//		putstring("\r\nUSB Setup");
//		putbuf(setupbuf, 8);
//		putstring("\r\n");
#endif	
}


void usb_sta_handle(void)
{
	u8 len;
	u8 length;
	
	if(usb_cdc_irq_flag&0x01){
		usb_cdc_irq_flag &= ~0x01;
		usb_cdc_request_handle();
	}
	
	if(usb_cdc_irq_flag&0x02){
		usb_cdc_irq_flag &= ~0x02;
#ifdef USB_DEBUG
		putstring("SET_LINE_CODING Data process\r\n");
#endif
		length = out0bc;
		if(length ==7){
			usb_cdc_line_coding.dwDTERate = out0buf[3];
			usb_cdc_line_coding.dwDTERate <<= 8;
			usb_cdc_line_coding.dwDTERate += out0buf[2];
			usb_cdc_line_coding.dwDTERate <<= 8;
			usb_cdc_line_coding.dwDTERate += out0buf[1];
			usb_cdc_line_coding.dwDTERate <<= 8;
			usb_cdc_line_coding.dwDTERate += out0buf[0];
			usb_cdc_line_coding.bCharFormat = out0buf[4];
			usb_cdc_line_coding.bParityType = out0buf[5];
			usb_cdc_line_coding.bDataBits = out0buf[6];
			
			master_cmd[0] = MASTER_CMD_SET_LINE_CODING;
			memcpy(master_cmd+1, out0buf, 7);
			master_cmd[8] = CMD_FILL;
			master_cmd[9] = CMD_FILL;
			hal_nrf_send_cmd(master_cmd, HAL_NRF_STA_RX_IDLE);

#ifdef USB_CDC_DEBUG
			putstring("Set BaudRate: 0x");
			puthex(out0buf[3]);
			puthex(out0buf[2]);
			puthex(out0buf[1]);
			puthex(out0buf[0]);
			putstring("\r\n");
#endif
		}
		usb_cdc_irq_flag |= 0x04;
		usb_delay_count=0;
	}
	if(usb_cdc_irq_flag&0x04){
		usb_delay_count++;
		if(usb_delay_count == 1000){
			usb_delay_count = 0;
			usb_cdc_irq_flag &= ~0x04;
			usb_buf_size = 0;
			usb_buf=NULL;
			usb_ep0_send();
		}
	}
	
	if(usb_cdc_irq_flag&0x08){
		usb_delay_count++;
		if(usb_delay_count == 1000){
			usb_delay_count = 0;
			usb_cdc_irq_flag &= ~0x08;
			usb_buf_size = 0;
			usb_buf=NULL;
			usb_ep0_send();
		}
	}
	

	if(ep2_irq_flag & 0x01){
		ep2_irq_flag &= ~0x01;
		len = out2bc;
#ifdef MASTER_DEBUG
// 		putstring("MAX:");
// 		puthex(hal_nrf_tx_cnt_max);
// 		putstring("\r\n");
#endif
		memcpy(usb_tmp, out2buf, len);
		if(len>32){
			hal_nrf_send_pkt(usb_tmp, 32);
			hal_nrf_send_pkt(usb_tmp+32, len-32);
		}else if(len==CMD_LENGTH){
			/** user data length equal to cmd length, split to 2 packages */
			hal_nrf_send_pkt(usb_tmp, CMD_LENGTH/2);
			hal_nrf_send_pkt(usb_tmp+CMD_LENGTH/2, CMD_LENGTH-CMD_LENGTH/2);
		}else if(len){
			hal_nrf_send_pkt(usb_tmp, len);
		}
		ep2_irq_flag |= 0x02;
		ep2_irq_delay_count = 0;
	}
	if(ep2_irq_flag & 0x02){
// 		if(hal_nrf_get_tx_cnt()==0){
//  			out2bc = 0xFF;
// 			ep2_irq_flag &= ~0x02;
// 		}else{
			ep2_irq_delay_count++;
			if(ep2_irq_delay_count == 2000){
				out2bc = 0xFF;
				ep2_irq_flag &= ~0x02;
			}
//		}
	}
}

#ifdef USB_USE_INTERRUPT
USB_ISR()
#else
void usb_irq(void)
#endif
{
#ifndef USB_USE_INTERRUPT
	if(USBF){
		USBF = 0;
#endif
#ifdef USB_DEBUG
		putstring("\r\nivec=");
		puthex(ivec);
		putstring(" ");
//		putstring("\r\n");
#endif
		if(ivec == INT_USBRESET){
			usbirq = 0x10;	//clear interrupt flag
			usb_state = DEFAULT;
			usb_current_config = 0;
// 			usb_current_alt_interface = 0;
// 			usb_bm_state = 0;
#ifdef USB_DEBUG
			putstring("USB reset\r\n");
#endif
		}else{
			switch(ivec){
				case INT_SUDAV:
					/** set up package */
					usbirq = 0x01;	//clear interrupt flag
					su_handle();
#ifdef USB_DEBUG	
					putstring("USB Setup");
					puthex_buf(setupbuf, 8);
					putstring("\r\n");
#endif	
					break;
				case INT_SOF:
					usbirq = 0x02;	//clear interrupt flag
#ifdef USB_DEBUG
					putstring("USB SOF\r\n");
#endif
					break;
				case INT_SUTOK:
					usbirq = 0x04;	//clear interrupt flag
					usb_buf = NULL;
					usb_buf_size = 0;
#ifdef USB_DEBUG
					putstring("USB SUTOK\r\n");
#endif
					break;
				case INT_SUSPEND:
					usbirq = 0x08;	//clear interrupt flag
#ifdef USB_DEBUG
					putstring("USB Suspend\r\n");
#endif
					break;
				case INT_EP0IN:
					in_irq = 0x01;	//clear interrupt flag
#ifdef USB_DEBUG
					putstring("USB BUF SIZE:");
					puthex(usb_buf_size);
					putstring("\r\n");
#endif
					usb_ep0_send();
#ifdef USB_DEBUG
					putstring("USB EP0IN\r\n");
#endif
					break;
				case INT_EP0OUT:
					out_irq = 0x01;	//clear interrupt flag
#ifdef USB_DEBUG
					putstring("USB EP0OUT\r\n");
#endif
					usb_ep0_receive();
					break;
				case INT_EP1IN:
					in_irq = 0x02;	//clear interrupt flag
#ifdef USB_DEBUG
					putstring("USB EP1IN\r\n");
#endif
					break;
				case INT_EP1OUT:
					out_irq = 0x02;	//clear interrupt flag
#ifdef USB_DEBUG
					putstring("USB EP1OUT\r\n");
#endif
					break;
				case INT_EP2IN:
					in_irq = 0x04;	//clear interrupt flag
					usb_ep2_send();
#ifdef USB_DEBUG
					putstring("USB EP2IN\r\n");
#endif
					break;
				case INT_EP2OUT:
					out_irq = 0x04;	//clear interrupt flag
					usb_ep2_receive();
#ifdef USB_DEBUG
					putstring("USB EP2OUT\r\n");
#endif
					break;
				case INT_EP3IN:
					break;
				case INT_EP3OUT:
					break;
				case INT_EP4IN:
					break;
				case INT_EP4OUT:
					break;
				case INT_EP5IN:
					break;
				case INT_EP5OUT:
					break;
				default:
					
					break;
			}
		}
#ifndef USB_USE_INTERRUPT
	}	
#endif
}
