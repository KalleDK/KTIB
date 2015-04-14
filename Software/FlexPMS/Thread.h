#pragma once
#include <pthread.h>


class Thread {
public:
    void start();
    void stop();
    void join();
    virtual void run() = 0;
private:
    static void* run_thread(void* arg);
protected:
    void sleep(unsigned int ms);
    pthread_t thread_;
};
