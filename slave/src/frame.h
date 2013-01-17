#ifndef __FRAME_H
#define __FRAME_H

#include "hardware.h"
#include "frame_map.h"
#include "bcc.h"
#include "rs485.h"

typedef struct{
	u8 soh;
	u8 type;
	u8 id;
	u8 fc;
	u8 len;
	
}frame_struct_type;

void frame_analyse(void);

#endif