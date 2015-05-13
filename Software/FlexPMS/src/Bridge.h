#pragma once
#include <map>
#include <cppconn/driver.h>
#include "Thread.h"
#include "MessageThread.h"
#include "KarContainer.h"
#include "SensorOeContainer.h"
#include "Message.h"
#include "events.h"


class Message;


class Bridge : public MessageThread {
public:
    Bridge(sql::Connection* db_conn, MessageThread* kar_bus) :
        kar_bus_(kar_bus),
        kar_list_(db_conn),
        oe_list_(db_conn),
        last_session_id_(0),
        pinger_(this) {
            pinger_.start();
            kar_list_.reload();
            oe_list_.reload();
        };

private:
    
    class KarPinger : public Thread {
    public:
        KarPinger(Bridge* b) : bridge_(b) {};
        void run();
    private:
        Bridge* bridge_;
    };
    
    // Members
    MessageThread* kar_bus_;
    KarContainer kar_list_;
    SensorOeContainer oe_list_;
	unsigned long last_session_id_;
    KarPinger pinger_;
    std::map<unsigned long, MessageThread*> sessions_;

    
    // Methods
    void dispatch(unsigned long event_id, Message* msg);
    
    // Eventhandlers for messages from KarPinger
    void handle_ping();
    
    // Eventhandlers for messages from KarBus
    void handle_kar_ready_state(MKarReadyState* msg);
    void handle_kar_oe_list(MKarOeList* msg);
    void handle_kar_sensor_data(MKarSensorData* msg);
    void handle_kar_valve_state(MKarValveState* msg);
    void handle_kar_pump_state(MKarPumpState* msg);
    void handle_oe_valve_state(MOeValveState* msg);
    void handle_oe_sensor_data(MOeSensorData* msg);
    void handle_oe_sensor_type(MOeSensorType* msg);
    
    // Eventhandlers for handling SocketClient sessions
    void handle_hello(Message* msg);
    void handle_bye(SessionMessage* msg);
    
    // Eventhandlers for messages from Gui
    void handle_start_watering(GuiMessage* msg);
    void handle_stop_watering(GuiMessage* msg);
    void handle_ovalve_open(GuiMessage* msg);
    void handle_ovalve_close(GuiMessage* msg);
    void handle_ivalve_open(GuiMessage* msg);
    void handle_ivalve_close(GuiMessage* msg);
    void handle_oe_read(GuiMessage* msg);
    void handle_kar_read(GuiMessage* msg);
    void handle_ready_read(GuiMessage* msg);
    void handle_oe_list_read(GuiMessage* msg);	
	void handle_sensor_type_read(GuiMessage* msg);	

};

