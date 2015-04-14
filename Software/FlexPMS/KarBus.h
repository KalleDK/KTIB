#pragma once
#include "MessageThread.h"


class Message;


class KarBus : public MessageThread {
public:
    KarBus::KarBus();
private:
    void dispatch(unsigned long event_id, Message* msg);
};
