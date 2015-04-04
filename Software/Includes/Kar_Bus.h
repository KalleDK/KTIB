#ifndef KAR_BUS_H
#define KAR_BUS_H

#include "RS485_Bus.h"

//PMSFlex <-> Kar Commands
enum {
	KARB_NONE = 0,
	KARB_REQ_FS_DATA = 1,
	KARB_RES_FS_DATA = 2,
	KARB_REQ_VENTIL = 3,
	KARB_RES_VENTIL = 4,
};

#ifdef DEBUG
const char* kar_cmd_str(uint8 id)
{
		switch(id)
		{
			case KARB_NONE:
				return "KARB_NONE";
			case KARB_REQ_FS_DATA:
				return "KARB_REQ_FS_DATA";
			case KARB_RES_FS_DATA:
				return "KARB_RES_FS_DATA";
			case KARB_REQ_VENTIL:
				return "KARB_REQ_VENTIL";
			case KARB_RES_VENTIL:
				return "KARB_RES_VENTIL";
			default:
				return "";
		}
}
#endif

#endif