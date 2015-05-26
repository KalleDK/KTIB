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
    
    KarStatus status;
    
    Kar(sql::Connection* db_conn) :
        status(OFFLINE),
		db_conn_(db_conn) {};
    
    void set_mwstatus(bool s);
    void set_ivalvestatus(bool s);
    void set_ovalvestatus(bool s);
	void add_sensor_data(int type, double value);
private:
    sql::Connection* db_conn_;
};
