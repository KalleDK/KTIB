#include <iostream>
#include <unistd.h>
#include "SocketClient.h"
#include "events.h"


using namespace std;


void SocketClient::run() {
    running_ = true;
    
    // FIXME What if client closes connection unexpectedly?
    
    while(running_) {
        // Handle data incoming from client
        handle_incoming();
        
        // Handle sending data to client
        handle_outgoing();
        
        wait(1);
    }
    cout << "KILLING CLIENT" << endl;
    close(sock_fd_);
}


void SocketClient::handle_outgoing() {
    int n;
    long event_id;
    string data;
    
    Message* msg = queue_.recieve_nowait(event_id);
    
    if (event_id != -1) {
        data = msg->getData() + "\r\n";
        n = write(sock_fd_, data.c_str(), data.length());
        
        if (n < 0) {
            cout << "ERROR writing to socket" << endl;
            running_ = false;
        }
    }
    
    if(msg != NULL)
        delete msg;
}


void SocketClient::handle_incoming() {
    string data;
    char* buf = new char[SOCK_BUFFER_SIZE];
    int n;
    
    n = read(sock_fd_, buf, SOCK_BUFFER_SIZE-1);
    
    if (n > 0) {
        data = string(buf, n);
        parse_incoming_data(&data);
    }
    /*
    else if(n < 0) {
        cout << "ERROR reading from socket" << endl;
        running_ = false;
    }
    */
    delete buf;
}


void SocketClient::parse_incoming_data(string* data) {
    string line;
    size_t end_pos;
    
    buffer_.append(*data);
    
    // FIXME What if the client never sends \r\n ?!?!?!
    
    while((end_pos = buffer_.find("\r\n")) != string::npos) {
        line = buffer_.substr(0, end_pos);
        buffer_ = buffer_.substr(end_pos + 2);
        if(line.length())
            parse_incoming_line(&line);
    }
}


void SocketClient::parse_incoming_line(string* line) {
    string cmd;
    string args;
    size_t pos = line->find(" ");
    
    if(pos != string::npos) {
        cmd = line->substr(0, pos);
        args = line->substr(pos + 1);
    } else {
        cmd = *line;
    }
    
    handle_incoming_command(cmd, args);
}


void SocketClient::handle_incoming_command(std::string cmd, std::string args) {
    GuiMessage* msg = new GuiMessage(this, 5);
    
    if(cmd == "MWSTART") {
        bridge_->send(E_START_WATERING, msg);
    } else if(cmd == "MWSTOP") {
        bridge_->send(E_STOP_WATERING, msg);
    } else if(cmd == "MWSTATUS") {
        bridge_->send(E_WATERING_STATUS, msg);
    } else {
        cout << "SocketClient recieved invalid command: " << cmd << "(args: " << args << ")" << endl;
    }
}
