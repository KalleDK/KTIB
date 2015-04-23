#pragma once
#include "MessageThread.h"
#include "RS485.h"

#define BUFFER_SIZE 50
#define UART_FILE "/dev/ttyAMA0"
#define UART_BAUD 19200


class Message;


class KarBus : public MessageThread {
public:
    KarBus(char masterAddr = 0x1) : 
        masterAddr_(masterAddr), 
        serialPort_(UART_FILE, UART_BAUD) {};
private:
    void dispatch(unsigned long event_id, Message* msg);
	void constructMessage(const char* message, char karAddr, char len);
	RS485 serialPort_;
	char masterAddr_;
	char data_[BUFFER_SIZE];
	
	// Return from PSOC
	void busHandleKarRdy(KarBusMessage* msg);
	void busHandleKarSensorData(KarBusMessage* msg);
	void busHandlePumpState(KarBusMessage* msg);
	void busHandleOeSensor(KarBusMessage* msg);
	void busHandleOeValveState(KarBusMessage* msg);
	void busHandleSensorType(KarBusMessage* msg);
	void busHandleKarValveState(KarBusMessage* msg);
	void busHandleKarOeList(KarBusMessage* msg);

	
	//Send to PSOC
	void eHandleKarReady(MKarReady* msg);
	void eHandleKarGetSensorData(MKarGetSensorData* msg);
	void eHandleKarSetPumpState(MKarSetPumpState* msg);
	void eHendleOeGetSensorData(MOeGetSensorData* msg);
	void eHandleOeSetValve(MOeSetValveState* msg);
	void eHandleOeGetSensorType(MOeGetSensorType* msg);
	void eHandleKarSetValve(MKarSetValveState* msg);
	void eHandleKerGetOeList(MKarGetOeList* msg);
};