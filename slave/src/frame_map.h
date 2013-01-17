#ifndef __FRAME_MAP_H
#define __FRAME_MAP_H

#define FRAME_HEAD						(0x09)
#define FRAME_TYPE						(0x48)
#define FRAME_EOT						(0x0d)

#define FRAME_FC_GET_CARDORKEY			('F')
#define FRAME_FC_SET_READERNUM			('C')
#define FRAME_FC_QUERY_READERNUM		('D')
#define FRAME_FC_SET_READERSERIAL		('S')
#define FRAME_FC_SET_READERTIME			(0x87)
#define FRAME_FC_SHOW_MEG_LCD			(0x88)



#define FRAME_BROADCAST_ADDR1			(0x00)
#define FRAME_BROADCAST_ADDR2			(0xFF)

/** frame head and control area */
#define FRAME_SEAT_HEAD					(0)
#define FRAME_SEAT_TYPE					(1)
#define FRAME_SEAT_ADDR					(2)
#define FRAME_SEAT_FC					(3)
#define FRAME_SEAT_DATA_LEN				(4)

#endif