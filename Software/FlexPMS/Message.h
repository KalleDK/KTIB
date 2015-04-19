#pragma once
#include <string>
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


class KarBusMessage : public Message {
public:
    KarBusMessage(MessageThread* s, Kar* k) : kar(k), Message(s) {};
    Kar* kar;
protected:
    char* data_;
};


class MKarReady : public KarBusMessage {};


class MKarReadyState : public KarBusMessage {
public:
    bool is_ready;
};


class MKarGetOeList : public KarBusMessage {};


class MKarOeList : public KarBusMessage {
    
};


class MKarGetSensorData : public KarBusMessage {
    
};


class MKarSensorData : public KarBusMessage {};


class MKarSetValveState : public KarBusMessage {
public:
    MKarSetValveState(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    enum ValveType { INTAKE = 1, OUTTAKE = 2 };
    enum ValveState { CLOSED = 0, OPEN = 1 };
    ValveType valve;
    ValveState state;
    
    const char* getData(unsigned int& length) {
        data_ = new char[4];
        data_[0] = 2;
        data_[1] = 0x07;
        data_[2] = valve;
        data_[3] = state;
        length = 4;
        return data_;
    }
};


class MKarValveState : public KarBusMessage {
public:
    MKarValveState(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    enum ValveType { INTAKE = 1, OUTTAKE = 2 };
    enum ValveState { CLOSED = 0, OPEN = 1 };
    ValveType valve;
    ValveState state;
};


class MKarSetPumpState : public KarBusMessage {
public:
    MKarSetPumpState(MessageThread* s, Kar* k) : KarBusMessage(s, k) {};
    enum PumpState { OFF = 0, SLOW = 1, MIDDLE = 2, FAST = 3 };
    PumpState state;
    
    const char* getData(unsigned int& length) {
        data_ = new char[3];
        data_[0] = 1;
        data_[1] = 0x03;
        data_[2] = state;
        length = 3;
        return data_;
    }
};


/* ------------------------------------------------------------------------- */
/* -- Gui MESSAGES --------------------------------------------------------- */
/* ------------------------------------------------------------------------- */


class GuiMessage : public Message {
public:
    GuiMessage(MessageThread* s, unsigned int id) : kar_id(id), Message(s) {};
    unsigned int kar_id;
};
