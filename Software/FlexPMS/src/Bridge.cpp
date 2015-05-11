#include <iostream>
#include "Bridge.h"


#define PING_TIME_SEC 10
#define PING_COUNT_TIMEOUT 2


using namespace std;


void Bridge::dispatch(unsigned long event_id, Message* msg) {
    switch(event_id) {
        
        // -- EVENTS FROM KarPinger ---------------------------------------- //
        
        case E_PING:
            cout << "Bridge recieved: E_PING" << endl;
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
            
        case E_HELLO:
            cout << "Bridge recieved: E_HELLO" << endl;
            handle_hello(msg);
            break;
        case E_BYE:
            cout << "Bridge recieved: E_BYE" << endl;
            handle_bye(static_cast<SessionMessage*>(msg));
            break;
        
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
			
        case E_OE_SENSOR:
            cout << "Bridge recieved: E_OE_SENSOR" << endl;
            handle_oe_read(static_cast<GuiMessage*>(msg));
            break;
        case E_KAR_SENSOR:
            cout << "Bridge recieved: E_KAR_SENSOR" << endl;
            handle_kar_read(static_cast<GuiMessage*>(msg));
            break;
        case E_RDY_REQ:
            cout << "Bridge recieved: E_RDY_REQ" << endl;
            handle_ready_read(static_cast<GuiMessage*>(msg));
            break;
        case E_OE_LIST:
            cout << "Bridge recieved: E_OE_LIST" << endl;
            handle_oe_list_read(static_cast<GuiMessage*>(msg));
            break;
        case E_SENSOR_TYPE:
            cout << "Bridge recieved: E_SENSOR_TYPE" << endl;
            handle_sensor_type_read(static_cast<GuiMessage*>(msg));
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
    SensorOe* oe;
    
    kar_list_.iter();
    while(kar = kar_list_.next()) {
        MKarGetSensorData* msg1 = new MKarGetSensorData(this, kar);
        kar_bus_->send(E_KAR_GET_SENSOR_DATA, msg1);
        
        oe_list_.iter();
        while(oe = oe_list_.next()) {
            if(oe->kar_id == kar->id) {
                MOeGetSensorData* msg2 = new MOeGetSensorData(this, kar);
                msg2->oe_id = oe->id;
                kar_bus_->send(E_OE_GET_SENSOR_DATA, msg2);
            }
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
    std::vector<MKarSensorData::KarSensorData>::iterator it;
    
    for(it = msg->sensor_data.begin(); it != msg->sensor_data.end(); ++it) {
        it->sensor_id;
        it->value;
    }
}


void Bridge::handle_kar_valve_state(MKarValveState* msg) {
    Kar* kar = msg->kar;
    bool status = (msg->state == MKarValveState::OPEN);
    
    if(msg->valve == MKarValveState::INTAKE)
        msg->kar->set_ivalvestatus(status);
    else if(msg->valve == MKarValveState::OUTTAKE)
        msg->kar->set_ovalvestatus(status);
}


void Bridge::handle_kar_pump_state(MKarPumpState* msg) {
    
}


void Bridge::handle_oe_valve_state(MOeValveState* msg) {
    
}


void Bridge::handle_oe_sensor_data(MOeSensorData* msg) {
    SensorOe* oe;
    std::vector<MOeSensorData::OeSensorData>::iterator it;
    
    for(it = msg->sensor_data.begin(); it != msg->sensor_data.end(); ++it) {
        oe = oe_list_.get(it->oe_id);
        if(oe != NULL) {
            oe->add_sensor_data(it->sensor_id, it->value);
        } else {
            cout << "Bridge: handle_oe_sensor_data() got unknown OE ID: " << it->oe_id << endl;
        }
    }
}


void Bridge::handle_oe_sensor_type(MOeSensorType* msg) {
    
}


/* ------------------------------------------------------------------------- */
/* -- EVENTS FROM Gui ------------------------------------------------------ */
/* ------------------------------------------------------------------------- */


void Bridge::handle_hello(Message* msg) {
    while(sessions_.count(++last_session_id_));
    sessions_[last_session_id_] = msg->sender;
    
    SessionMessage* response = new SessionMessage(this);
    response->session_id = last_session_id_;
    msg->sender->send(E_START_SESSION, response);
}


void Bridge::handle_bye(SessionMessage* msg) {
    msg->sender->join();
    sessions_.erase(msg->session_id);
}


void Bridge::handle_start_watering(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    SensorOe* oe;
    
    if(kar == NULL) {
        cout << "Bridge: handle_start_watering() got an unknown KarID: " << msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    // FOR EACH OE - SET VALVE STATE (open)
    oe_list_.iter();
    while(oe = oe_list_.next()) {
        if(oe->kar_id == kar->id) {
            MOeSetValveState* vmsg = new MOeSetValveState(this, kar);
            vmsg->oe_id = oe->id;
            vmsg->state = MOeSetValveState::OPEN;
            kar_bus_->send(E_OE_SET_VALVE_STATE, vmsg);
        }
    }
    
    // SET PUMP STATE ON KAR
    MKarSetPumpState* pmsg = new MKarSetPumpState(this, kar);
    pmsg->state = MKarSetPumpState::MIDDLE;
    kar_bus_->send(E_KAR_SET_PUMP_STATE, pmsg);
    
    kar->set_mwstatus(true);
}


void Bridge::handle_stop_watering(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    SensorOe* oe;
    
    if(kar == NULL) {
        cout << "Bridge: handle_start_watering() got an unknown KarID: " << msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    // FOR EACH OE - SET VALVE STATE (closed)
    oe_list_.iter();
    while(oe = oe_list_.next()) {
        if(oe->kar_id == kar->id) {
            MOeSetValveState* vmsg = new MOeSetValveState(this, kar);
            vmsg->oe_id = oe->id;
            vmsg->state = MOeSetValveState::CLOSED;
            kar_bus_->send(E_OE_SET_VALVE_STATE, vmsg);
        }
    }
    
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
    
    MKarSetValveState* kmsg = new MKarSetValveState(this, kar);
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
    
    MKarSetValveState* kmsg = new MKarSetValveState(this, kar);
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
    
    MKarSetValveState* kmsg = new MKarSetValveState(this, kar);
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
    
    MKarSetValveState* kmsg = new MKarSetValveState(this, kar);
    kmsg->valve = MKarSetValveState::OUTTAKE;
    kmsg->state = MKarSetValveState::CLOSED;
    kar_bus_->send(E_KAR_SET_VALVE_STATE, kmsg);
    
    kar->set_ovalvestatus(false);
}


/* -- SESNOR READ -------------------------------------------------------- */

void Bridge::handle_oe_read(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_open() got an unknown KarID: " << msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MOeGetSensorData* kmsg = new MOeGetSensorData(this, kar);
    kmsg->oe_id = 0x4; 
    kar_bus_->send(E_OE_GET_SENSOR_DATA, kmsg);

}

void Bridge::handle_kar_read(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_close() got an unknown KarID: " << msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MKarSensorData* kmsg = new MKarSensorData(this, kar);
    kar_bus_->send(E_KAR_GET_SENSOR_DATA, kmsg);
    
}

void Bridge::handle_ready_read(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_close() got an unknown KarID: " << msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MKarReady* kmsg = new MKarReady(this, kar);
    kar_bus_->send(E_KAR_READY, kmsg);
    
}

void Bridge::handle_oe_list_read(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_close() got an unknown KarID: " << msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MKarGetOeList* kmsg = new MKarGetOeList(this, kar);
    kar_bus_->send(E_KAR_GET_OE_LIST, kmsg);
    
}

void Bridge::handle_sensor_type_read(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_close() got an unknown KarID: " << msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MOeGetSensorType* kmsg = new MOeGetSensorType(this, kar);
	kmsg->oe_id = 0x4;
	kmsg->sensor_id = 0x1;
    kar_bus_->send(E_OE_GET_SENSOR_TYPE, kmsg);
    
}

/* ------------------------------------------------------------------------- */
/* -- KAR PINGER ----------------------------------------------------------- */
/* ------------------------------------------------------------------------- */


void Bridge::KarPinger::run() {
    while(1) {
        //bridge_->send(E_PING);
        ssleep(PING_TIME_SEC);
    }
}
