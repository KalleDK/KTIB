#ifndef RS485_H_
#define RS485_H_

#include <termios.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

#define BUFFER_SIZE 50
#define RS485DEBUG
//#define DEVKIT8000


enum {
	MSG_POS_RX_ADDR = 0,
	MSG_POS_TX_ADDR,
	MSG_POS_LEN,
	MSG_POS_CMD,
	MSG_POS_ARGS,
};

enum {
	MSG_IDLE = 0,
	MSG_RX_ADDR,
	MSG_TX_ADDR,
	MSG_LEN,
	MSG_CMD,
	MSG_ARGS,
	MSG_READY,
	MSG_ERROR,
};

struct rawMsg{
	char data[BUFFER_SIZE];
	char* head;
	uint8_t status;
	uint8_t remainingArgs;
	uint8_t errorCount;
};

class RS485
{
	public:
		RS485(char* port, int baud);
		~RS485();
		void sendPacket(char * packet, unsigned int len);
		void getPacket(void);
		bool getMessage(char* msg);
		void initBuffer();
		int set_interface_attribs(int baud);
		
	private:
		void sendChar(char ch, bool address = false);
		int getChar(char &ch, bool &address);
		bool parityCheck(char &ch, bool parity = false);
		struct termios tio_;
		int tty_fd_;
		rawMsg buffer_array[1];
		rawMsg* readInBuf;
};

#endif
