#pragma once
#include "Thread.h"
#include "MessageQueue.h"


class Message;


class MessageThread : public Thread {
public:
    void run();
    void send(unsigned long id, Message* msg = NULL);
protected:
    bool running_;
private:
    virtual void init() {};
    virtual void dispatch(unsigned long event_id, Message* msg) = 0;
    MessageQueue queue_;
};
