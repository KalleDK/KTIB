#pragma once
#include <unistd.h>
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
    void ssleep(unsigned int sec);
    void msleep(unsigned int msec);
    pthread_t thread_;
};
