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
    MessageThread* bridge_;
    void dispatch(unsigned long event_id, Message* msg);
	void constructMessage(const char* message, char cmd, char karAddr, char len);
	RS485 serialPort_;
	char masterAddr_;
	char data_[BUFFER_SIZE];
};
