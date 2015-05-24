#include <iostream>
#include <sstream>
#include "defines.h"
#include "Bridge.h"
#include "Log.h"


#define PING_TIME_SEC 10
#define PING_COUNT_TIMEOUT 2


using namespace std;


void Bridge::dispatch(unsigned long event_id, Message* msg) {
    
    switch(event_id) {
        
        // -- EVENTS FROM KarPinger ---------------------------------------- //
        
        case E_PING:
            cout << "Bridge: recieved event E_PING" << endl;
            handle_ping();
            break;
        
        // -- EVENTS FROM KarBus ------------------------------------------- //
        
        case E_KAR_READY_STATE:
            cout << "Bridge: recieved event E_KAR_READY_STATE" << endl;
            handle_kar_ready_state(static_cast<MKarReadyState*>(msg));
            break;
        case E_KAR_OE_LIST:
            cout << "Bridge: recieved event E_KAR_OE_LIST" << endl;
            handle_kar_oe_list(static_cast<MKarOeList*>(msg));
            break;
        case E_KAR_SENSOR_DATA:
            cout << "Bridge: recieved event E_KAR_SENSOR_DATA" << endl;
            handle_kar_sensor_data(static_cast<MKarSensorData*>(msg));
            break;
        case E_KAR_VALVE_STATE:
            cout << "Bridge: recieved event E_KAR_VALVE_STATE" << endl;
            handle_kar_valve_state(static_cast<MKarValveState*>(msg));
            break;
        case E_KAR_PUMP_STATE:
            cout << "Bridge: recieved event E_KAR_PUMP_STATE" << endl;
            handle_kar_pump_state(static_cast<MKarPumpState*>(msg));
            break;
            
        case E_OE_VALVE_STATE:
            cout << "Bridge: recieved event E_OE_VALVE_STATE" << endl;
            handle_oe_valve_state(static_cast<MOeValveState*>(msg));
            break;
        case E_OE_SENSOR_DATA:
            cout << "Bridge: recieved event E_OE_SENSOR_DATA" << endl;
            handle_oe_sensor_data(static_cast<MOeSensorData*>(msg));
            break;
        case E_OE_SENSOR_TYPE:
            cout << "Bridge: recieved event E_OE_SENSOR_TYPE" << endl;
            handle_oe_sensor_type(static_cast<MOeSensorType*>(msg));
            break;
        
        // -- EVENTS FROM Gui ---------------------------------------------- //
        
        case E_HELLO:
            cout << "Bridge: recieved event E_HELLO" << endl;
            handle_hello(msg);
            break;
        case E_BYE:
            cout << "Bridge: recieved event E_BYE" << endl;
            handle_bye(static_cast<SessionMessage*>(msg));
            break;
        
        case E_START_WATERING:
            cout << "Bridge: recieved event E_START_WATERING" << endl;
            handle_start_watering(static_cast<GuiMessage*>(msg));
            break;
        case E_STOP_WATERING:
            cout << "Bridge: recieved event E_STOP_WATERING" << endl;
            handle_stop_watering(static_cast<GuiMessage*>(msg));
            break;
        
        case E_OVALVE_OPEN:
            cout << "Bridge: recieved event E_OVALVE_OPEN" << endl;
            handle_ovalve_open(static_cast<GuiMessage*>(msg));
            break;
        case E_OVALVE_CLOSE:
            cout << "Bridge: recieved event E_OVALVE_CLOSE" << endl;
            handle_ovalve_close(static_cast<GuiMessage*>(msg));
            break;
        
        case E_IVALVE_OPEN:
            cout << "Bridge: recieved event E_IVALVE_OPEN" << endl;
            handle_ivalve_open(static_cast<GuiMessage*>(msg));
            break;
        case E_IVALVE_CLOSE:
            cout << "Bridge: recieved event E_IVALVE_CLOSE" << endl;
            handle_ivalve_close(static_cast<GuiMessage*>(msg));
            break;
			
#ifdef DEBUG
        case E_OE_SENSOR:
            cout << "Bridge: recieved event E_OE_SENSOR" << endl;
            handle_oe_read(static_cast<GuiMessage*>(msg));
            break;
        case E_KAR_SENSOR:
            cout << "Bridge: recieved event E_KAR_SENSOR" << endl;
            handle_kar_read(static_cast<GuiMessage*>(msg));
            break;
        case E_RDY_REQ:
            cout << "Bridge: recieved event E_RDY_REQ" << endl;
            handle_ready_read(static_cast<GuiMessage*>(msg));
            break;
        case E_OE_LIST:
            cout << "Bridge: recieved event E_OE_LIST" << endl;
            handle_oe_list_read(static_cast<GuiMessage*>(msg));
            break;
        case E_SENSOR_TYPE:
            cout << "Bridge: recieved event E_SENSOR_TYPE" << endl;
            handle_sensor_type_read(static_cast<GuiMessage*>(msg));
            break;
#endif
			
        
        // -- OTHER EVENTS ------------------------------------------------- //
        
        default:
            cout << "Bridge: recieved unknown event: " << event_id << endl;
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
    while((kar = kar_list_.next())) {
		if(kar->status == OFFLINE) {
			MKarReady* msg1 = new MKarReady(this, kar);
			kar_bus_->send(E_KAR_READY, msg1);
		} else {
			MKarGetSensorData* msg1 = new MKarGetSensorData(this, kar);
			kar_bus_->send(E_KAR_GET_SENSOR_DATA, msg1);
			
			oe_list_.iter();
			while((oe = oe_list_.next())) {
				if(oe->kar_id == kar->id) {
					MOeGetSensorData* msg2 = new MOeGetSensorData(this, kar);
					msg2->address = oe->address;
					kar_bus_->send(E_OE_GET_SENSOR_DATA, msg2);
				}
			}
		}
    }
}


/* ------------------------------------------------------------------------- */
/* -- EVENTS FROM KarBus --------------------------------------------------- */
/* ------------------------------------------------------------------------- */


void Bridge::handle_kar_ready_state(MKarReadyState* msg) {
    Kar* kar = msg->kar;
    SensorOe* oe;
	
	kar->status = ONLINE;
	
	oe_list_.iter();
	while((oe = oe_list_.next())) {
		if(oe->kar_id == kar->id) {
			MKarSetOpretOe* msg2 = new MKarSetOpretOe(this, kar);
			msg2->address = oe->address;
			kar_bus_->send(E_KAR_OPRET_OE, msg2);
		}
	}
}


void Bridge::handle_kar_oe_list(MKarOeList* msg) {
    Log* log = Log::getInstance();
    log->write("");
}


void Bridge::handle_kar_sensor_data(MKarSensorData* msg) {
    Log* log = Log::getInstance();
    stringstream ss;
    Kar* kar = msg->kar;
    std::vector<MKarSensorData::KarSensorData>::iterator it;
    
    for(it = msg->sensor_data.begin(); it != msg->sensor_data.end(); ++it) {
		kar->add_sensor_data(it->sensor_id, it->value);
        
        // Write to log
		ss.str("");
        ss << "Kar data [KarID: " << kar->id << ", Type: " << (int)it->sensor_id << "] Measured value: " << (int)it->value;
        log->write(ss.str());
    }
}


void Bridge::handle_kar_valve_state(MKarValveState* msg) {
    Log* log = Log::getInstance();
    stringstream ss;
    Kar* kar = msg->kar;
    bool status = (msg->state == MKarValveState::OPEN);
    string status_str = (status) ? "Open" : "Closed";
    
    if(msg->valve == MKarValveState::INTAKE) {
        msg->kar->set_ivalvestatus(status);
        ss << "Confirmed Kar Intake valve state [KarID: " << (int)kar->id << "] State: " << status_str;
        log->write(ss.str());
    } else if(msg->valve == MKarValveState::OUTTAKE) {
        msg->kar->set_ovalvestatus(status);
        ss << "Confirmed Kar Outtake valve state [KarID: " << (int)kar->id << "] State: " << status_str;
        log->write(ss.str());
    }
}


void Bridge::handle_kar_pump_state(MKarPumpState* msg) {
    Kar* kar = msg->kar;
    Log* log = Log::getInstance();
    stringstream ss;
    string pump_state_str;
    
    switch(msg->state) {
        case MKarPumpState::OFF:
            pump_state_str = "Off";
            break;
        case MKarPumpState::SLOW:
            pump_state_str = "Slow";
            break;
        case MKarPumpState::MIDDLE:
            pump_state_str = "Middle";
            break;
        case MKarPumpState::FAST:
            pump_state_str = "Fast";
            break;
        default:
            pump_state_str = "Unknown";
    }
    
    ss << "Confirmed Kar Pump state [KarID: " << (int)kar->id << "] State: " << pump_state_str;
    log->write(ss.str());
}


void Bridge::handle_oe_valve_state(MOeValveState* msg) {
    Log* log = Log::getInstance();
    stringstream ss;
    SensorOe* oe = oe_list_.get(msg->address);
	if(oe != NULL) {
		bool status = (msg->state == MOeValveState::OPEN);
		string status_str = (status) ? "Open" : "Closed";
		
		ss << "Confirmed OE valve state [KarID: " << (int)oe->kar_id << ", OeID: " << (int)oe->id << "] State: " << status_str;
		log->write(ss.str());
	}
}


void Bridge::handle_oe_sensor_data(MOeSensorData* msg) {
    Log* log = Log::getInstance();
    stringstream ss;
	Kar* kar;
    SensorOe* oe;
    std::vector<MOeSensorData::OeSensorData>::iterator it;
    
    for(it = msg->sensor_data.begin(); it != msg->sensor_data.end(); ++it) {
        oe = oe_list_.get(it->address);
		
        if(oe != NULL) {
            oe->add_sensor_data(it->sensor_id, it->value);
			
            // Write to log
            ss << "SensorOE data [KarID: " << oe->kar_id << ", OeID: " << oe->id << ", Type: " << (int)it->sensor_id << "] Measured value: " << it->value;
            log->write(ss.str());
			
			if((int)it->sensor_id == SENSOR_TYPE_MOISTURE) {
				kar = kar_list_.get(oe->kar_id);
				if(kar != NULL) {
					kar->add_sensor_data(it->sensor_id, it->value);
				}
			}
        } else {
            cout << "Bridge: handle_oe_sensor_data() got unknown OE address: " << (int)it->address << endl;
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
    
    cout << "Created client session (id: " << (int)last_session_id_ << ")" << endl;
}


void Bridge::handle_bye(SessionMessage* msg) {
    msg->sender->join();
    sessions_.erase(msg->session_id);
    
    cout << "Closed client session (id: " << (int)msg->session_id << ")";
}


void Bridge::handle_start_watering(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    SensorOe* oe;
    stringstream ss;
    Log* log = Log::getInstance();
    
    if(kar == NULL) {
        cout << "Bridge: handle_start_watering() got an unknown KarID: " << (int)msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    // FOR EACH OE - SET VALVE STATE (open)
    oe_list_.iter();
    while((oe = oe_list_.next())) {
        if(oe->kar_id == kar->id) {
            MOeSetValveState* vmsg = new MOeSetValveState(this, kar);
            vmsg->address = oe->address;
            vmsg->state = MOeSetValveState::OPEN;
            kar_bus_->send(E_OE_SET_VALVE_STATE, vmsg);
            
            ss.str("");
            ss << "Open valve for OE address: " << (int)oe->address;
            log->write(ss.str());
        }
    }
    
    // SET PUMP STATE ON KAR
    MKarSetPumpState* pmsg = new MKarSetPumpState(this, kar);
    pmsg->state = MKarSetPumpState::MIDDLE;
    kar_bus_->send(E_KAR_SET_PUMP_STATE, pmsg);
    kar->set_mwstatus(true);
    
    // Write to log
    ss.str("");
    ss << "Start pump for Kar: " << (int)kar->id;
    log->write(ss.str());
}


void Bridge::handle_stop_watering(GuiMessage* msg) {
    stringstream ss;
    Log* log = Log::getInstance();
    Kar* kar = kar_list_.get(msg->kar_id);
    SensorOe* oe;
    
    if(kar == NULL) {
        cout << "Bridge: handle_stop_watering() got an unknown KarID: " << (int)msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    // SET PUMP STATE ON KAR
    MKarSetPumpState* pmsg = new MKarSetPumpState(this, kar);
    pmsg->state = MKarSetPumpState::OFF;
    kar_bus_->send(E_KAR_SET_PUMP_STATE, pmsg);
    kar->set_mwstatus(false);
    
    // Write to log
    ss.str("");
    ss << "Stop pump for Kar: " << (int)kar->id;
    log->write(ss.str());
    
    // FOR EACH OE - SET VALVE STATE (closed)
    oe_list_.iter();
    while((oe = oe_list_.next())) {
        if(oe->kar_id == kar->id) {
            MOeSetValveState* vmsg = new MOeSetValveState(this, kar);
            vmsg->address = oe->address;
            vmsg->state = MOeSetValveState::CLOSED;
            kar_bus_->send(E_OE_SET_VALVE_STATE, vmsg);
            
			// Write to log
            ss.str("");
            ss << "Close valve for OE address: " << (int)oe->address;
            log->write(ss.str());
        }
    }
}


/* -- INTAKE VALVE --------------------------------------------------------- */


void Bridge::handle_ivalve_open(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    stringstream ss;
    Log* log = Log::getInstance();
    
    if(kar == NULL) {
        cout << "Bridge: handle_ivalve_open() got an unknown KarID: " << (int)msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MKarSetValveState* kmsg = new MKarSetValveState(this, kar);
    kmsg->valve = MKarSetValveState::INTAKE;
    kmsg->state = MKarSetValveState::OPEN;
    kar_bus_->send(E_KAR_SET_VALVE_STATE, kmsg);
    
    kar->set_ivalvestatus(true);
    
    // Write to log
    ss.str("");
    ss << "Opening intake valve for Kar: " << (int)kar->id;
    log->write(ss.str());
}


void Bridge::handle_ivalve_close(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    stringstream ss;
    Log* log = Log::getInstance();
    
    if(kar == NULL) {
        cout << "Bridge: handle_ivalve_close() got an unknown KarID: " << (int)msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MKarSetValveState* kmsg = new MKarSetValveState(this, kar);
    kmsg->valve = MKarSetValveState::INTAKE;
    kmsg->state = MKarSetValveState::CLOSED;
    kar_bus_->send(E_KAR_SET_VALVE_STATE, kmsg);
    
    kar->set_ivalvestatus(false);
    
    // Write to log
    ss.str("");
    ss << "Closing intake valve for Kar: " << (int)kar->id;
    log->write(ss.str());
}


/* -- OUTTAKE VALVE -------------------------------------------------------- */


void Bridge::handle_ovalve_open(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    stringstream ss;
    Log* log = Log::getInstance();
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_open() got an unknown KarID: " << (int)msg->kar_id << ", quitting!" << endl;
        return;
    }
    
	// OPEN VALVE
    MKarSetValveState* kmsg = new MKarSetValveState(this, kar);
    kmsg->valve = MKarSetValveState::OUTTAKE;
    kmsg->state = MKarSetValveState::OPEN;
    kar_bus_->send(E_KAR_SET_VALVE_STATE, kmsg);
    
    // Write to log
    ss.str("");
    ss << "Opening outtake valve for Kar: " << (int)kar->id;
    log->write(ss.str());
	
	// START PUMP
    MKarSetPumpState* pmsg = new MKarSetPumpState(this, kar);
    pmsg->state = MKarSetPumpState::MIDDLE;
    kar_bus_->send(E_KAR_SET_PUMP_STATE, pmsg);
    
    // Write to log
    ss.str("");
    ss << "Start pump for Kar: " << (int)kar->id;
    log->write(ss.str());
    
    kar->set_ovalvestatus(true);
}


void Bridge::handle_ovalve_close(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    stringstream ss;
    Log* log = Log::getInstance();
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_close() got an unknown KarID: " << (int)msg->kar_id << ", quitting!" << endl;
        return;
    }
	
	// STOP PUMP
    MKarSetPumpState* pmsg = new MKarSetPumpState(this, kar);
    pmsg->state = MKarSetPumpState::OFF;
    kar_bus_->send(E_KAR_SET_PUMP_STATE, pmsg);
    
    // Write to log
    ss.str("");
    ss << "Stop pump for Kar: " << (int)kar->id;
    log->write(ss.str());
    
	// CLOSE VALVE
    MKarSetValveState* kmsg = new MKarSetValveState(this, kar);
    kmsg->valve = MKarSetValveState::OUTTAKE;
    kmsg->state = MKarSetValveState::CLOSED;
    kar_bus_->send(E_KAR_SET_VALVE_STATE, kmsg);
    
    // Write to log
    ss.str("");
    ss << "Closing outtake valve for Kar: " << (int)kar->id;
    log->write(ss.str());
    
    kar->set_ovalvestatus(false);
}


/* -- SESNOR READ -------------------------------------------------------- */

void Bridge::handle_oe_read(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_open() got an unknown KarID: " << (int)msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MOeGetSensorData* kmsg = new MOeGetSensorData(this, kar);
    kmsg->address = 0x4; 
    kar_bus_->send(E_OE_GET_SENSOR_DATA, kmsg);

}

void Bridge::handle_kar_read(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_close() got an unknown KarID: " << (int)msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MKarSensorData* kmsg = new MKarSensorData(this, kar);
    kar_bus_->send(E_KAR_GET_SENSOR_DATA, kmsg);
    
}

void Bridge::handle_ready_read(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_close() got an unknown KarID: " << (int)msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MKarReady* kmsg = new MKarReady(this, kar);
    kar_bus_->send(E_KAR_READY, kmsg);
    
}

void Bridge::handle_oe_list_read(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_close() got an unknown KarID: " << (int)msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MKarGetOeList* kmsg = new MKarGetOeList(this, kar);
    kar_bus_->send(E_KAR_GET_OE_LIST, kmsg);
    
}

void Bridge::handle_sensor_type_read(GuiMessage* msg) {
    Kar* kar = kar_list_.get(msg->kar_id);
    
    if(kar == NULL) {
        cout << "Bridge: handle_ovalve_close() got an unknown KarID: " << (int)msg->kar_id << ", quitting!" << endl;
        return;
    }
    
    MOeGetSensorType* kmsg = new MOeGetSensorType(this, kar);
	kmsg->address = 0x4;
	kmsg->sensor_id = 0x1;
    kar_bus_->send(E_OE_GET_SENSOR_TYPE, kmsg);
    
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
