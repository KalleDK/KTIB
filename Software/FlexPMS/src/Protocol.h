#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#include "RS485.h"

class RS485;

struct Bus_Message {
	unsigned char txAddr;
	unsigned char rxAddr;
	unsigned char len;
	unsigned char cmd;
	unsigned char args[50];
};

class Protocol {
	public:
		Protocol(RS485* comPort, unsigned char masterAddr);
		~Protocol();
		bool getKarRdy(unsigned char karID, unsigned char& karState);
		bool getKarSensorData(unsigned char karID, unsigned char& len, unsigned char* data);
		bool setPumpState(unsigned char karID, unsigned char& state);
		bool getOeSensorData(unsigned char karID, unsigned char oeID, unsigned char& len, unsigned char* data);
		bool setOeValve(unsigned char karID, unsigned char oeID, unsigned char& state);
		bool setKarValve(unsigned char karID, unsigned char& ventilID, unsigned char& state);
		bool getKarOelist(unsigned char karID, unsigned char& len, unsigned char* data);
		bool getOeSensorType(unsigned char karID, unsigned char oeID, unsigned char& fsID);
	private:
		RS485* comPortPtr_;
		unsigned char masterAddr_;
		struct Bus_Message* txMessage_;
		struct Bus_Message* rxMessage_;
		
		bool sendAndReceive();
};


#endif