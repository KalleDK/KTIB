#ifndef FIELDSENSOR_H_
#define FIELDSENSOR_H_

//Fieldsensor types
enum
{
	unknown = 0,
	jordsensor = 1,
	tempsensor = 2,
};

#ifdef DEBUG
const char* fs_types_str(uint8 id)
{
		switch(id)
		{
			case unknown:
				return "Unknown";
			case jordsensor:
				return "Jordsensor";
			case tempsensor:
				return "Tempsensor";
			default:
				return "";
		}
}
#endif

#endif