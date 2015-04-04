#ifndef RS485_BUS_H_
#define RS485_BUS_H_

#include <stdlib.h>

enum {
	RS_RECEIVER = 0,
	RS_TRANSMITTER = 1,
	RS_LEN_HIGH = 2,
	RS_LEN_LOW = 3,
	RS_CMD = 4,
	RS_ARG_START = 5,
}; 

enum {
   RS_MSG_EMPTY = 0,
	RS_MSG_RECEIVER = 1,
	RS_MSG_TRANSMITTER = 2,
   RS_MSG_LEN_HIGH = 3,
   RS_MSG_LEN_LOW = 4,
   RS_MSG_CMD = 5,
   RS_MSG_ARGS = 6,
   RS_MSG_READY = 7,
};

#ifdef DEBUG
const char* msg_status_str(uint8 id)
{
		switch(id)
		{
			case RS_MSG_EMPTY:
				return "RS_MSG_EMPTY";
			case RS_MSG_RECEIVER:
				return "RS_MSG_RECEIVER";
			case RS_MSG_TRANSMITTER:
				return "RS_MSG_TRANSMITTER";
			case RS_MSG_LEN_HIGH:
				return "RS_MSG_LEN_HIGH";
			case RS_MSG_LEN_LOW:
				return "RS_MSG_LEN_LOW";
			case RS_MSG_CMD:
				return "RS_MSG_CMD";
			case RS_MSG_ARGS:
				return "RS_MSG_ARGS";
			case RS_MSG_READY:
				return "RS_MSG_READY";
			default:
				return "";
		}
}
#endif

#ifndef MESSAGE_QUEUE_SIZE
#define MESSAGE_QUEUE_SIZE 50
#endif

typedef struct {
	uint8 status;
	uint8 busid;
	volatile uint8* front;
	volatile uint8* back;
	void (*WriteTxData)(uint32);
	uint16 missing_args;
	uint8 msg[MESSAGE_QUEUE_SIZE];
} RS485Bus;


void moveRS485BusPointer(volatile RS485Bus* bus, volatile uint8* volatile * pointer);

void clearRS485Bus(volatile RS485Bus* bus);

void initRS485Bus(volatile RS485Bus** bus, uint8 busid, void (*WriteTxData)(uint32));

void pushRS485Bus(volatile RS485Bus* bus, char ch);

void verifyRS485Bus(volatile RS485Bus* bus);

void sendRS485BusCommand(volatile RS485Bus* bus, uint16 receiver, uint16 len, uint8 cmd);

void sendRS485BusArg(volatile RS485Bus* bus, uint8 arg);

void moveRS485BusPointer(volatile RS485Bus* bus, volatile uint8* volatile * pointer) {
	if (*pointer != &bus->msg[MESSAGE_QUEUE_SIZE-1])
		++(*pointer);
	else
		*pointer = &bus->msg[0];  
}

void clearRS485Bus(volatile RS485Bus* bus) {
	bus->front = &bus->msg[0];
	bus->back = &bus->msg[0];
	bus->missing_args = 0;
	bus->status = RS_MSG_EMPTY;
	bus->msg[RS_RECEIVER] = 0x00;
	bus->msg[RS_TRANSMITTER] = 0x00;
	bus->msg[RS_LEN_HIGH] = 0;
	bus->msg[RS_LEN_LOW] = 0;
	bus->msg[RS_CMD] = 0;
}

void initRS485Bus(volatile RS485Bus** bus, uint8 busid, void (*WriteTxData)(uint32)) {
	//Create new RS485Bus
	*bus = malloc(sizeof(RS485Bus));
	(*bus)->WriteTxData = WriteTxData;
	(*bus)->busid = busid;
	
	//Clear the RS485Bus
	clearRS485Bus(*bus);
}

void pushRS485Bus(volatile RS485Bus* bus, char ch) {
	//Add char to Message
	*(bus->front) = ch;
	 
	//Move to next space
	moveRS485BusPointer(bus, &bus->front);
}

void verifyRS485Bus(volatile RS485Bus* bus) {
	while (bus->front != bus->back && bus->status != RS_MSG_READY) {
		 
		switch(bus->status) {
			case RS_MSG_EMPTY :
				++bus->status;
			case RS_MSG_RECEIVER :
			case RS_MSG_TRANSMITTER :
			case RS_MSG_LEN_HIGH :
			case RS_MSG_LEN_LOW :
				 ++bus->status;
				 break;
			case RS_MSG_CMD :
				 ++bus->status;
				 bus->missing_args = (bus->msg[RS_LEN_HIGH] << 8) | bus->msg[RS_LEN_LOW];
				 break;
			case RS_MSG_ARGS :
				 --bus->missing_args;
				 break;
			default:
				 break;
		}
		  
		if (bus->status == RS_MSG_ARGS && bus->missing_args == 0) {
			bus->status = RS_MSG_READY;
		}
			
		moveRS485BusPointer(bus, &bus->back);
	}
}

void sendCommand(volatile RS485Bus* bus, uint16 receiver, uint16 len, uint8 cmd) {
	//Set specialbit to mark receiver
	receiver |= 0x100u;
	bus->WriteTxData(receiver);
	bus->WriteTxData(bus->busid);
	bus->WriteTxData(len >> 8);
	bus->WriteTxData(len);
	bus->WriteTxData(cmd);
}

void sendArg(volatile RS485Bus* bus, uint8 arg) {
	bus->WriteTxData(arg);
}
#endif
