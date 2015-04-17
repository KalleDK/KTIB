#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include "RS485.h"

using namespace std;

RS485::RS485(char* port_path, int baud)
{
	tty_fd_ = open(port_path, O_RDWR | O_NOCTTY | O_SYNC);
	if(tty_fd_<0){
		cout << "Could not open file, error: " << errno << "\n";
	}
	set_interface_attribs(baud);
	#ifdef DEVKIT8000
	int fd = open("/sys/class/cplddrv/cpld/ext_serial_if_route_reg", O_WRONLY);
	if (fd < 0){
		cout << "Could not set serialport direction\n";
	}
	write(fd, "0x2", 3);
	close(fd);
	#endif
	readInBuf = buffer_array;
	initBuffer();
}

RS485::~RS485()
{
	close(tty_fd_);	
}

int RS485::set_interface_attribs(int speed)
{
        memset(&tio_, 0, sizeof(tio_));
        if (tcgetattr (tty_fd_, &tio_) != 0)
        {
                perror("error %d from tcgetattr");
                return -1;
        }

        cfsetospeed (&tio_, speed);
        cfsetispeed (&tio_, speed);

        tio_.c_cflag = (tio_.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break as \000 chars
        tio_.c_iflag &= ~IGNBRK;        					// disable break processing
		tio_.c_iflag |= INPCK | PARMRK;  				// Mark bits with parity errors
        tio_.c_lflag = 0;               					// no signaling chars, no echo, no canonical processing
        tio_.c_oflag = 0;                		 		// no remapping, no delays
        tio_.c_cc[VMIN]  = 0;           			 		// read blocks
        tio_.c_cc[VTIME] = 5;           			 		// 0.5 seconds read timeout

        tio_.c_iflag &= ~(IXON | IXOFF | IXANY); 		// shut off xon/xoff ctrl

        tio_.c_cflag |= (CLOCAL | CREAD);				// ignore modem controls, enable reading
        tio_.c_cflag &= ~(PARENB | PARODD);      		// shut off parity
		tio_.c_cflag |= PARENB;
        tio_.c_cflag &= ~CSTOPB;
        tio_.c_cflag &= ~CRTSCTS;

        if (tcsetattr (tty_fd_, TCSADRAIN, &tio_) != 0)
        {
                perror("error %d from tcsetattr");
                return -1;
        }
        return 0;
}


void RS485::sendChar(char ch, bool address) {
	if (parityCheck(ch, address)) {
		tio_.c_cflag |= PARODD;
	} else {
		tio_.c_cflag &= ~PARODD;
	}
	tcsetattr(tty_fd_, TCSADRAIN, &tio_);
	write(tty_fd_, &ch, 1);
}

bool RS485::parityCheck(char &ch, bool parity){
	for (unsigned char i = 0; i < 8; ++i) {
		if (ch & (1 << i)) {
			parity = (parity ? false : true);
		}
	}
	return parity;
}

int RS485::getChar(char &ch, bool &address)
{
	int readRetur;
	bool parity = true;
	address = false;
	readRetur = read(tty_fd_, &ch, 1);
	if(ch == 0xFF){
		readRetur = read(tty_fd_, &ch, 1);
		if(ch == 0x00){
			parity = false;
			readRetur = read(tty_fd_, &ch, 1);
		}
	}
	address = parityCheck(ch, address) == parity;
	#ifdef RS485DEBUG
	cout << "Character: " << (unsigned int)ch << " " << "Bytes read: " << readRetur  << " \n";
	#endif
	return readRetur;	
}

void RS485::getPacket()
{
	char in = 0;
	bool addr = false;

	while(getChar(in, addr) > 0){
		#ifdef RS485DEBUG
		cout << "Char in packet: " << (unsigned int)in << " addr: " << addr << "\n\n";
		#endif
		if(addr == true){
			#ifdef RS485DEBUG
			cout << "addr true\n";
			#endif
			if(readInBuf->status == MSG_IDLE){
				if(in == 1){
					readInBuf->head = readInBuf->data;
					++readInBuf->status;
				}
			}else if(readInBuf->status == MSG_ERROR){
				if(in == 1){
					readInBuf->head = readInBuf->data;
					*readInBuf->head = in;
					++readInBuf->head;
					readInBuf->status = MSG_RX_ADDR;
				}else{
					++readInBuf->errorCount;
				}
			}else{
				readInBuf->status = MSG_ERROR;
			}
			addr = false;
		}

		switch(readInBuf->status){
			case MSG_RX_ADDR:
			case MSG_TX_ADDR:
			case MSG_LEN:
				*readInBuf->head = in;
				++readInBuf->head;
				++readInBuf->status;
				break;
			case MSG_CMD:
				*readInBuf->head = in;
				++readInBuf->head;
				++readInBuf->status;
				readInBuf->remainingArgs = readInBuf->data[MSG_POS_LEN];
				break;
			case MSG_ARGS:
				*readInBuf->head = in;
				++readInBuf->head;
				readInBuf->remainingArgs--;
				break;
			default:
				break;
		}
		if(readInBuf->status == MSG_ARGS && readInBuf->remainingArgs == 0){
			readInBuf->status = MSG_READY;
			break;
		}
		if(readInBuf->errorCount >= 5){
			readInBuf->status = MSG_IDLE;
			tcflush(tty_fd_, TCIFLUSH);
			break;
		}
		cout << "State is " << (unsigned int)readInBuf->status << "\n";
	}
}

void RS485::sendPacket(char *packet, unsigned int len)
{	
	sendChar(packet[0], true);
	for (int i=1; i<len; i++) {
		sendChar(packet[i]);
	}
	tio_.c_cflag &= ~PARODD;
	tcsetattr(tty_fd_, TCSADRAIN, &tio_);
	if (tcflush(tty_fd_, TCIFLUSH) == 0)
	{
		cout << "I flushed the queue\n";
	}
}

bool RS485::getMessage(char* msg)
{
	if(readInBuf->status == MSG_READY){
		strcpy(msg, readInBuf->data);
		initBuffer();
		return true;
	}
	return false;
}

void RS485::initBuffer()
{
	readInBuf->status = MSG_IDLE;
	readInBuf->errorCount = 0;
	readInBuf->remainingArgs = 0;
}



