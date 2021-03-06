#pragma once
#include <queue>
#include <pthread.h>
#include "Message.h"


class Message;


struct Item {
    Item(Message* m, unsigned long i) : msg(m), id(i) {};
    Message *msg;
    unsigned long id;
};


class MessageQueue
{
public:
    MessageQueue();
    void send(unsigned long id, Message* msg = NULL);
    Message* recieve(unsigned long& id);
    ~MessageQueue();
private:
    std::queue<Item*> q_;
    pthread_mutex_t mtx_;
    pthread_cond_t cond_;
};
