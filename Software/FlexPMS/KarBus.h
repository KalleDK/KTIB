#pragma once
#include "Thread.h"


class Message;


class KarBus : public Thread {
public:
    void run();
private:
    void dispatch(long event_id, Message* msg);
    void parse(std::string line);
};

