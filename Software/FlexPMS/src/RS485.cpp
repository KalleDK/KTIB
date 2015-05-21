#include <stdio.h>
#include <iostream>
#include <string>
#include <stdint.h>
#include <unistd.h>
#include "RS485.h"


using namespace std;

RS485::RS485(std::string port_path, int baud) {
	tty_fd_ = open(port_path.c_str(), O_RDWR | O_NOCTTY | O_SYNC);
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
	parityWatcher_ = false;
	readInBuf = buffer_array;
	initBuffer();
	initGPIO();
}

RS485::~RS485() {
	close(tty_fd_);
	close(gpio_fd_);
	gpio_fd_ = open("/sys/class/gpio/unexport", O_WRONLY);
	write(gpio_fd_, "18", 2);
	close(gpio_fd_);
}

void RS485::initGPIO() {
	gpio_fd_ = open("/sys/class/gpio/export", O_WRONLY);
	if(gpio_fd_ < 0)
	{
		cout << "Could not create gpio" << endl;
	}
	write(gpio_fd_, "18", 2);
	close(gpio_fd_);
	gpio_fd_ = open("/sys/class/gpio/gpio18/direction", O_WRONLY);
	if(gpio_fd_ < 0)
	{
		cout << "Could not set gpio direction" << endl;
	}
	write(gpio_fd_, "out", 3);
	close(gpio_fd_);
	gpio_fd_ = open("/sys/class/gpio/gpio18/value", O_WRONLY | O_SYNC);
	if(gpio_fd_ < 0)
	{
		cout << "could not open gpio for changing state" << endl;
	}
}

void RS485::txEnable(bool state) {
	char setDirection;
	setDirection = (state ? '1' : '0');
	cout << "gpio value is: "  << state << "\n";
	if(!write(gpio_fd_, &setDirection, 1))
	{
		cout << "I did not write to the GPIO" << endl;
	}
}

int RS485::set_interface_attribs(int speed) {
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
	if (parityCheck(ch, address) == true) {
		tio_.c_cflag |= PARODD;
		parityWatcher_ = true;
	} else {
		tio_.c_cflag &= ~PARODD;
		parityWatcher_ = false;
	}
	tcsetattr(tty_fd_, TCSADRAIN, &tio_);
	write(tty_fd_, &ch, 1);
	tcdrain(tty_fd_);
}

bool RS485::parityCheck(char &ch, bool parity) {
	for (unsigned char i = 0; i < 8; ++i) {
		if (ch & (1 << i)) {
			parity = (parity ? false : true);
		}
	}
//	cout << "Char: \t" << (int)ch << " Parity: " << parity << endl; 
	return parity;
}

int RS485::getChar(char &ch, bool &address) {
	int readRetur;
	bool parity = true;
	address = parityWatcher_;
//	cout << "Reveicer in parity: " << (int)(tio_.c_cflag & PARODD) << "  ";
	readRetur = read(tty_fd_, &ch, 1);
	if(ch == 0xFF){
		readRetur = read(tty_fd_, &ch, 1);
		if(ch == 0x00){
			parity = false;
			readRetur = read(tty_fd_, &ch, 1);
		}
	}
	address = parityCheck(ch, address) == parity;
	
	#ifdef RS485_DEBUG_DEEP_PACKET
	cout << "Reading a single byte:" << "\t" << "Number of bytes read:\n";
	cout << "Character as integer: " << (unsigned int)ch << "\t" << "Bytes read: " << readRetur << "\n\n";
	#endif
	
	return readRetur;
}

void RS485::getPacket() {
	char in = 0;
	bool addr = true;

	while(getChar(in, addr) > 0){
		
		#ifdef RS485_DEBUG_STATE_MACHINE
		cout << hex;
		cout << "------ State Machine Received Byte ------\n";
		cout << "|Receiving chars:\t\t\t|\n";
		cout << "|State is        \t" << (unsigned int)readInBuf->status << "\t        |\n";
		cout << "|Char in packet: \t" << (unsigned int)in << "\t        |\n" ;
		cout << "|addr:           \t" << addr << "\t        |\n";
		cout << "-----------------------------------------\n";
		cout << dec;
		#endif
		
		if(addr == true){
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
			#ifdef RS485_DEBUG_STATE_MACHINE_MSG
			cout << hex;
			cout << "----- ReceivedPacket -----\n";
			cout << "|RX addr \t" << "0x" << (int)buffer_array[0].data[0] << "\t |\n";
			cout << "|TX addr \t" << "0x" << (int)buffer_array[0].data[1] << "\t |\n";
			cout << "|Length  \t" << "0x" << (int)buffer_array[0].data[2] << "\t |\n";
			cout << "|Command \t" << "0x" << (int)buffer_array[0].data[3] << "\t |\n";
			for(int i = 0; i < (int)buffer_array[0].data[2]; ++i)
				cout << "|Args" << i << "     \t" << "0x" << (int)buffer_array[0].data[4+i] << "\t |\n";
			cout << "--------------------------\n";
			cout << dec;
			#endif
			break;
		}
		if(readInBuf->errorCount >= 5){
			readInBuf->status = MSG_IDLE;
			tcflush(tty_fd_, TCIFLUSH);
			break;
		}
	}
}

void RS485::sendPacket(char *packet, unsigned int len) {
//	char flip = 0x1;
	#ifdef RS485_DEBUG_SENDER
	cout << hex;
	cout << "----- Sending packet -----\n";
	cout << "|RX addr \t" << "0x" << (int)packet[0] << "\t |\n";
	cout << "|TX addr \t" << "0x" << (int)packet[1] << "\t |\n";
	cout << "|Length  \t" << "0x" << (int)packet[2] << "\t |\n";
	cout << "|Command \t" << "0x" << (int)packet[3] << "\t |\n";
	for(int i = 0; i < (int)packet[2]; ++i)
		cout << "|Args" << i << "    \t" << "0x" << (int)packet[4+i] << "\t |\n";
	cout << "--------------------------\n";
	cout << dec;
	#endif
	txEnable(true);
	sendChar(packet[0], true);
	for (unsigned int i=1; i<len; i++) {
		sendChar(packet[i]);
	}
	#ifdef RS485DEBUG
	cout << "Done sending\n";
	#endif
	txEnable(false);
/*	if (parityCheck( flip, false) == true) {
		tio_.c_cflag |= PARODD;
	} else {
		tio_.c_cflag &= ~PARODD;
	}
	tcsetattr(tty_fd_, TCSADRAIN, &tio_);*/
}

bool RS485::getMessage(char* msg) {
	if(readInBuf->status == MSG_READY){
		strcpy(msg, readInBuf->data);
		initBuffer();
		return true;
	}
	return false;
}

void RS485::initBuffer() {
	readInBuf->status = MSG_IDLE;
	readInBuf->errorCount = 0;
	readInBuf->remainingArgs = 0;
}

void RS485::RS485send(Bus_Message* txMessage) {
//	cout << "RS485: I am preparing the message" << endl;
	rxBuffer_[0] = txMessage->rxAddr;
	rxBuffer_[1] = txMessage->txAddr;
	rxBuffer_[2] = txMessage->len;
	rxBuffer_[3] = txMessage->cmd;
	for(int i = 0; i < rxBuffer_[2]; ++i) {
		rxBuffer_[i + 4] = txMessage->args[i];
	}
//	cout << "RS485: I am sending" << endl;
	sendPacket(rxBuffer_, 4 + rxBuffer_[2]);
}

bool RS485::RS485read(Bus_Message* rxMessage) {
	for(int i = 0; !getMessage(rxBuffer_) && i < 10; i++) {
		getPacket();
		usleep(10);
		if(i == 9) {
			cout << "|----- Could not recieve -----|" << endl;
			initBuffer();
			return false;
		}
	} 
	rxMessage->rxAddr = readInBuf->data[0];
	rxMessage->txAddr = readInBuf->data[1];
	rxMessage->len = readInBuf->data[2];
	rxMessage->cmd = readInBuf->data[3];
	for(int i = 0; i < readInBuf->data[2]; ++i) {
		rxMessage->args[i] = readInBuf->data[i + 4];
	}
	return true;
}

void RS485::RS485readDebug() {
	char read_out;
	cout << hex;
	while(1){
		if(read(tty_fd_,&read_out,1)){
			cout << "0x" << (unsigned int)read_out << endl;
		}
	}
}

