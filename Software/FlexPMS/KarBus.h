#pragma once
#include "MessageThread.h"
#include "RS485.h"

#define BUFFER_SIZE 50


class Message;


class KarBus : public MessageThread {
public:
    KarBus(char Addr = 0x1);
private:
    void dispatch(unsigned long event_id, Message* msg);
	void constructMessage(const char* message, char cmd, char karAddr, char len);
	RS485 serialPort_;
	char masterAddr_;
	char data_[BUFFER_SIZE];
};
