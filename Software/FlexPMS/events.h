#pragma once


typedef enum {
    
    /*
     * EVENTS FROM KarPinger
     */
    E_PING,             // Tells Bridge that it needs to PING all Kar
    
    /*
     * EVENTS FROM KarBus
     */
    E_KAR_READY_STATE,  // Tells ready state for a Kar
    E_KAR_OE_LIST,      // Sends a list of OE for a Kar
    E_KAR_SENSOR_DATA,  // Sends sensor data for a Kar
    E_KAR_VALVE_STATE,  // Sends state of a valve from a Kar (open or closed)
    E_KAR_PUMP_STATE,   // Sends pump state from a Kar (off/on-speed)
    
    E_OE_VALVE_STATE,   // Sends valve state of OE from a Kar (open or closed)
    E_OE_SENSOR_DATA,   // Sends sensor data of OE from a Kar
    E_OE_SENSOR_TYPE,   // Sends sensor type
    
    /*
     * EVENTS FROM Gui
     */
    E_START_WATERING,   // Start manual watering
    E_STOP_WATERING,    // Stop manual watering
    E_IVALVE_OPEN,      // Open intake valve
    E_IVALVE_CLOSE,     // Close intake valve
    E_OVALVE_OPEN,      // Open outtake valve
    E_OVALVE_CLOSE,     // Close outtake valve
} BridgeEvent;


typedef enum {
    E_KAR_READY,            // Request ready state from a Kar
    
    E_KAR_GET_OE_LIST,      // Get list of OE connected to Kar
    E_KAR_GET_SENSOR_DATA,  // Get data from sensor connected to Kar
    E_KAR_SET_VALVE_STATE,  // Set state of valve connected to Kar
    E_KAR_SET_PUMP_STATE,   // Set state of pump connected to Kar
    
    E_OE_SET_VALVE_STATE,   // Set state of valve connected to OE
    E_OE_GET_SENSOR_DATA,   // Get data from sensor connected to OE
    E_OE_GET_SENSOR_TYPE,   // Get type of sensor connected to OE
} KarBusEvent;


typedef enum {
    E_RECV_DATA,        // Recieved data from socket
    E_SEND_DATA,        // Send data to socket
    E_KILL,             // Kill connection to client
} GuiEvent;
