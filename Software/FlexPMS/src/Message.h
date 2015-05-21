#pragma once
#include <string>
#include <vector>
#include "Kar.h"


class MessageThread;


std::string to_string(unsigned long s);


class Message {
public:
    Message(MessageThread* s = NULL) : sender(s) {};
    virtual ~Message() {};
    void setData(std::string data);
    void setData(char* data, unsigned int length);
    std::string getData();
    const char* getData(unsigned int& length);
    MessageThread* sender;
private:
    std::string data_;
};


/* ------------------------------------------------------------------------- */
/* -- KarBus MESSAGES ------------------------------------------------------ */
/* ------------------------------------------------------------------------- */


// FIXME Namespace?


class KarBusMessage : public Message {
public:
    KarBusMessage(MessageThread* s, Kar* k) : Message(s), kar(k) {};
    Kar* kar;
protected:
    char* data_;
};


// ------------------------------------------------------------------------- //


class MKarReady : public KarBusMessage {
public:
    MKarReady(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
};


class MKarReadyState : public KarBusMessage {
public:
    MKarReadyState(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    bool is_ready;
};


// ------------------------------------------------------------------------- //


class MKarGetSensorData : public KarBusMessage {
public:
    MKarGetSensorData(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
};


class MKarSensorData : public KarBusMessage {
public:
    MKarSensorData(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    
    typedef struct {
        unsigned char sensor_id;
        double value;
    } KarSensorData;
    
    std::vector<KarSensorData> sensor_data;
};


// ------------------------------------------------------------------------- //


class MKarSetPumpState : public KarBusMessage {
public:
    MKarSetPumpState(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    enum PumpState { OFF = 0, SLOW = 1, MIDDLE = 2, FAST = 3 };
    PumpState state;
};


class MKarPumpState : public KarBusMessage {
public:
    MKarPumpState(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    enum PumpState { OFF = 0, SLOW = 1, MIDDLE = 2, FAST = 3 };
    PumpState state;
};


// ------------------------------------------------------------------------- //


class MOeGetSensorData : public KarBusMessage {
public:
    MOeGetSensorData(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    unsigned char address;
    unsigned char sensor_id;
};


class MOeSensorData : public KarBusMessage {
public:
    MOeSensorData(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    
    typedef struct {
        unsigned char address;
        unsigned char sensor_id;
		unsigned char status;
        int value;
    } OeSensorData;
    
    std::vector<OeSensorData> sensor_data;
};


// ------------------------------------------------------------------------- //


class MOeSetValveState : public KarBusMessage {
public:
    MOeSetValveState(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    enum ValveState { CLOSED = 0, OPEN = 1 };
    ValveState state;
    unsigned char address;
};


class MOeValveState : public KarBusMessage {
public:
	MOeValveState(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    enum ValveState { CLOSED = 0, OPEN = 1 };
    ValveState state;
    unsigned char address;
};


// ------------------------------------------------------------------------- //


class MOeGetSensorType : public KarBusMessage {
public:
    MOeGetSensorType(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    unsigned char address;
    unsigned char sensor_id;
};


class MOeSensorType : public KarBusMessage {
public:
    MOeSensorType(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    unsigned char address;
    unsigned char sensor_id;
    unsigned char sensor_type;
};


// ------------------------------------------------------------------------- //


class MKarGetOeList : public KarBusMessage {
public:
    MKarGetOeList(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    enum ValveState { CLOSED = 0, OPEN = 1 };
    ValveState state;
};


class MKarOeList : public KarBusMessage {
public:
    MKarOeList(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    
    std::vector<unsigned char> addresses;
};


// ------------------------------------------------------------------------- //


class MKarSetValveState : public KarBusMessage {
public:
    MKarSetValveState(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    enum ValveType { INTAKE = 1, OUTTAKE = 2 };
    enum ValveState { CLOSED = 0, OPEN = 1 };
    ValveType valve;
    ValveState state;
};


class MKarValveState : public KarBusMessage {
public:
    MKarValveState(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    enum ValveType { INTAKE = 1, OUTTAKE = 2 };
    enum ValveState { CLOSED = 0, OPEN = 1 };
    ValveType valve;
    ValveState state;
};

// ------------------------------------------------------------------------- //


class MKarSetOpretOe : public KarBusMessage {
public:
    MKarSetOpretOe(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
	unsigned char address;
};


class MKarOpretState : public KarBusMessage {
public:
    MKarOpretState(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
	bool succes;
};


/* ------------------------------------------------------------------------- */
/* -- Gui MESSAGES --------------------------------------------------------- */
/* ------------------------------------------------------------------------- */


class SessionMessage : public Message {
public:
    SessionMessage(MessageThread* s) : Message(s) {};
    unsigned long session_id;
};


class GuiMessage : public Message {
public:
    GuiMessage(MessageThread* s) : Message(s) {};
    unsigned int kar_id;
    unsigned int oe_id;
    unsigned long session_id;
};
