#ifndef __HARDWARE_H
#define __HARDWARE_H

#define ENABLE_BIT_DEFINITIONS

#include <iom168pa.h>
#include <intrinsics.h>
#include "type.h"
#include "compile.h"

#define ENTER_MUTEX()			__disable_interrupt()
#define EXIT_MUTEX()			__enable_interrupt()

#define F_CPU					(16000000UL)



#endif