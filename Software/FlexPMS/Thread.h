#pragma once
#include <pthread.h>


class Thread {
public:
    void start();
    void cancel();
    void join();
    virtual void run() = 0;
private:
    static void* run_thread(void* arg);
protected:
    void enable_cancel();
    void disable_cancel();
    void sleep(unsigned int ms);
    pthread_t thread_;
};
