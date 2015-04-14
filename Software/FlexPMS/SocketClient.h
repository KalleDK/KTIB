#pragma once
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include "MessageThread.h"


#define SOCK_BUFFER_SIZE 256


class Message;


class SocketClient : public MessageThread {
public:
    SocketClient(MessageThread* b, int sock_fd) : 
        bridge_(b), 
        sock_fd_(sock_fd),
        reader_(this) {
            reader_.start();
        };
    
    ~SocketClient() {
        reader_.stop();
        close(sock_fd_);
        stop();
    }
    
    int getSockFD() {
        return sock_fd_;
    }
    
private:
    class SocketReader : public Thread {
    public:
        SocketReader(SocketClient* c) : client_(c) {};
        void run();
    private:
        SocketClient* client_;
    };
    
    SocketReader reader_;
    MessageThread* bridge_;
    int sock_fd_;
    std::string buffer_;
    
    void dispatch(unsigned long event_id, Message* msg);
    void handle_kill();
    void handle_send_data(Message* msg);
    void handle_recieve_data(Message* msg);
    
    void parse_incoming_data(std::string* data);
    void parse_incoming_line(std::string* line);
    void handle_incoming_command(std::string cmd, std::string args);
};
