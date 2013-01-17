/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic 
 * Semiconductor ASA.Terms and conditions of usage are described in detail 
 * in NORDIC SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT. 
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRENTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *              
 * $LastChangedRevision: 133 $
 */

/** @file
 * This file contain structures and constants defined in Chapter 9 of the USB 2.0 standard
 *
 *
 */
#ifndef USB_DESC_H__
#define USB_DESC_H__

#include "stdint.h"

// Standard request codes
#define USB_REQ_GET_STATUS         0x00
#define USB_REQ_CLEAR_FEATURE      0x01
#define USB_REQ_RESERVED_1         0x02
#define USB_REQ_SET_FEATURE        0x03
#define USB_REQ_RESERVED_2         0x04
#define USB_REQ_SET_ADDRESS        0x05
#define USB_REQ_GET_DESCRIPTOR     0x06
#define USB_REQ_SET_DESCRIPTOR     0x07
#define USB_REQ_GET_CONFIGURATION  0x08
#define USB_REQ_SET_CONFIGURATION  0x09
#define USB_REQ_GET_INTERFACE      0x0a
#define USB_REQ_SET_INTERFACE      0x0b
#define USB_REQ_SYNCH_FRAME        0x0c

// Descriptor types
#define USB_DESC_DEVICE           0x01
#define USB_DESC_CONFIGURATION    0x02
#define USB_DESC_STRING           0x03
#define USB_DESC_INTERFACE        0x04
#define USB_DESC_ENDPOINT         0x05
#define USB_DESC_DEVICE_QUAL      0x06
#define USB_DESC_OTHER_SPEED_CONF 0x07
#define USB_DESC_INTERFACE_POWER  0x08

// CDC class request code
#define GET_LINE_CODING         0x21
#define SERIAL_STATE            0x20
#define SET_LINE_CODING         0x20
#define SET_CONTROL_LINE_STATE  0x22
#define SEND_BREAK              0x23

#define USB_ENDPOINT_TYPE_BULK    0x02

typedef struct
{
     volatile uint8_t bLength;
     volatile uint8_t bDescriptorType;
     volatile uint16_t bcdUSB;
     volatile uint8_t bDeviceClass;
     volatile uint8_t bDeviceSubClass;
     volatile uint8_t bDeviceProtocol;
     volatile uint8_t bMaxPacketSize0;
     volatile uint16_t idVendor;
     volatile uint16_t idProduct;
     volatile uint16_t bcdDevice;
     volatile uint8_t iManufacturer;
     volatile uint8_t iProduct;
     volatile uint8_t iSerialNumber;
     volatile uint8_t bNumConfigurations;
} usb_dev_desc_t;

typedef struct
{
     volatile uint8_t bLength;
     volatile uint8_t bDescriptorType;
     volatile uint16_t wTotalLength;
     volatile uint8_t bNumInterfaces;
     volatile uint8_t bConfigurationValue;
     volatile uint8_t iConfiguration;
     volatile uint8_t bmAttributes;
     volatile uint8_t bMaxPower;
} usb_conf_desc_t;

typedef struct
{
     volatile uint8_t bLength;
     volatile uint8_t bDescriptorType;
     volatile uint8_t bInterfaceNumber;
     volatile uint8_t bAlternateSetting;
     volatile uint8_t bNumEndpoints;
     volatile uint8_t bInterfaceClass;
     volatile uint8_t bInterfaceSubClass;
     volatile uint8_t bInterfaceProtocol;
     volatile uint8_t iInterface;
} usb_if_desc_t;

typedef struct
{
     volatile uint8_t bLength;
     volatile uint8_t bDescriptorType;
     volatile uint8_t bEndpointAddress;
     volatile uint8_t bmAttributes;
     volatile uint16_t wMaxPacketSize;
     volatile uint8_t bInterval;
} usb_ep_desc_t;

typedef struct{
	u32 dwDTERate;
	u8 bCharFormat;
	u8 bParityType;
	u8 bDataBits;
}usb_cdc_line_coding_t;

#endif // USB_DESC_H__
