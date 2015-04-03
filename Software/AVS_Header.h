//Fieldsensor types
enum
{
	unknown = 0,
	jordsensor = 1,
	tempsensor = 2,
};

#ifdef DEBUG
#ifdef FS_STRINGS
const char* fs_types_str(uint8 id)
{
		switch(id)
		{
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

//SensorØ <-> Fieldsensor
enum
{
    returnValues  = 1,
    returnTypes = 2,
};