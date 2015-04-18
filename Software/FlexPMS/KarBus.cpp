#include <iostream>
#include "KarBus.h"
#include "Message.h"
#include "events.h"

using namespace std;

#define UART_FILE "/dev/ttyAMA0"
#define UART_BAUD 19200



KarBus::KarBus(char masterAddr)	: masterAddr_(masterAddr), serialPort_(UART_FILE, UART_BAUD) {

}


void KarBus::dispatch(unsigned long event_id, Message* msg) {

	KarBusMessage* kmsg = static_cast<KarBusMessage*>(msg);
//	char address = kmsg->kar->address;
	char address = 0x2;
	unsigned int data_length;
	char cmd;
	unsigned long response_id;
	const char* message;

	message = kmsg->getData(data_length);

	switch(event_id)
	{
		case REQ_KAR_SENSOR_DATA:
			cmd = 1;
			response_id = E_GET_KAR_SENSOR_DATA_CNF;
			cout << "KarBus: E_REQ_KAR_SENSOR_DATA" << endl;
			break;
		case REQ_KAR_AKTUATOR_DATA:
			cmd = 3;
			response_id = CNF_KAR_AKTUATOR_DATA;
			cout << "KarBus: E_REQ_KAR_AKTUATOR_DATA" << endl;
			break;
		case REQ_KAR_OE_SENSOR_DATA:
			cmd = 5;
			response_id = CNF_KAR_OE_SENSOR_DATA;
			cout << "KarBus: E_REQ_KAR_OE_SENSOR_DATA" << endl;
			break;
		case REQ_KAR_OE_VENTIL:
			cmd = 7;
			response_id = CNF_KAR_OE_VENTIL;
			cout << "KarBus: E_REQ_KAR_OE_VENTIL" << endl;
			break;
		case REQ_KAR_OE_SENSOR_TYPE:
			cmd = 9;
			response_id = CNF_KAR_OE_SENSOR_TYPE;
			cout << "KarBus: E_REQ_KAR_OE_SENSOR_TYPE" << endl;
			break;
		case REQ_KAR_VENTIL:
			cmd = 11;
			response_id = CNF_KAR_VENTIL;
			cout << "KarBus: E_REQ_KAR_VENTIL" << endl;
			break;
		case REQ_KAR_OE_LIST:
			cmd = 13;
			response_id = CNF_KAR_OE_LIST;
			cout << "KarBus: E_REQ_KAR_OE_LIST" << endl;
			break;
		default:
			break;
	}
	constructMessage(message, cmd, address, data_length);
	serialPort_.sendPacket(this->data_, data_length + 4);

	for(int i = 0; !serialPort_.getMessage(data_) && i < 10; i++) {
		cout << "getting packet " << i << endl;
		serialPort_.getPacket();
		sleep(1);
	}
	cout << "Msg done or timedout\n";

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

