#include <iostream>
#include "KarBus.h"
#include "Message.h"
#include "events.h"

using namespace std;

void KarBus::eHandleKarReady(MKarReady* msg){
	char address = 0x2;
	unsigned int data_length;
	const char* message;
	message = msg->getData(data_length);
	constructMessage(message, address, data_length);
	cout << "String to send: " << (int)data_[0] << (int)data_[1] << (int)data_[2] << (int)data_[3] << endl;
	serialPort_.sendPacket(this->data_, data_length + 4);
}

void KarBus::eHandleKarGetSensorData(MKarGetSensorData* msg){
	char address = 0x2;
	unsigned int data_length;
	const char* message;
	message = msg->getData(data_length);
	constructMessage(message, address, data_length);
	cout << "String to send: " << (int)data_[0] << (int)data_[1] << (int)data_[2] << (int)data_[3] << endl;
	serialPort_.sendPacket(this->data_, data_length + 4);
}

void KarBus::eHandleKarSetPumpState(MKarSetPumpState* msg){
	char address = 0x2;
	unsigned int data_length;
	const char* message;
	message = msg->getData(data_length);
	constructMessage(message, address, data_length);
	cout << "String to send: " << (int)data_[0] << (int)data_[1] << (int)data_[2] << (int)data_[3] << endl;
	serialPort_.sendPacket(this->data_, data_length + 4);
}

void KarBus::eHendleOeGetSensorData(MOeGetSensorData* msg){
	char address = 0x2;
	unsigned int data_length;
	const char* message;
	message = msg->getData(data_length);
	constructMessage(message, address, data_length);
	cout << "String to send: " << (int)data_[0] << (int)data_[1] << (int)data_[2] << (int)data_[3] << endl;
	serialPort_.sendPacket(this->data_, data_length + 4);
}


void KarBus::eHandleOeSetValve(MOeSetValveState* msg){
	char address = 0x2;
	unsigned int data_length;
	const char* message;
	message = msg->getData(data_length);
	cout << "String to send: " << (int)message[0] << (int)message[1] << (int)message[2] << (int)message[3] << endl;
	constructMessage(message, address, data_length);
	cout << "String to send: " << (int)data_[0] << (int)data_[1] << (int)data_[2] << (int)data_[3] << endl;
	serialPort_.sendPacket(this->data_, data_length + 4);
}

void KarBus::eHandleOeGetSensorType(MOeGetSensorType* msg){
	char address = 0x2;
	unsigned int data_length;
	const char* message;
	message = msg->getData(data_length);
	constructMessage(message, address, data_length);
	cout << "String to send: " << (int)data_[0] << (int)data_[1] << (int)data_[2] << (int)data_[3] << endl;
	serialPort_.sendPacket(this->data_, data_length + 4);
}

void KarBus::eHandleKarSetValve(MKarSetValveState* msg){
	char address = 0x2;
	unsigned int data_length;
	const char* message;
	message = msg->getData(data_length);
	constructMessage(message, address, data_length);
	cout << "String to send: " << (int)data_[0] << (int)data_[1] << (int)data_[2] << (int)data_[3] << endl;
	serialPort_.sendPacket(this->data_, data_length + 4);
}

void KarBus::eHandleKerGetOeList(MKarGetOeList* msg){
	char address = 0x2;
	unsigned int data_length;
	const char* message;
	message = msg->getData(data_length);
	constructMessage(message, address, data_length);
	cout << "String to send: " << (int)data_[0] << (int)data_[1] << (int)data_[2] << (int)data_[3] << endl;
	serialPort_.sendPacket(this->data_, data_length + 4);
}



void KarBus::dispatch(unsigned long event_id, Message* msg) {

	KarBusMessage* kmsg = static_cast<KarBusMessage*>(msg);
	KarBusMessage* response;
	unsigned long response_id;
	char address = 0x2;
	unsigned int data_length;
	char cmd;

	


	switch (event_id)
	{
		case E_KAR_READY:
			eHandleKarReady(static_cast<MKarReady*>(msg));
			cout << "KarBus: E_KAR_READY" << endl;
			break;
		case E_KAR_GET_SENSOR_DATA:
			eHandleKarGetSensorData(static_cast<MKarGetSensorData*>(msg));
			cout << "KarBus: E_KAR_GET_SENSOR_DATA" << endl;
			break;
		case E_KAR_SET_PUMP_STATE:
			eHandleKarSetPumpState(static_cast<MKarSetPumpState*>(msg));
			cout << "KarBus: E_KAR_SET_PUMP_STATE" << endl;
			break;
		case E_OE_GET_SENSOR_DATA:
			eHendleOeGetSensorData(static_cast<MOeGetSensorData*>(msg));
			cout << "KarBus: E_OE_GET_SENSOR_DATA" << endl;
			break;
		case E_OE_SET_VALVE_STATE:
			eHandleOeSetValve(static_cast<MOeSetValveState*>(msg));
			cout << "KarBus: E_OE_SET_VALVE_STATE" << endl;
			break;
		case E_OE_GET_SENSOR_TYPE:
			eHandleOeGetSensorType(static_cast<MOeGetSensorType*>(msg));
			cout << "KarBus: E_OE_GET_SENSOR_TYPE" << endl;
			break;
		case E_KAR_SET_VALVE_STATE:
			eHandleKarSetValve(static_cast<MKarSetValveState*>(msg));
			cout << "KarBus: E_KAR_SET_VALVE_STATE" << endl;
			break;
		case E_KAR_GET_OE_LIST:
			eHandleKerGetOeList(static_cast<MKarGetOeList*>(msg));
			cout << "KarBus: E_KAR_GET_OE_LIST" << endl;
			break;
		default:
			break;
	}
	
	for(int j = 0; j < BUFFER_SIZE; ++j){
		data_[j] = 0;
	}


	for(int i = 0; !serialPort_.getMessage(data_) && i < 10; i++) {
		cout << "getting packet " << i << endl;
		serialPort_.getPacket();
		msleep(1);
	}
	cout << "Msg done or timedout\n";
	
	
	switch ((int)data_[3])
	{
		case 16:
			busHandleKarRdy(kmsg);
			cout << "KarBus: 0" << endl;
			break;
		case 2:
			busHandleKarSensorData(kmsg);
			cout << "KarBus: 2" << endl;
			break;
		case 4:
			busHandlePumpState(kmsg);
			cout << "KarBus: 4" << endl;
			break;
		case 6:
			busHandleOeSensor(kmsg);
			cout << "KarBus: 6" << endl;
			break;
		case 8:
			busHandleOeValveState(kmsg);
			cout << "KarBus: 8" << endl;
			break;
		case 10:
			busHandleSensorType(kmsg);
			cout << "KarBus: 10" << endl;
			break;
		case 12:
			busHandleKarValveState(kmsg);
			cout << "KarBus: 12" << endl;
			break;
		case 14:
			busHandleKarOeList(kmsg);
			cout << "KarBus: 14" << endl;
			break;
		default:
			cout << "Wrong message back " << (int)data_[3] << endl;
			break;
	}

}

void KarBus::busHandleKarRdy(KarBusMessage* msg){
	unsigned long response_id;
	response_id = E_KAR_READY_STATE;
	MKarReadyState* response = new MKarReadyState(this, msg->kar);
	response->is_ready = true;
	msg->sender->send(response_id,response);
}

void KarBus::busHandleKarSensorData(KarBusMessage* msg){
	unsigned long response_id = E_KAR_SENSOR_DATA;
	MKarSensorData* response = new MKarSensorData(this, msg->kar);
	msg->sender->send(response_id,response);
}

void KarBus::busHandlePumpState(KarBusMessage* msg){
	unsigned long response_id = E_KAR_PUMP_STATE;
	MKarPumpState* response = new MKarPumpState(this, msg->kar);
	response->state = MKarPumpState::OFF;
	msg->sender->send(response_id,response);
}

void KarBus::busHandleOeSensor(KarBusMessage* msg){
	unsigned long response_id = E_OE_SENSOR_DATA;
	MOeSensorData* response = new MOeSensorData(this, msg->kar);
	msg->sender->send(response_id,response);
}

void KarBus::busHandleOeValveState(KarBusMessage* msg){
	unsigned long response_id = E_OE_VALVE_STATE;
	MOeValveState* response = new MOeValveState(this, msg->kar);
	response->state = MOeValveState::CLOSED;
	msg->sender->send(response_id,response);
}

void KarBus::busHandleSensorType(KarBusMessage* msg){
	unsigned long response_id = E_OE_SENSOR_TYPE;
	MOeSensorType* response = new MOeSensorType(this, msg->kar);
	msg->sender->send(response_id,response);
}

void KarBus::busHandleKarValveState(KarBusMessage* msg){
	unsigned long response_id = E_KAR_VALVE_STATE;
	MKarValveState* response = new MKarValveState(this, msg->kar);
	response->valve = MKarValveState::INTAKE;
	response->state = MKarValveState::CLOSED;
	msg->sender->send(response_id,response);
}

void KarBus::busHandleKarOeList(KarBusMessage* msg){
	unsigned long response_id = E_KAR_OE_LIST;
	MKarOeList* response = new MKarOeList(this, msg->kar);
	msg->sender->send(response_id,response);
}

void KarBus::constructMessage(const char* message, char address, char len){
	data_[0] = address;
	data_[1] = masterAddr_;
	for(int i = 0; i < len; ++i){
		data_[i+2] = message[i];
	}
}

