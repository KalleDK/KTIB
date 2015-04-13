#pragma once
#include <string>
#include "Kar.h"


std::string to_string(unsigned long s);


class Thread;


class Message {
public:
    Message(Thread* s) : sender(s) {};
    virtual ~Message() {};
    void setData(std::string data);
    std::string getData();
    Thread* sender;
private:
    std::string data_;
};


class KarBusMessage : public Message {
public:
    KarBusMessage(Thread* s, Kar* k) : kar(k), Message(s) {};
    Kar* kar;
};


class GuiMessage : public Message {
public:
    GuiMessage(Thread* s, unsigned int id) : kar_id(id), Message(s) {};
    unsigned int kar_id;
};

