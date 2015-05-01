#pragma once
#include <string>
#include <cppconn/driver.h>


class SensorOe {
public:
    typedef enum {
        HUMID = 4,
    } SensorType;
    
    unsigned int id;
    unsigned int kar_id;
    unsigned int address;
    bool valvestatus;
    
    SensorOe(sql::Connection* db_conn) : db_conn_(db_conn) {};
    void set_valvestatus(bool s);
    void add_sensor_data(SensorType type, double value);
private:
    sql::Connection* db_conn_;
};
