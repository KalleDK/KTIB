#pragma once
#include <map>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "Thread.h"
#include "SocketClient.h"
#include "Message.h"


#define SOCK_HOST localhost
#define SOCK_PORT 5555


class SocketServer : public Thread {
public:
    SocketServer(Thread* b) : bridge_(b) {};
    void run();
private:
    Thread* bridge_;
    std::map<unsigned int, Thread*> clients_;
    int sock_fd_;
    socklen_t client_len_;
    
    void init();
    void handle_connection();
};