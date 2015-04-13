#pragma once
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "Thread.h"


#define SOCK_BUFFER_SIZE 256


class Message;


class SocketClient : Thread {
public:
    SocketClient(Thread* b, int sock_fd) : bridge_(b), sock_fd_(sock_fd) {};
    void run();
private:
    bool running_;
    Thread* bridge_;
    int sock_fd_;
    std::string buffer_;
    
    void handle_outgoing();
    void handle_incoming();
    void parse_incoming_data(std::string* data);
    void parse_incoming_line(std::string* line);
    void handle_incoming_command(std::string cmd, std::string args);
};
