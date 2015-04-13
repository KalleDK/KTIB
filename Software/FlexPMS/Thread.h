#pragma once
#include <pthread.h>
#include "MessageQueue.h"


class Message;


class Thread {
public:
    Thread();
    static void* run_thread(void* arg);
    void join();
    void wait(unsigned int ms);
    virtual void run() = 0;
    void send(unsigned long id, Message* msg = NULL);
protected:
    pthread_t thread_;
    MessageQueue queue_;
};

