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
        reader_(this) {};
    
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
    unsigned long session_id_;
    
    void init();
    void shutdown();
    void dispatch(unsigned long event_id, Message* msg);
    void handle_start_session(SessionMessage* msg);
    void handle_stop_session();
    void handle_kill();
    void handle_send_data(Message* msg);
    void handle_recieve_data(Message* msg);
    
    void parse_data_buffer();
    void parse_incoming_data(std::string* data);
    void parse_incoming_line(std::string* line);
    void handle_incoming_command(std::string cmd, std::string args);
    
    void handle_start_watering(std::string args);
    void handle_stop_watering(std::string args);
    void handle_get_watering_status(std::string args);
    
    void handle_ovalve_open(std::string args);
    void handle_ovalve_close(std::string args);
    void handle_ovalve_status(std::string args);
    
    void handle_ivalve_open(std::string args);
    void handle_ivalve_close(std::string args);
    void handle_ivalve_status(std::string args);
	
	void handle_oe_sensor_read(std::string args);
    void handle_kar_sensor_read(std::string args);
	void handle_kar_ready_read(std::string args);
	void handle_oe_list_read(std::string args);
	void handle_sensor_type_read(std::string args);

};
