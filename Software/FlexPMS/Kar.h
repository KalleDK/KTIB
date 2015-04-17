#pragma once
#include <string>
#include <time.h>
#include <cppconn/driver.h>


typedef enum {
    ONLINE,
    OFFLINE,
} KarStatus;


class Kar {
public:
    unsigned int id;
    std::string name;
    unsigned int address;
    double ph;
    unsigned int volumen;
    unsigned int humidity;
    
    unsigned int unanswered_pings;
    time_t last_ping_sent;
    KarStatus status;
    
    Kar(sql::Connection* db_conn) : db_conn_(db_conn) {
        mwstatus_ = false;
        ivalvestatus_ = false;
        ovalvestatus_ = false;
        unanswered_pings = 0;
        last_ping_sent = 0;
        status = OFFLINE;
    }
    
    void set_mwstatus(bool mwstatus);
    bool get_mwstatus();
    void set_ovalvestatus(bool ovalvestatus);
    bool get_ovalvestatus();
    void set_ivalvestatus(bool ivalvestatus);
    bool get_ivalvestatus();
private:
    sql::Connection* db_conn_;
    bool mwstatus_;
    bool ivalvestatus_;
    bool ovalvestatus_;
};
