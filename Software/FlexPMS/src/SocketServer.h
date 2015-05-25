#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "Thread.h"
#include "MessageThread.h"


#define SOCK_HOST localhost
#define SOCK_PORT 5555


class SocketClient;


class SocketServer : public Thread {
public:
    SocketServer(MessageThread* b) : bridge_(b) {};
    void run();
private:
    MessageThread* bridge_;
    int sock_fd_;
    
    void init();
    void handle_connection();
};
