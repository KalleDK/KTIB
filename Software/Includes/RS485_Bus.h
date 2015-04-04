#ifndef RS485_BUS_H_
#define RS485_BUS_H_

#include <stdlib.h>

enum {
	RS_RECEIVER = 0,
	RS_SENDER = 1,
	RS_LEN_HIGH = 2,
	RS_LEN_LOW = 3,
	RS_CMD = 4,
	RS_ARG_START = 5,
}; 

enum {
   RS_MSG_EMPTY = 0,
	RS_MSG_RECEIVER = 1,
	RS_MSG_SENDER = 2,
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
			case RS_MSG_SENDER:
				return "RS_MSG_SENDER";
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
   volatile uint8* front;
   volatile uint8* back;
	void (*WriteTxData)(uint32);
   uint16 missing_args;
   uint8 msg[MESSAGE_QUEUE_SIZE];
} MessageQueue;


void moveMessageQueuePointer(volatile uint8* msg, volatile uint8* volatile * pointer);

void clearMessageQueue(volatile MessageQueue* q);

void initMessageQueue(volatile MessageQueue** q, void (*WriteTxData)(uint32));

void pushMessageQueue(volatile MessageQueue* q, char ch);

void verifyMessageQueue(volatile MessageQueue* q);

void sendCommand(volatile MessageQueue* q, uint16 receiver, uint8 transmitter, uint16 len, uint8 cmd);

void sendArg(volatile MessageQueue* q, uint8 arg);

void moveMessageQueuePointer(volatile uint8* msg, volatile uint8* volatile * pointer) {
	if (*pointer != &msg[MESSAGE_QUEUE_SIZE-1])
		++(*pointer);
	else
		*pointer = &msg[0];  
}

void clearMessageQueue(volatile MessageQueue* q) {
	q->front = &q->msg[0];
	q->back = &q->msg[0];
	q->missing_args = 0;
	q->status = RS_MSG_EMPTY;
	q->msg[RS_RECEIVER] = 0x00;
	q->msg[RS_SENDER] = 0x00;
	q->msg[RS_LEN_HIGH] = 0;
	q->msg[RS_LEN_LOW] = 0;
	q->msg[RS_CMD] = 0;
}

void initMessageQueue(volatile MessageQueue** q, void (*WriteTxData)(uint32)) {
	//Create new MessageQueue
	*q = malloc(sizeof(MessageQueue));
	(*q)->WriteTxData = WriteTxData;
	
	//Clear the MessageQueue
	clearMessageQueue(*q);
}

void pushMessageQueue(volatile MessageQueue* q, char ch) {
	//Add char to Message
	*(q->front) = ch;
	 
	//Move to next space
	moveMessageQueuePointer(q->msg, &q->front);
}

void verifyMessageQueue(volatile MessageQueue* q) {
	while (q->front != q->back && q->status != RS_MSG_READY) {
		 
		switch(q->status) {
			case RS_MSG_EMPTY :
				++q->status;
			case RS_MSG_RECEIVER :
			case RS_MSG_SENDER :
			case RS_MSG_LEN_HIGH :
			case RS_MSG_LEN_LOW :
				 ++q->status;
				 break;
			case RS_MSG_CMD :
				 ++q->status;
				 q->missing_args = (q->msg[RS_LEN_HIGH] << 8) | q->msg[RS_LEN_LOW];
				 break;
			case RS_MSG_ARGS :
				 --q->missing_args;
				 break;
			default:
				 break;
		}
		  
		if (q->status == RS_MSG_ARGS && q->missing_args == 0) {
			q->status = RS_MSG_READY;
		}
			
		 moveMessageQueuePointer(q->msg, &(q->back));
	}
}

void sendCommand(volatile MessageQueue* q, uint16 receiver, uint8 transmitter, uint16 len, uint8 cmd) {
	//Set specialbit to mark receiver
	receiver |= 0x100u;
	q->WriteTxData(receiver);
	q->WriteTxData(transmitter);
	q->WriteTxData(len >> 8);
	q->WriteTxData(len);
	q->WriteTxData(cmd);
}

void sendArg(volatile MessageQueue* q, uint8 arg) {
	q->WriteTxData(arg);
}
#endif
