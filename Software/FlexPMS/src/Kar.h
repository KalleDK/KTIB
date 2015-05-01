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
    bool mwstatus;
    bool ivalvestatus;
    bool ovalvestatus;
    
    unsigned int unanswered_pings;
    time_t last_ping_sent;
    KarStatus status;
    
    Kar(sql::Connection* db_conn) : 
        db_conn_(db_conn),
        unanswered_pings(0),
        last_ping_sent(0), 
        status(OFFLINE) {};
    
    void set_mwstatus(bool s);
    void set_ivalvestatus(bool s);
    void set_ovalvestatus(bool s);
    //void add_sensor_data();
private:
    sql::Connection* db_conn_;
};
