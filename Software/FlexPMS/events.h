#pragma once
#include "Message.h"


// Events to MAIN
enum {
    // From Kar
    E_QUIT,
    E_PING_CNF,
    E_READY_CNF,
    E_SET_PH_LEVEL_CNF,
    E_SET_VOLUMEN_LEVEL_CNF,
    E_SET_SOIL_HUMIDITY_LEVEL_CNF,
    E_GET_KAR_SENSOR_DATA_CNF,
    
    // From Gui
    E_START_WATERING,
    E_STOP_WATERING,
    E_WATERING_STATUS,
    E_IS_WATERING,
    E_IS_NOT_WATERING,
    
    E_PING_REQ,
    E_READY_REQ,
    E_SET_PH_LEVEL_REQ,
    E_SET_VOLUMEN_LEVEL_REQ,
    E_SET_SOIL_HUMIDITY_LEVEL_REQ,
    E_GET_KAR_SENSOR_DATA_REQ,
    
    E_RECV_DATA,
    E_SEND_DATA,
    E_KILL,
};

