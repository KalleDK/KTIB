#ifndef COMMMODULE_H_
#define COMMMODULE_H_

struct Bus_Message {
	unsigned int txAddr;
	unsigned int rxAddr;
	unsigned int len;
	unsigned int cmd;
	unsigned int* args;
};

class CommModule {
	public:
		CommModule() {};
		send(Bus_Message msg) = 0;
		Bus_Message read() = 0;
		
	private:
		
}


#endif