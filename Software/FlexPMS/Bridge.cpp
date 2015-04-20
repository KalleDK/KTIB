#include <iostream>
#include "Bridge.h"


#define PING_TIME_SEC 10
#define PING_COUNT_TIMEOUT 2


using namespace std;


void Bridge::dispatch(unsigned long event_id, Message* msg) {
    switch(event_id) {
        
        // -- EVENTS FROM KarPinger ---------------------------------------- //
        
        case E_PING:
            //cout << "Bridge recieved: E_PING" << endl;
            handle_ping();
            break;
        
        // -- EVENTS FROM KarBus ------------------------------------------- //
        
        case E_KAR_READY_STATE:
            cout << "Bridge recieved: E_KAR_READY_STATE" << endl;
            handle_kar_ready_state(static_cast<MKarReadyState*>(msg));
            break;
        case E_KAR_OE_LIST:
            cout << "Bridge recieved: E_KAR_OE_LIST" << endl;
            handle_kar_oe_list(static_cast<MKarOeList*>(msg));
            break;
        case E_KAR_SENSOR_DATA:
            cout << "Bridge recieved: E_KAR_SENSOR_DATA" << endl;
            handle_kar_sensor_data(static_cast<MKarSensorData*>(msg));
            break;
        case E_KAR_VALVE_STATE:
            cout << "Bridge recieved: E_KAR_VALVE_STATE" << endl;
            handle_kar_valve_state(static_cast<MKarValveState*>(msg));
            break;
        case E_KAR_PUMP_STATE:
            cout << "Bridge recieved: E_KAR_PUMP_STATE" << endl;
            handle_kar_pump_state(static_cast<MKarPumpState*>(msg));
            break;
            
        case E_OE_VALVE_STATE:
            cout << "Bridge recieved: E_OE_VALVE_STATE" << endl;
            handle_oe_valve_state(static_cast<MOeValveState*>(msg));
            break;
        case E_OE_SENSOR_DATA:
            cout << "Bridge recieved: E_OE_SENSOR_DATA" << endl;
            handle_oe_sensor_data(static_cast<MOeSensorData*>(msg));
            break;
        case E_OE_SENSOR_TYPE:
            cout << "Bridge recieved: E_OE_SENSOR_TYPE" << endl;
            handle_oe_sensor_type(static_cast<MOeSensorType*>(msg));
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
        
        // -- OTHER EVENTS ------------------------------------------------- //
        
        default:
            cout << "Bridge recieved: unknown command: " << event_id << endl;
            break;
    }
}


/* ------------------------------------------------------------------------- */
/* -- EVENTS FROM KarPinger ------------------------------------------------ */
/* ------------------------------------------------------------------------- */


void Bridge::handle_ping() {
    Kar* kar;
    KarBusMessage* msg;
    time_t now = time(NULL);
    
    kar_list_.iter();
    
    while(kar = kar_list_.next()) {
        // Send PING command
        msg = new KarBusMessage(this, kar);
        msg->setData(to_string(0xA));
        //kar_bus_->send(E_PING_REQ, msg);
        
        // Register PING on Kar and handle unanswered PINGs
        kar->last_ping_sent = now;
        kar->unanswered_pings++;
        if(kar->unanswered_pings > PING_COUNT_TIMEOUT) {
            kar->status = OFFLINE;
        }
    }
}


/* ------------------------------------------------------------------------- */
/* -- EVENTS FROM KarBus --------------------------------------------------- */
/* ------------------------------------------------------------------------- */


void Bridge::handle_kar_ready_state(MKarReadyState* msg) {
    
}


void Bridge::handle_kar_oe_list(MKarOeList* msg) {
    
}


void Bridge::handle_kar_sensor_data(MKarSensorData* msg) {
    
}


void Bridge::handle_kar_valve_state(MKarValveState* msg) {
    
}


void Bridge::handle_kar_pump_state(MKarPumpState* msg) {
    
}


void Bridge::handle_oe_valve_state(MOeValveState* msg) {
    
}


void Bridge::handle_oe_sensor_data(MOeSensorData* msg) {
    
}


void Bridge::handle_oe_sensor_type(MOeSensorType* msg) {
    
}


/* ------------------------------------------------------------------------- */
/* -- EVENTS FROM Gui ------------------------------------------------------ */
/* ------------------------------------------------------------------------- */


void Bridge::handle_start_watering(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_start_watering() got an unknown KarID: " << msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    // FOR EACH OE - SET VALVE STATE (open)
    MOeSetValveState* vmsg = new MOeSetValveState(this, kar);
    vmsg->oe_id = 4;
    vmsg->state = MOeSetValveState::OPEN;
    kar_bus_->send(E_OE_SET_VALVE_STATE, vmsg);
    
    // SET PUMP STATE ON KAR
    MKarSetPumpState* pmsg = new MKarSetPumpState(this, kar);
    pmsg->state = MKarSetPumpState::MIDDLE;
    kar_bus_->send(E_KAR_SET_PUMP_STATE, pmsg);
    
    kar->set_mwstatus(true);
}


void Bridge::handle_stop_watering(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_start_watering() got an unknown KarID: " << msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    // FOR EACH OE - SET VALVE STATE (closed)
    MOeSetValveState* vmsg = new MOeSetValveState(this, kar);
    vmsg->oe_id = 4;
    vmsg->state = MOeSetValveState::CLOSED;
    kar_bus_->send(E_OE_SET_VALVE_STATE, vmsg);
    
    // SET PUMP STATE ON KAR
    MKarSetPumpState* pmsg = new MKarSetPumpState(this, kar);
    pmsg->state = MKarSetPumpState::OFF;
    kar_bus_->send(E_KAR_SET_PUMP_STATE, pmsg);
    
    kar->set_mwstatus(false);
}


/* -- INTAKE VALVE --------------------------------------------------------- */


void Bridge::handle_ivalve_open(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ivalve_open() got an unknown KarID: " << msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MKarSetValveState* kmsg = new MKarSetValveState(this, NULL);
    kmsg->valve = MKarSetValveState::INTAKE;
    kmsg->state = MKarSetValveState::OPEN;
    kar_bus_->send(E_KAR_SET_VALVE_STATE, kmsg);
    
    kar->set_ivalvestatus(true);
}


void Bridge::handle_ivalve_close(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ivalve_close() got an unknown KarID: " << msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MKarSetValveState* kmsg = new MKarSetValveState(this, NULL);
    kmsg->valve = MKarSetValveState::INTAKE;
    kmsg->state = MKarSetValveState::CLOSED;
    kar_bus_->send(E_KAR_SET_VALVE_STATE, kmsg);
    
    kar->set_ivalvestatus(false);
}


/* -- OUTTAKE VALVE -------------------------------------------------------- */


void Bridge::handle_ovalve_open(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_open() got an unknown KarID: " << msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MKarSetValveState* kmsg = new MKarSetValveState(this, NULL);
    kmsg->valve = MKarSetValveState::OUTTAKE;
    kmsg->state = MKarSetValveState::OPEN;
    kar_bus_->send(E_KAR_SET_VALVE_STATE, kmsg);
    
    kar->set_ovalvestatus(true);
}


void Bridge::handle_ovalve_close(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_close() got an unknown KarID: " << msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MKarSetValveState* kmsg = new MKarSetValveState(this, NULL);
    kmsg->valve = MKarSetValveState::OUTTAKE;
    kmsg->state = MKarSetValveState::CLOSED;
    kar_bus_->send(E_KAR_SET_VALVE_STATE, kmsg);
    
    kar->set_ovalvestatus(false);
}


/* ------------------------------------------------------------------------- */
/* -- KAR PINGER ----------------------------------------------------------- */
/* ------------------------------------------------------------------------- */


void Bridge::KarPinger::run() {
    while(1) {
        bridge_->send(E_PING);
        ssleep(PING_TIME_SEC);
    }
}
