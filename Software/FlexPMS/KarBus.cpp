#include "KarBus.h"
#include "Message.h"

enum {
	REQ_KAR_SENSOR_DATA
	REQ_KAR_AKTUATOR_DATA
	REQ_KAR_OE_SENSOR_DATA
	REQ_KAR_OE_VENTIL
	REQ_KAR_OE_SENSOR_TYPE
	REQ_KAR_VENTIL
	REQ_KAR_OE_LIST
}

enum {
	CNF_KAR_SENSOR_DATA
	CNF_KAR_AKTUATOR_DATA
	CNF_KAR_OE_SENSOR_DATA
	CNF_KAR_OE_VENTIL
	CNF_KAR_OE_SENSOR_TYPE
	CNF_KAR_VENTIL
	CNF_KAR_OE_LIST
}


KarBus::KarBus(char masterAddr)	: masterAddr_(masterAddr), serialPort_("/dev/ttyO2",19200) {
	
}


void KarBus::dispatch(unsigned long event_id, Message* msg) {
	char address = msg->kar->address;
	unsigned int data_length;
	char cmd;
	char* message[BUFFER_SIZE];
	
	message = msg->getData(data_length);
	
	switch event_id
	{
		case REQ_KAR_SENSOR_DATA:
			cmd = 1;
			break;
		case REQ_KAR_AKTUATOR_DATA:
			cmd = 3;
			break;
		case REQ_KAR_OE_SENSOR_DATA:
			cmd = 5;
			break;
		case REQ_KAR_OE_VENTIL:
			cmd = 7;
			break;
		case REQ_KAR_OE_SENSOR_TYPE:
			cmd = 9;
			break;
		case REQ_KAR_VENTIL:
			cmd = 11;
			break;
		case REQ_KAR_OE_LIST:
			cmd = 13;
			break;
		default:
			break;
	}
	constructMessage(message, cmd, address);
	serialPort_.sendPacket(this->data_, len + 4);
	
	while(!serialPort_.getMessage(data_))
		serialPort_.getPacket();
	
	KarMessage* response = new KarMessage(msg->kar);
	response->setData((data_ + 4), data_[3]);
	msg->sender->send(response);
}


void KarBus::constructMessage(char* message, char cmd, char address, char len){
	data_[0] = address;
	data_[1] = masterAddr_;
	data_[2] = len;
	data_[3] = cmd;
	for(int i = 4; i < len + 4; ++i){
		data_[i] = message[i-4];
	}
}

