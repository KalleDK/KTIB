#include <unistd.h>
#include "Thread.h"


void Thread::start() {
    pthread_create(&thread_, NULL, Thread::run_thread, (void *) this);
}


void* Thread::run_thread(void* arg) {
    Thread* obj = (Thread*) arg;
    obj->run();
}


void Thread::stop() {
    pthread_cancel(thread_);
}


void Thread::join() {
    void* res_dummy;
    pthread_join(thread_, &res_dummy);
}


void Thread::sleep(unsigned int ms) {
    usleep(ms * 1000);
}
