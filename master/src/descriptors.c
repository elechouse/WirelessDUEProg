#include "descriptors.h"

code u8 DeviceDescriptor[0x12]={
	0x12,	// bLength
	0x01,	// bDescriptorType
	0x10,	// bcdUSB
	0x01,
	0x02,	// class
	0x00,	// subClass
	0x00,	// protocol
	0x20, 	// FIXED_CONTROL_ENDPOINT_SIZE
	0x41,	// VID
	0x23,
//	0x43,	// PID
//	0x3D,
	0x01,
	0x00,
	0x01,	// bcdDevice, release number
	0x00,
	0x01,	// iManufacturer
	0x02,	// iProduct
	0x03,	// iSerialNumber
	0x01
};

code u8 ConfigurationDescriptor[9+9+5+5+4+5+7+9+7+7]={
	/** configuration descriptor **/
	0x09,	// bLength
	0x02,	// DTYPE_Configuration
	
	sizeof(ConfigurationDescriptor)&0xFF,	// total length
	(sizeof(ConfigurationDescriptor)>>8)&0xFF,	
	
	0x02, 	// bNumInterfaces
	0x01, 	// bConfiguration
	0x00, 	// iConfiguration
	
	0x80,	// bmAttributes
	0x32, 	// bMaxPower
	
	/** CDC_CCI_Interface */
	0x09, 	// bLength
	0x04,	// bDescriptorType
	0x00,	// InterfaceNumber
	0x00,	// AlternateSetting
	0x01,	// TotalEndpointsNumber
	0x02,	// bInterfaceClass
	0x02,	// bInterfaceSubClass
	0x01,	// bInterfaceProtocol
	0x00,	// iConfiguration
	
	/** CDC_Functional_IntHeader */
	0x05,	// bFunctionLength
	0x24,	// bDescriptorType
	0x00,	// bDescriptorSubtype
	0x10,	// bcdCDC
	0x01,	
	
	/** Call management functional descriptor */
	0x05,	// bFunctionLength
	0x24,	// bDescriptorType
	0x01,	// bDescriptorSubtype
	0x00,	// bmCapabilities
	0x00, 	// bDataInterface
	
	/** Abstract Control Management Functional Descriptor */
	0x04, 	// bFunctionLength
	0x24,	// bDescriptorType
	0x02,	// bDescriptorSubtype
	0x02,	// bmCapabilities ??? 0x06

	/** CDC_Functional_Union */
	0x05,	// bFunctionLength
	0x24,	// bDescriptorType
	0x06,	// bDescriptorSubtype
	0x00,	// bmCapabilities
	0x01, 	// bDataInterface
	
	/** CDC_CCI_Interface endpoint1 descriptor */
	0x07,
	0x05,
	0x81,	// Endp1_In
	0x03,
	0x10,
	0x00,
	0x0A,
	
	/** CDC_DCI_Interface */
	0x09, 	// bLength
	0x04,	// bDescriptorType
	0x01,	// InterfaceNumber
	0x00,	// AlternateSetting
	0x02,	// TotalEndpointsNumber
	0x0A,	// bInterfaceClass
	0x00,	// bInterfaceSubClass
	0x00,	// bInterfaceProtocol
	0x00,	// iConfiguration
	
	/** CDC_DCI_Interface endp2_in*/
	0x07,	// bLength
	0x05,	// bDescriptorType
	0x82,	// bEndpointAddress
	0x02,	// bmAttributes
	0x40,	// wMaxPacketSize
	0x00,	
	0x01,	// bInterval
	
	/** CDC_DCI_Interface endp2_out*/
	0x07,	// bLength
	0x05,	// bDescriptorType
	0x02,	// bEndpointAddress
	0x02,	// bmAttributes
	0x40,	// wMaxPacketSize
	0x00,	
	0x01,	// bInterval
};

code u8 LanguageString[4]={
	0x04,
	0x03,
	0x09,
	0x04
};

code u8 ManufacturerString[0x14]={
	0x14,
	0x03,
	'E',
	0x00,
	'l',
	0x00,
	'e',
	0x00,
	'c',
	0x00,
	'h',
	0x00,
	'o',
	0x00,
	'u',
	0x00,
	's',
	0x00,
	'e',
	0x00,
};

code u8 ProductString[0x22]={
	0x22,
	0x03,
	'W',
	0x00,
	'i',
	0x00,
	'r',
	0x00,
	'e',
	0x00,
	'l',
	0x00,
	'e',
	0x00,
	's',
	0x00,
	's',
	0x00,
	' ',
	0x00,
	'S',
	0x00,
	'e',
	0x00,
	'r',
	0x00,
	'i',
	0x00,
	'a',
	0x00,
	'l',
	0x00,
	' ',
	0x00,
};

code u8 SerialNumberString[0x16]={
	0x16,
	0x03,
	'2', 0x00,
	'0', 0x00,
	'1', 0x00,
	'2', 0x00,
	'-', 0x00,
	'1', 0x00,
	'1', 0x00,
	'-', 0x00,
	'1', 0x00,
	'2', 0x00
};