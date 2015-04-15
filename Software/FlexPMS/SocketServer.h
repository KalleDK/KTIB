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
#include "MessageThread.h"


#define SOCK_HOST localhost
#define SOCK_PORT 5556


class SocketClient;


class SocketServer : public Thread {
public:
    SocketServer(MessageThread* b) : bridge_(b) {};
    void run();
    void remove_client(int client_sock_fd);
private:
    MessageThread* bridge_;
    std::map<int, SocketClient*> clients_;
    int sock_fd_;
    
    void init();
    void handle_connection();
};
