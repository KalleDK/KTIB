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
            handle_ready_cnf(static_cast<KarBusMessage*>(msg));
            break;
        case E_PING_CNF:
            cout << "Bridge recieved: E_PING_CNF" << endl;
            handle_ready_cnf(static_cast<KarBusMessage*>(msg));
            break;
        case E_SET_PH_LEVEL_CNF:
            cout << "Bridge recieved: E_SET_PH_LEVEL_CNF" << endl;
            handle_set_ph_level_cnf(static_cast<KarBusMessage*>(msg));
            break;
        case E_SET_VOLUMEN_LEVEL_CNF:
            cout << "Bridge recieved: E_SET_VOLUMEN_LEVEL_CNF" << endl;
            handle_set_volumen_level_cnf(static_cast<KarBusMessage*>(msg));
            break;
        case E_SET_SOIL_HUMIDITY_LEVEL_CNF:
            cout << "Bridge recieved: E_SET_SOIL_HUMIDITY_LEVEL_CNF" << endl;
            handle_set_soil_humidity_level_cnf(static_cast<KarBusMessage*>(msg));
            break;
        case E_GET_KAR_SENSOR_DATA_CNF:
            cout << "Bridge recieved: E_GET_KAR_SENSOR_DATA_CNF" << endl;
            handle_get_kar_sensor_data_cnf(static_cast<KarBusMessage*>(msg));
            break;
        
        // -- EVENTS FROM Gui ---------------------------------------------- //
        
        case E_START_WATERING:
            cout << "Bridge recieved: E_START_WATERING" << endl;
            handle_start_watering(static_cast<GuiMessage*>(msg));
            break;
        case E_STOP_WATERING:
            cout << "Bridge recieved: E_STOP_WATERING" << endl;
            handle_stop_watering(static_cast<GuiMessage*>(msg));
            break;
        
        case E_OVALVE_OPEN:
            cout << "Bridge recieved: E_OVALVE_OPEN" << endl;
            handle_ovalve_open(static_cast<GuiMessage*>(msg));
            break;
        case E_OVALVE_CLOSE:
            cout << "Bridge recieved: E_OVALVE_CLOSE" << endl;
            handle_ovalve_close(static_cast<GuiMessage*>(msg));
            break;
        
        case E_IVALVE_OPEN:
            cout << "Bridge recieved: E_IVALVE_OPEN" << endl;
            handle_ivalve_open(static_cast<GuiMessage*>(msg));
            break;
        case E_IVALVE_CLOSE:
            cout << "Bridge recieved: E_IVALVE_CLOSE" << endl;
            handle_ivalve_close(static_cast<GuiMessage*>(msg));
            break;
        
        // -- OTHER EVENTS ----------------------------------------------- //
        
        //case E_QUIT:
        //    cout << "Bridge recieved: E_QUIT" << endl;
        //    running_ = false;
        //    break;
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


void Bridge::handle_ready_cnf(KarBusMessage* msg) {
    
}


void Bridge::handle_set_ph_level_cnf(KarBusMessage* msg) {
    
}


void Bridge::handle_set_volumen_level_cnf(KarBusMessage* msg) {
    
}


void Bridge::handle_set_soil_humidity_level_cnf(KarBusMessage* msg) {
    
}


void Bridge::handle_get_kar_sensor_data_cnf(KarBusMessage* msg) {
    
}


/* -- MANUAL WATERING ------------------------------------------------------ */


void Bridge::handle_start_watering(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar != NULL && !kar->mwstatus) {
	cout << "SENDER!" << endl;
        kar->set_mwstatus(true);
        
        KarBusMessage* kmsg = new KarBusMessage(this, NULL);
        kar_bus_->send(REQ_KAR_SENSOR_DATA, kmsg);
    }
}


void Bridge::handle_stop_watering(GuiMessage* msg) {
}


/* -- INTAKE VALVE --------------------------------------------------------- */


void Bridge::handle_ivalve_open(GuiMessage* msg) {
    char data[] = { 1, 1 };
    
    KarBusMessage* kmsg = new KarBusMessage(this, NULL);
    kmsg->setData(data, 2);
    kar_bus_->send(REQ_KAR_VENTIL, kmsg);
}


void Bridge::handle_ivalve_close(GuiMessage* msg) {
    char data[] = { 1, 0 };
    
    KarBusMessage* kmsg = new KarBusMessage(this, NULL);
    kmsg->setData(data, 2);
    kar_bus_->send(REQ_KAR_VENTIL, kmsg);
}


/* -- OUTTAKE VALVE -------------------------------------------------------- */


void Bridge::handle_ovalve_open(GuiMessage* msg) {
    char data[] = { 2, 1 };
    
    KarBusMessage* kmsg = new KarBusMessage(this, NULL);
    kmsg->setData(data, 2);
    kar_bus_->send(REQ_KAR_VENTIL, kmsg);
}


void Bridge::handle_ovalve_close(GuiMessage* msg) {
    char data[] = { 2, 0 };
    
    KarBusMessage* kmsg = new KarBusMessage(this, NULL);
    kmsg->setData(data, 2);
    kar_bus_->send(REQ_KAR_VENTIL, kmsg);
}
