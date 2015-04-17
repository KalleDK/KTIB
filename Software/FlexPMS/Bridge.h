#pragma once
#include <cppconn/driver.h>
#include "MessageThread.h"
#include "KarContainer.h"


class Message;


class Bridge : public MessageThread {
public:
    Bridge(sql::Connection* db_conn, MessageThread* kar_bus) :
        kar_list_(db_conn),
        kar_bus_(kar_bus),
        manual_watering_(false),
        ovalve_open_(false),
        ivalve_open_(false) {};
private:
    MessageThread* kar_bus_;
    KarContainer kar_list_;
    
    bool manual_watering_;
    bool ovalve_open_;
    bool ivalve_open_;
    
    void dispatch(unsigned long event_id, Message* msg);
    void ping_kars();
    void handle_ready_cnf(KarBusMessage* msg);
    void handle_set_ph_level_cnf(KarBusMessage* msg);
    void handle_set_volumen_level_cnf(KarBusMessage* msg);
    void handle_set_soil_humidity_level_cnf(KarBusMessage* msg);
    void handle_get_kar_sensor_data_cnf(KarBusMessage* msg);
    
    void handle_start_watering(GuiMessage* msg);
    void handle_stop_watering(GuiMessage* msg);
    
    void handle_ovalve_open(GuiMessage* msg);
    void handle_ovalve_close(GuiMessage* msg);
    
    void handle_ivalve_open(GuiMessage* msg);
    void handle_ivalve_close(GuiMessage* msg);
};
