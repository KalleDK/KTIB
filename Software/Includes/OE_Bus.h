#ifndef OE_BUS_H
#define OE_BUS_H

#include "RS485_Bus.h"

//SensorØ <-> Kar Commands
enum {
	OEB_NONE = 0,
   OEB_REQ_FS_DATA = 1,
	OEB_RES_FS_DATA = 2,
	OEB_REQ_VENTIL = 3,
	OEB_RES_VENTIL = 4,
};

#ifdef DEBUG
const char* oeb_cmd_str(uint8 id)
{
		switch(id)
		{
			case OEB_NONE:
				return "OEB_NONE";
			case OEB_REQ_FS_DATA:
				return "OEB_REQ_FS_DATA";
			case OEB_RES_FS_DATA:
				return "OEB_RES_FS_DATA";
			case OEB_REQ_VENTIL:
				return "OEB_REQ_VENTIL";
			case OEB_RES_VENTIL:
				return "OEB_RES_VENTIL";
			default:
				return "";
		}
}
#endif

#endif