#ifndef _TYPE_H
#define _TYPE_H

typedef unsigned char u8;
typedef signed char  s8;
typedef unsigned int u16;
typedef signed int  s16;
typedef unsigned long u32;
typedef signed long  s32;
typedef unsigned long long u64;
typedef signed long long s64;

typedef union{
	u16 bit16;
	struct{
		u8 lb;
		u8 hb;
	};
}u16_union_type;


#endif