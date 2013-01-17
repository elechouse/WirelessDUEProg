#ifndef __DESCRIPTORS_H
#define __DESCRIPTORS_H

#include "hardware.h"
extern code u8 DeviceDescriptor[0x12];
extern code u8 ConfigurationDescriptor[9+9+5+5+4+5+7+9+7+7];
extern code u8 LanguageString[4];
extern code u8 ManufacturerString[0x14];
extern code u8 ProductString[0x22];
extern code u8 SerialNumberString[0x16];
#endif