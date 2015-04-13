#include <iostream>
#include "Bridge.h"
#include "Message.h"
#include "events.h"


#define PING_TIME 10
#define PING_COUNT_TIMEOUT 2


using namespace std;


void Bridge::run() {
    long event_id = -1;
    Message* msg;
    
    while(1) {
        msg = queue_.recieve_nowait(event_id);
        
        if(event_id != -1)
            dispatch(event_id, msg);
        if(msg != NULL)
            delete msg;
        
        //ping_kars();
        wait(1);
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


void Bridge::dispatch(long event_id, Message* msg) {
    switch(event_id) {
        
        // -- EVENTS FROM KarBus ----------------------------------------- //
        
        case E_READY_CNF:
            cout << "Recieved E_READY_CNF" << endl;
            handle_ready_cnf(msg);
            break;
        case E_PING_CNF:
            cout << "Recieved E_PING_CNF" << endl;
            handle_ready_cnf(msg);
            break;
        case E_SET_PH_LEVEL_CNF:
            cout << "Recieved E_SET_PH_LEVEL_CNF" << endl;
            handle_set_ph_level_cnf(msg);
            break;
        case E_SET_VOLUMEN_LEVEL_CNF:
            cout << "Recieved E_SET_VOLUMEN_LEVEL_CNF" << endl;
            handle_set_volumen_level_cnf(msg);
            break;
        case E_SET_SOIL_HUMIDITY_LEVEL_CNF:
            cout << "Recieved E_SET_SOIL_HUMIDITY_LEVEL_CNF" << endl;
            handle_set_soil_humidity_level_cnf(msg);
            break;
        case E_GET_KAR_SENSOR_DATA_CNF:
            cout << "Recieved E_GET_KAR_SENSOR_DATA_CNF" << endl;
            handle_get_kar_sensor_data_cnf(msg);
            break;
        
        // -- EVENTS FROM KarBus ----------------------------------------- //
        
        case E_START_WATERING:
            cout << "Recieved E_START_WATERING" << endl;
            handle_start_watering(msg);
            break;
        case E_STOP_WATERING:
            cout << "Recieved E_STOP_WATERING" << endl;
            handle_stop_watering(msg);
            break;
        case E_WATERING_STATUS:
            cout << "Recieved E_WATERING_STATUS" << endl;
            handle_watering_status(msg);
            break;
        
        // -- OTHER EVENTS ----------------------------------------------- //
        
        case E_QUIT:
            cout << "Recieved E_QUIT" << endl;
            break;
        default:
            cout << "Recieved unknown command: " << event_id << endl;
            break;
    }
}


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
    GuiMessage* response = new GuiMessage(this, 5);
    response->setData("Vanding startet!");
    msg->sender->send(E_IS_WATERING, response);
}


void Bridge::handle_stop_watering(Message* msg) {
    GuiMessage* response = new GuiMessage(this, 5);
    response->setData("Vanding stoppet!");
    msg->sender->send(E_IS_WATERING, response);
}


void Bridge::handle_watering_status(Message* msg) {
    GuiMessage* response = new GuiMessage(this, 5);
    response->setData("Her er vanding status!");
    msg->sender->send(E_IS_WATERING, response);
}
