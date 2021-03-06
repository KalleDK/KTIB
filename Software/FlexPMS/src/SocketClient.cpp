#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include "SocketClient.h"
#include "Message.h"
#include "events.h"


using namespace std;


void SocketClient::init() {
    bridge_->send(E_HELLO, new GuiMessage(this));
}


void SocketClient::shutdown() {
    running_ = false;
    reader_.cancel();
    reader_.join();
    close(sock_fd_);
    cancel();
}


void SocketClient::dispatch(unsigned long event_id, Message* msg) {
    switch(event_id) {
        case E_START_SESSION:
            cout << "SocketClient recieved: E_START_SESSION" << endl;
            handle_start_session(static_cast<SessionMessage*>(msg));
            break;
        case E_STOP_SESSION:
            cout << "SocketClient recieved: E_STOP_SESSION" << endl;
            handle_stop_session();
            break;
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


void SocketClient::handle_start_session(SessionMessage* msg) {
    session_id_ = msg->session_id;
    reader_.start();
}


void SocketClient::handle_stop_session() {
    shutdown();
}


void SocketClient::handle_kill() {
    SessionMessage* msg = new SessionMessage(this);
    msg->session_id = session_id_;
    
    bridge_->send(E_BYE, msg);
    shutdown();
}


void SocketClient::handle_send_data(Message* msg) {
    string data = msg->getData() + "\r\n";
    int n = write(sock_fd_, data.c_str(), data.length());
    
    if(n < 0) {
        SessionMessage* msg = new SessionMessage(this);
        msg->session_id = session_id_;
        
        bridge_->send(E_BYE, msg);
        shutdown();
    }
}


void SocketClient::handle_recieve_data(Message* msg) {
    buffer_.append(msg->getData());
    parse_data_buffer();
}


/* ------------------------------------------------------------------------- */
/* -- PARSE INCOMING DATA -------------------------------------------------- */
/* ------------------------------------------------------------------------- */


void SocketClient::parse_data_buffer() {
    string line;
    size_t end_pos;
    
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
    
    // FIXME What if line starts with a space-character?
    // Then cmd is empty and args are assigned the rest of the string.
    
    if(pos != string::npos) {
        cmd = line->substr(0, pos);
        args = line->substr(pos + 1);
    } else {
        cmd = *line;
    }
    
    handle_incoming_command(cmd, args);
}


void SocketClient::handle_incoming_command(string cmd, string args) {
    if(cmd == "MWSTART") {
        handle_start_watering(args);
    } else if(cmd == "MWSTOP") {
        handle_stop_watering(args);
    } else if(cmd == "IVALVEOPEN") {
        handle_ivalve_open(args);
    } else if(cmd == "IVALVECLOSE") {
        handle_ivalve_close(args);
    } else if(cmd == "OVALVEOPEN") {
        handle_ovalve_open(args);
    } else if(cmd == "OVALVECLOSE") {
#ifdef DEBUG
        handle_ovalve_close(args);
    } else if(cmd == "OEREAD") {
        handle_oe_sensor_read(args);
    } else if(cmd == "KARREAD") {
        handle_kar_sensor_read(args);
    } else if(cmd == "RDYREAD") {
        handle_kar_ready_read(args);
    } else if(cmd == "OELIST") {
        handle_oe_list_read(args);
    } else if(cmd == "SENSORTYPE") {
        handle_sensor_type_read(args);
#endif
    } else {
        cout << "SocketClient recieved invalid command: " << cmd << "(args: " << args << ")" << endl;
    }
}


/* -- MANUAL WATERING ------------------------------------------------------ */


void SocketClient::handle_start_watering(string args) {
    int kar_id = atoi(args.c_str());
    if(kar_id != 0) {
        GuiMessage* msg = new GuiMessage(this);
        msg->kar_id = kar_id;
        msg->session_id = session_id_;
        bridge_->send(E_START_WATERING, msg);
    } else {
        cout << "SocketClient ignoring MWSTART. Invalid KarID given: " << args << endl;
    }
}


void SocketClient::handle_stop_watering(string args) {
    int kar_id = atoi(args.c_str());
    if(kar_id != 0) {
        GuiMessage* msg = new GuiMessage(this);
        msg->kar_id = kar_id;
        msg->session_id = session_id_;
        bridge_->send(E_STOP_WATERING, msg);
    } else {
        cout << "SocketClient ignoring MWSTOP. Invalid KarID given: " << args << endl;
    }
}


/* -- INTAKE VALVE --------------------------------------------------------- */


void SocketClient::handle_ivalve_open(string args) {
    int kar_id = atoi(args.c_str());
    if(kar_id != 0) {
        GuiMessage* msg = new GuiMessage(this);
        msg->kar_id = kar_id;
        msg->session_id = session_id_;
        bridge_->send(E_IVALVE_OPEN, msg);
    } else {
        cout << "SocketClient ignoring IVALVEOPEN. Invalid KarID given: " << args << endl;
    }
}


void SocketClient::handle_ivalve_close(string args) {
    int kar_id = atoi(args.c_str());
    if(kar_id != 0) {
        GuiMessage* msg = new GuiMessage(this);
        msg->kar_id = kar_id;
        msg->session_id = session_id_;
        bridge_->send(E_IVALVE_CLOSE, msg);
    } else {
        cout << "SocketClient ignoring IVALVECLOSE. Invalid KarID given: " << args << endl;
    }
}


/* -- OUTTAKE VALVE -------------------------------------------------------- */


void SocketClient::handle_ovalve_open(string args) {
    int kar_id = atoi(args.c_str());
    if(kar_id != 0) {
        GuiMessage* msg = new GuiMessage(this);
        msg->kar_id = kar_id;
        msg->session_id = session_id_;
        bridge_->send(E_OVALVE_OPEN, msg);
    } else {
        cout << "SocketClient ignoring OVALVEOPEN. Invalid KarID given: " << args << endl;
    }
}


void SocketClient::handle_ovalve_close(string args) {
    int kar_id = atoi(args.c_str());
    if(kar_id != 0) {
        GuiMessage* msg = new GuiMessage(this);
        msg->kar_id = kar_id;
        msg->session_id = session_id_;
        bridge_->send(E_OVALVE_CLOSE, msg);
    } else {
        cout << "SocketClient ignoring OVALVECLOSE. Invalid KarID given: " << args << endl;
    }
}


/* -- ADD/REMOVE SENSOR OE ------------------------------------------------- */


void SocketClient::handle_add_sensor_oe(string args) {
    size_t space = args.find(' ');
    
    if(space < 1) {
        SessionMessage* msg = new SessionMessage(this);
        msg->session_id = session_id_;
        
        cout << "SocketClient ignoring ADDSENSOROE. Invalid arguments: " << args << endl;
        bridge_->send(E_BYE, msg);
        shutdown();
        return;
    }
        
    string kar_id_str = args.substr(0, space);
    string oe_id_str = args.substr(space+1);
    
    int kar_id = atoi(kar_id_str.c_str());
    int oe_id = atoi(oe_id_str.c_str());
    
    if(kar_id != 0) {
        GuiMessage* msg = new GuiMessage(this);
        msg->kar_id = kar_id;
        msg->oe_id = oe_id;
        msg->session_id = session_id_;
        bridge_->send(E_OVALVE_OPEN, msg);
    } else {
        cout << "SocketClient ignoring OVALVEOPEN. Invalid KarID given: " << args << endl;
    }
}


/* -- DEBUG METHODS -------------------------------------------------------- */


#ifdef DEBUG


void SocketClient::handle_oe_sensor_read(string args) {
    int kar_id = atoi(args.c_str());
    if(kar_id != 0) {
        GuiMessage* msg = new GuiMessage(this);
        msg->kar_id = kar_id;
        msg->session_id = session_id_;
        bridge_->send(E_OE_SENSOR, msg);
    } else {
        cout << "SocketClient ignoring OEREAD. Invalid KarID given: " << args << endl;
    }
}


void SocketClient::handle_kar_sensor_read(string args) {
    int kar_id = atoi(args.c_str());
    if(kar_id != 0) {
        GuiMessage* msg = new GuiMessage(this);
        msg->kar_id = kar_id;
        msg->session_id = session_id_;
        bridge_->send(E_KAR_SENSOR, msg);
    } else {
        cout << "SocketClient ignoring KARREAD. Invalid KarID given: " << args << endl;
    }
}

void SocketClient::handle_kar_ready_read(string args) {
    int kar_id = atoi(args.c_str());
    if(kar_id != 0) {
        GuiMessage* msg = new GuiMessage(this);
        msg->kar_id = kar_id;
        msg->session_id = session_id_;
        bridge_->send(E_RDY_REQ, msg);
    } else {
        cout << "SocketClient ignoring READYREAD. Invalid KarID given: " << args << endl;
    }
}

void SocketClient::handle_oe_list_read(string args) {
    int kar_id = atoi(args.c_str());
    if(kar_id != 0) {
        GuiMessage* msg = new GuiMessage(this);
        msg->kar_id = kar_id;
        msg->session_id = session_id_;
        bridge_->send(E_OE_LIST, msg);
    } else {
        cout << "SocketClient ignoring OELIST. Invalid KarID given: " << args << endl;
    }
}

void SocketClient::handle_sensor_type_read(string args) {
    int kar_id = atoi(args.c_str());
    if(kar_id != 0) {
        GuiMessage* msg = new GuiMessage(this);
        msg->kar_id = kar_id;
        msg->session_id = session_id_;
        bridge_->send(E_SENSOR_TYPE, msg);
    } else {
        cout << "SocketClient ignoring SENSORTYPE. Invalid KarID given: " << args << endl;
    }
}

#endif


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
            cout << "SocketReader: ERROR reading from socket, closing connection" << endl;
            client_->send(E_KILL);
            break;
        }
    }
}
