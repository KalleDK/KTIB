#include <iostream>
#include "KarBus.h"
#include "Message.h"
#include "events.h"

using namespace std;

void KarBus::eHandleKarReady(MKarReady* msg){
	unsigned char karState;
	unsigned long response_id;
	if(karComAVS_.getKarRdy(msg->kar->address, karState)) {
		response_id = E_KAR_READY_STATE;
		MKarReadyState* response = new MKarReadyState(this, msg->kar);
		response->is_ready = karState;
		msg->sender->send(response_id,response);
	}
}

void KarBus::eHandleKarGetSensorData(MKarGetSensorData* msg){
	unsigned char len;
	unsigned char data[50];
	unsigned char tempSensorValue;
	MKarSensorData::KarSensorData tempPusher;
	unsigned long response_id;
	if(karComAVS_.getKarSensorData(msg->kar->address, len, data)) {
		response_id = E_KAR_SENSOR_DATA;
		MKarSensorData* response = new MKarSensorData(this, msg->kar);
		for(int i = 0; i < len; i = i +3) {
			if(data[i+2] & 128){
				tempSensorValue = data[i+1] + 0.5;
			} else {
				tempSensorValue = data[i+1];
			}
			tempPusher.sensor_id = data[i];
			tempPusher.value = tempSensorValue;
			response->sensor_data.push_back(tempPusher);
		}
		msg->sender->send(response_id,response);
	}
}

void KarBus::eHandleKarSetPumpState(MKarSetPumpState* msg){
	unsigned char state;
	unsigned long response_id;
	switch(msg->state) {
		case MKarSetPumpState::OFF :
			state = 0;
			break;
		case MKarSetPumpState::SLOW :
			state = 25;
			break;
		case MKarSetPumpState::MIDDLE :
			state = 60;
			break;
		case MKarSetPumpState::FAST :
			state = 90;
			break;
		default:
			break;
	}
	if(karComAVS_.setPumpState(msg->kar->address, state)) {
		response_id = E_KAR_PUMP_STATE;
		MKarPumpState* response = new MKarPumpState(this, msg->kar);
		if(state <= 0) {
			response->state = MKarPumpState::OFF;
		} else if(state <= 30) {
			response->state = MKarPumpState::SLOW;
		} else if(state <= 65) {
			response->state = MKarPumpState::MIDDLE;	
		} else if(state <= 100) {
			response->state = MKarPumpState::FAST;
		}
		msg->sender->send(response_id,response);
	}
}

void KarBus::eHendleOeGetSensorData(MOeGetSensorData* msg){
	unsigned long response_id;
	unsigned char len;
	unsigned char data[50];
	unsigned char tempSensorValue;
	MOeSensorData::OeSensorData tempPusher;
	
	if(karComAVS_.getOeSensorData(msg->kar->address, msg->oe_id, len, data)) {
		response_id = E_OE_SENSOR_DATA;
		MOeSensorData* response = new MOeSensorData(this, msg->kar);
		for(int i = 0; i < len; i = i +4) {
			if(data[i+3] & 128){
				tempSensorValue = data[i+2] + 0.5;
			} else {
				tempSensorValue = data[i+2];
			}
			tempPusher.sensor_id = data[i];
			tempPusher.status = data[i+1];
			tempPusher.oe_id = msg->oe_id;
			tempPusher.value = tempSensorValue;
			response->sensor_data.push_back(tempPusher);
		}
		msg->sender->send(response_id,response);
	}
}


void KarBus::eHandleOeSetValve(MOeSetValveState* msg){
	unsigned long response_id;
	unsigned char state;
	switch(msg->state) {
		case MOeSetValveState::CLOSED :
			state = 0;
			break;
		case MOeSetValveState::OPEN :
			state = 1;
			break;
		default:
			break;
	}
	
	if(karComAVS_.setOeValve(msg->kar->address, msg->oe_id, state)) {
		response_id = E_OE_VALVE_STATE;
		MOeValveState* response = new MOeValveState(this, msg->kar);
		if(state == 0) {
			response->state = MOeValveState::CLOSED;
		} else {
			response->state = MOeValveState::OPEN;
		}
		msg->sender->send(response_id,response);
	}
}

void KarBus::eHandleOeGetSensorType(MOeGetSensorType* msg){
	unsigned long response_id;
	unsigned char type = msg->sensor_id;
	
	response_id = E_OE_SENSOR_TYPE;
	if(karComAVS_.getOeSensorType(msg->kar->address, msg->oe_id, type)) {
		MOeSensorType* response = new MOeSensorType(this, msg->kar);
		response->oe_id = msg->oe_id;
		response->sensor_id = msg->sensor_id;
		response->sensor_type = type;
		msg->sender->send(response_id,response);
	}
}

void KarBus::eHandleKarSetValve(MKarSetValveState* msg){
	unsigned long response_id;
	unsigned char state;
	unsigned char valve;
	switch(msg->state) {
		case MKarSetValveState::CLOSED :
			state = 0;
			break;
		case MKarSetValveState::OPEN :
			state = 1;
			break;
		default:
			break;
	}
	switch(msg->valve) {
		case MKarSetValveState::INTAKE :
			state = 1;
			break;
		case MKarSetValveState::OUTTAKE :
			state = 2;
			break;
		default:
			break;
	}
	
	if(karComAVS_.setKarValve(msg->kar->address, valve, state)) {
		response_id = E_KAR_VALVE_STATE;
		MKarValveState* response = new MKarValveState(this, msg->kar);
		if(valve == 1) {
			response->valve = MKarValveState::INTAKE;
		} else if(valve == 2) {
			response->valve = MKarValveState::OUTTAKE;
		}
		if(state == 0) {
			response->state = MKarValveState::CLOSED;
		} else if(state == 1) {
			response->state = MKarValveState::OPEN;
		}
		msg->sender->send(response_id,response);
	}
}

void KarBus::eHandleKerGetOeList(MKarGetOeList* msg){
	unsigned long response_id;
	unsigned char len = 0;
	unsigned char data[50];
	
	if(karComAVS_.getKarOelist(msg->kar->address, len, data)) {
		response_id = E_KAR_OE_LIST;
		MKarOeList* response = new MKarOeList(this, msg->kar);
		for(int i = 0; i < len; ++i) {
			response->oe_ids.push_back(data[i]);
		}
		msg->sender->send(response_id,response);
	}
}



void KarBus::dispatch(unsigned long event_id, Message* msg) {

	switch (event_id)
	{
		case E_KAR_READY:
			cout << "KarBus: E_KAR_READY" << endl;
			eHandleKarReady(static_cast<MKarReady*>(msg));
			break;
		case E_KAR_GET_SENSOR_DATA:
			cout << "KarBus: E_KAR_GET_SENSOR_DATA" << endl;
			eHandleKarGetSensorData(static_cast<MKarGetSensorData*>(msg));
			break;
		case E_KAR_SET_PUMP_STATE:
			cout << "KarBus: E_KAR_SET_PUMP_STATE" << endl;
			eHandleKarSetPumpState(static_cast<MKarSetPumpState*>(msg));
			break;
		case E_OE_GET_SENSOR_DATA:
			cout << "KarBus: E_OE_GET_SENSOR_DATA" << endl;
			eHendleOeGetSensorData(static_cast<MOeGetSensorData*>(msg));
			break;
		case E_OE_SET_VALVE_STATE:
			cout << "KarBus: E_OE_SET_VALVE_STATE" << endl;
			eHandleOeSetValve(static_cast<MOeSetValveState*>(msg));
			break;
		case E_OE_GET_SENSOR_TYPE:
			cout << "KarBus: E_OE_GET_SENSOR_TYPE" << endl;
			eHandleOeGetSensorType(static_cast<MOeGetSensorType*>(msg));
			break;
		case E_KAR_SET_VALVE_STATE:
			cout << "KarBus: E_KAR_SET_VALVE_STATE" << endl;
			eHandleKarSetValve(static_cast<MKarSetValveState*>(msg));
			break;
		case E_KAR_GET_OE_LIST:
			cout << "KarBus: E_KAR_GET_OE_LIST" << endl;
			eHandleKerGetOeList(static_cast<MKarGetOeList*>(msg));
			break;
		default:
			break;
	}

}
