#include <iostream>
#include <unistd.h>
#include "SocketClient.h"
#include "events.h"


using namespace std;


void SocketClient::dispatch(unsigned long event_id, Message* msg) {
    switch(event_id) {
        case E_KILL:
            cout << "SocketClient recieved: E_KILL" << endl;
            handle_kill();
            break;
        case E_SEND_DATA:
            cout << "SocketClient recieved: E_SEND_DATA" << endl;
            handle_send_data(msg);
            break;
        case E_RECV_DATA:
            cout << "SocketClient recieved: E_RECV_DATA" << endl;
            handle_recieve_data(msg);
            break;
    }
}


/* ------------------------------------------------------------------------- */
/* -- EVENT HANDLERS ------------------------------------------------------- */
/* ------------------------------------------------------------------------- */


void SocketClient::handle_kill() {
    // FIXME How to implement?
}


void SocketClient::handle_send_data(Message* msg) {
    string data = msg->getData() + "\r\n";
    int n = write(sock_fd_, data.c_str(), data.length());
    
    if(n < 0) {
        // FIXME handle errors better
        cout << "ERROR writing to socket" << endl;
        running_ = false;
    }
}


void SocketClient::handle_recieve_data(Message* msg) {
    string line;
    size_t end_pos;
    
    buffer_.append(msg->getData());
    
    // FIXME What if the client never sends \r\n ?!?!?!
    
    while((end_pos = buffer_.find("\r\n")) != string::npos) {
        line = buffer_.substr(0, end_pos);
        buffer_ = buffer_.substr(end_pos + 2);
        if(line.length())
            parse_incoming_line(&line);
    }
}


/* ------------------------------------------------------------------------- */
/* -- PARSE INCOMING DATA -------------------------------------------------- */
/* ------------------------------------------------------------------------- */


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


void SocketClient::handle_incoming_command(string cmd, string args) {
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


/* ------------------------------------------------------------------------- */
/* -- SOCKET READER -------------------------------------------------------- */
/* ------------------------------------------------------------------------- */


void SocketClient::SocketReader::run() {
    int sock_fd = client_->getSockFD();
    Message* msg;
    string data;
    char buf[SOCK_BUFFER_SIZE];
    int n;
    
    while(1) {
        n = read(sock_fd, buf, SOCK_BUFFER_SIZE-1);
        
        if (n > 0) {
            msg = new Message();
            msg->setData(string(buf, n));
            client_->send(E_RECV_DATA, msg);
        } else {
            cout << "ERROR reading from socket" << endl;
            client_->send(E_KILL);
            break;
        }
    }
}
