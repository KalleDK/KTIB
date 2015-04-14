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


class KarBusMessage : public Message {
public:
    KarBusMessage(MessageThread* s, Kar* k) : kar(k), Message(s) {};
    Kar* kar;
};


class GuiMessage : public Message {
public:
    GuiMessage(MessageThread* s, unsigned int id) : kar_id(id), Message(s) {};
    unsigned int kar_id;
};
