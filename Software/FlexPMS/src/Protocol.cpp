#include <iostream>
#include "Protocol.h"

using namespace std;

enum {
	REQ_KAR_NONE = 0,
	REQ_KAR_SENSOR_DATA,
	RES_KAR_SENSOR_DATA,
	REQ_KAR_VENTIL,
	RES_KAR_VENTIL,
	REQ_KAR_PUMPE,
	RES_KAR_PUMPE,
	REQ_KAR_OPRET,
	RES_KAR_OPRET,
	REQ_KAR_READY,
	RES_KAR_READY,
	REQ_KAR_OE_LIST,
	RES_KAR_OE_LIST,
	REQ_KAR_OE_SENSOR_DATA,
	RES_KAR_OE_SENSOR_DATA,
	REQ_KAR_OE_VENTIL,
	RES_KAR_OE_VENTIL,
	REQ_KAR_OE_SENSOR_TYPE,
	RES_KAR_OE_SENSOR_TYPE,
};

Protocol::Protocol(RS485* comPort, unsigned char masterAddr) : comPortPtr_(comPort), masterAddr_(masterAddr) {
	txMessage_ = new Bus_Message;
	rxMessage_ = new Bus_Message;
}

Protocol::~Protocol() {
	delete txMessage_;
	delete rxMessage_;
}

bool Protocol::getKarRdy(unsigned char karID, unsigned char& karState) {
	txMessage_->rxAddr = karID;
	txMessage_->txAddr = masterAddr_;
	txMessage_->len = 0;
	txMessage_->cmd = REQ_KAR_READY;
	if(sendAndReceive()){
		if(rxMessage_->cmd == RES_KAR_READY) {
			karState = rxMessage_->args[0];
			return true;
		}
	}
	return false;
}
bool Protocol::getKarSensorData(unsigned char karID, unsigned char& len, unsigned char* data) {
	txMessage_->rxAddr = karID;
	txMessage_->txAddr = masterAddr_;
	txMessage_->len = 0;
	txMessage_->cmd = REQ_KAR_SENSOR_DATA;
	if(sendAndReceive()){
		if(rxMessage_->cmd == RES_KAR_SENSOR_DATA) {
			len = rxMessage_->len;
			data = rxMessage_->args;
			return true;
		}
	}
	return false;
}
bool Protocol::setPumpState(unsigned char karID, unsigned char& state) {
	txMessage_->rxAddr = karID;
	txMessage_->txAddr = masterAddr_;
	txMessage_->len = 1;
	txMessage_->cmd = REQ_KAR_PUMPE;
	txMessage_->args[0] = state; 
	if(sendAndReceive()){
		if(rxMessage_->cmd == RES_KAR_PUMPE) {
			state = rxMessage_->args[0];
			return true;
		}
	}
	return false;
}
bool Protocol::getOeSensorData(unsigned char karID, unsigned char oeID, unsigned char& len, unsigned char* data) {
	txMessage_->rxAddr = karID;
	txMessage_->txAddr = masterAddr_;
	txMessage_->len = 1;
	txMessage_->cmd = REQ_KAR_OE_SENSOR_DATA;
	txMessage_->args[0] = oeID;
	if(sendAndReceive()){
		if(rxMessage_->cmd == RES_KAR_OE_SENSOR_DATA) {
			len = rxMessage_->len;
			data = rxMessage_->args;
			return true;
		}
	}
	return false;
}
bool Protocol::setOeValve(unsigned char karID, unsigned char oeID, unsigned char& state) {
	txMessage_->rxAddr = karID;
	txMessage_->txAddr = masterAddr_;
	txMessage_->len = 2;
	txMessage_->cmd = REQ_KAR_OE_VENTIL;
	txMessage_->args[0] = oeID;
	txMessage_->args[1] = state;
	if(sendAndReceive()){
		if(rxMessage_->cmd == RES_KAR_OE_VENTIL) {
			state = rxMessage_->args[0];
			return true;
		}
	}
	return false;
}
bool Protocol::setKarValve(unsigned char karID, unsigned char &ventilID, unsigned char& state) {
	cout << "Protocol: Taking parameters to message" << endl;
	txMessage_->rxAddr = karID;
	txMessage_->txAddr = masterAddr_;
	txMessage_->len = 2;
	txMessage_->cmd = REQ_KAR_VENTIL;
	txMessage_->args[0] = ventilID;
	txMessage_->args[1] = state;
	cout << "Protocol: Done making message sending to RS485" << endl;
	if(sendAndReceive()){
		if(rxMessage_->cmd == RES_KAR_VENTIL) {
			state = rxMessage_->args[0];
			return true;
		}
	}
	return false;
}
bool Protocol::getKarOelist(unsigned char karID, unsigned char& len, unsigned char* data) {
	txMessage_->rxAddr = karID;
	txMessage_->txAddr = masterAddr_;
	txMessage_->len = 0;
	txMessage_->cmd = REQ_KAR_OE_LIST;
	if(sendAndReceive()){
		if(rxMessage_->cmd == RES_KAR_OE_LIST) {
			len = rxMessage_->len;
			data = rxMessage_->args;
			return true;
		}
	}
	return false;
}

bool Protocol::getOeSensorType(unsigned char karID, unsigned char oeID, unsigned char& fsID) {
	txMessage_->rxAddr = karID;
	txMessage_->txAddr = masterAddr_;
	txMessage_->len = 2;
	txMessage_->cmd = REQ_KAR_OE_SENSOR_TYPE;
	txMessage_->args[0] = oeID;
	txMessage_->args[1] = fsID;
	if(sendAndReceive()){
		if(rxMessage_->cmd == RES_KAR_OE_SENSOR_TYPE) {
			fsID = rxMessage_->args[0];
			return true;
		}
	}
	return false;
}

bool Protocol::sendAndReceive() {
	comPortPtr_->RS485send(txMessage_);
	return comPortPtr_->RS485read(rxMessage_);
}
