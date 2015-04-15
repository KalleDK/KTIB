#include <iostream>
#include "Bridge.h"
#include "Message.h"
#include "events.h"


#define PING_TIME 10
#define PING_COUNT_TIMEOUT 2


using namespace std;


void Bridge::dispatch(unsigned long event_id, Message* msg) {
    switch(event_id) {
        
        // -- EVENTS FROM KarBus ----------------------------------------- //
        
        case E_READY_CNF:
            cout << "Bridge recieved: E_READY_CNF" << endl;
            handle_ready_cnf(msg);
            break;
        case E_PING_CNF:
            cout << "Bridge recieved: E_PING_CNF" << endl;
            handle_ready_cnf(msg);
            break;
        case E_SET_PH_LEVEL_CNF:
            cout << "Bridge recieved: E_SET_PH_LEVEL_CNF" << endl;
            handle_set_ph_level_cnf(msg);
            break;
        case E_SET_VOLUMEN_LEVEL_CNF:
            cout << "Bridge recieved: E_SET_VOLUMEN_LEVEL_CNF" << endl;
            handle_set_volumen_level_cnf(msg);
            break;
        case E_SET_SOIL_HUMIDITY_LEVEL_CNF:
            cout << "Bridge recieved: E_SET_SOIL_HUMIDITY_LEVEL_CNF" << endl;
            handle_set_soil_humidity_level_cnf(msg);
            break;
        case E_GET_KAR_SENSOR_DATA_CNF:
            cout << "Bridge recieved: E_GET_KAR_SENSOR_DATA_CNF" << endl;
            handle_get_kar_sensor_data_cnf(msg);
            break;
        
        // -- EVENTS FROM KarBus ----------------------------------------- //
        
        case E_START_WATERING:
            cout << "Bridge recieved: E_START_WATERING" << endl;
            handle_start_watering(msg);
            break;
        case E_STOP_WATERING:
            cout << "Bridge recieved: E_STOP_WATERING" << endl;
            handle_stop_watering(msg);
            break;
        case E_WATERING_STATUS:
            cout << "Bridge recieved: E_WATERING_STATUS" << endl;
            handle_watering_status(msg);
            break;
        
        // -- OTHER EVENTS ----------------------------------------------- //
        
        case E_QUIT:
            cout << "Bridge recieved: E_QUIT" << endl;
            running_ = false;
            break;
        default:
            cout << "Bridge recieved: unknown command: " << event_id << endl;
            break;
    }
}


void Bridge::ping_kars() {
    Kar* kar;
    KarBusMessage* msg;
    time_t now = time(NULL);
    
    kar_list_.iter();
    
    while(kar = kar_list_.next()) {
        if(difftime(now, kar->last_ping_sent) >= PING_TIME) {
            // Send PING command
            msg = new KarBusMessage(this, kar);
            msg->setData(to_string(0xA));
            kar_bus_->send(E_PING_REQ, msg);
            
            // Register PING on Kar and handle unanswered PINGs
            kar->last_ping_sent = now;
            kar->unanswered_pings++;
            if(kar->unanswered_pings > PING_COUNT_TIMEOUT) {
                kar->status = OFFLINE;
            }
        }
    }
}


/* ------------------------------------------------------------------------- */
/* -- EVENT HANDLERS ------------------------------------------------------- */
/* ------------------------------------------------------------------------- */


void Bridge::handle_ready_cnf(Message* msg) {
    
}


void Bridge::handle_set_ph_level_cnf(Message* msg) {
    
}


void Bridge::handle_set_volumen_level_cnf(Message* msg) {
    
}


void Bridge::handle_set_soil_humidity_level_cnf(Message* msg) {
    
}


void Bridge::handle_get_kar_sensor_data_cnf(Message* msg) {
    
}


void Bridge::handle_start_watering(Message* msg) {
    manual_watering_ = true;
}


void Bridge::handle_stop_watering(Message* msg) {
    manual_watering_ = false;
}


void Bridge::handle_watering_status(Message* msg) {
    string resp = (manual_watering_) ? "MWSTATUS 1" : "MWSTATUS 0";
    
    GuiMessage* response = new GuiMessage(this, 5);
    response->setData(resp);
    msg->sender->send(E_SEND_DATA, response);
}
