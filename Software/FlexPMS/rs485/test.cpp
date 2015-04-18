#include <stdio.h>
#include <iostream>
#include <string>

#include "rs485.hpp"

using namespace std;

/*
void parsePacket(string packet)
{
	switch(packet[3])
	{
		case 2:
			args = packet[5-10]
			break;
		case 4:
			args = packet[5-10]
			break;
		case 6:
			args = packet[5-10]
			break;
		case 8:
			args = packet[5-10]
			break;
		case 10:
			args = packet[5-10]
			break;
		case 12:
			args = packet[5-10]
			break;
		case 14:
			args = packet[5-10]
			break;
		default:
			//wrong :(
			break;
	}
}*/


int main(int argc, char** argv)
{
	string tty_path = "/dev/ttyAMA0";
	char pakke[10];
	char pakke2[10] = "2HAAAT\r\n";
	char retur[60];

	sprintf(pakke, "%c%c%c%c", 0x2, 0x1, 0x0, 0x1);
	char inBuf[10] = "a";
	unsigned int i = 0;
	unsigned int len = 0;


	sprintf(pakke2, "%cTOPHAT\r\n", 0x3);


	#ifdef CMSPAR
	printf("Please start with %s /dev/ttyO1 (Or other tty device)\n", argv[0]);
	#endif

	cout << "Initializing serial port\n";
	RS485 serialPort(tty_path, B19200); 
	cout << "Done initializing\n";


	while (1)
	{
	//	serialPort.sendPacket(pakke, sizeof(pakke));
	//	sleep(1);
	//	serialPort.sendPacket(pakke2, sizeof(pakke2));
		for (i = 0; i < 59; ++i)
			retur[i] = 0;

		serialPort.sendPacket(pakke, 4);
		serialPort.getPacket(retur);
		serialPort.getMessege(retur);


		cout << "received: ";
		len = retur[2] + 4;
		cout << "Retur Len: " << (unsigned int)retur[2] << "\n";
		for(i = 0; i < len; ++i){
			cout << hex << (unsigned int)retur[i] << " ";
		}
		cout << endl;

		usleep(1000);
	}
}
