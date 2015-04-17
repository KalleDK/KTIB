#include "KarBus.h"
#include "Message.h"


#define UART_FILE "/dev/ttyAMA0"
#define UART_BAUD 19200


enum {
	REQ_KAR_SENSOR_DATA,
	REQ_KAR_AKTUATOR_DATA,
	REQ_KAR_OE_SENSOR_DATA,
	REQ_KAR_OE_VENTIL,
	REQ_KAR_OE_SENSOR_TYPE,
	REQ_KAR_VENTIL,
	REQ_KAR_OE_LIST,
};

enum {
	CNF_KAR_SENSOR_DATA,
	CNF_KAR_AKTUATOR_DATA,
	CNF_KAR_OE_SENSOR_DATA,
	CNF_KAR_OE_VENTIL,
	CNF_KAR_OE_SENSOR_TYPE,
	CNF_KAR_VENTIL,
	CNF_KAR_OE_LIST,
};


KarBus::KarBus(char masterAddr)	: masterAddr_(masterAddr), serialPort_(UART_FILE, UART_BAUD) {
	
}


void KarBus::dispatch(unsigned long event_id, Message* msg) {
	
	KarBusMessage* kmsg = static_cast<KarBusMessage*>(msg);
	char address = kmsg->kar->address;
	unsigned int data_length;
	char cmd;
	unsigned long response_id;
	const char* message;
	
	message = kmsg->getData(data_length);
	
	switch(event_id)
	{
		case REQ_KAR_SENSOR_DATA:
			cmd = 1;
			response_id = CNF_KAR_SENSOR_DATA;
			break;
		case REQ_KAR_AKTUATOR_DATA:
			cmd = 3;
			response_id = CNF_KAR_AKTUATOR_DATA;
			break;
		case REQ_KAR_OE_SENSOR_DATA:
			cmd = 5;
			response_id = CNF_KAR_OE_SENSOR_DATA;
			break;
		case REQ_KAR_OE_VENTIL:
			cmd = 7;
			response_id = CNF_KAR_OE_VENTIL;
			break;
		case REQ_KAR_OE_SENSOR_TYPE:
			cmd = 9;
			response_id = CNF_KAR_OE_SENSOR_TYPE;
			break;
		case REQ_KAR_VENTIL:
			cmd = 11;
			response_id = CNF_KAR_VENTIL;
			break;
		case REQ_KAR_OE_LIST:
			cmd = 13;
			response_id = CNF_KAR_OE_LIST;
			break;
		default:
			break;
	}
	constructMessage(message, cmd, address, data_length);
	serialPort_.sendPacket(this->data_, data_length + 4);
	
	while(!serialPort_.getMessage(data_))
		serialPort_.getPacket();
	
	KarBusMessage* response = new KarBusMessage(this, kmsg->kar);
	response->setData((data_ + 4), data_[3]);
	kmsg->sender->send(response_id, response);
}


void KarBus::constructMessage(const char* message, char cmd, char address, char len){
	data_[0] = address;
	data_[1] = masterAddr_;
	data_[2] = len;
	data_[3] = cmd;
	for(int i = 4; i < len + 4; ++i){
		data_[i] = message[i-4];
	}
}

