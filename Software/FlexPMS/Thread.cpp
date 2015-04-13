#include <unistd.h>
#include "Thread.h"
#include "Message.h"


Thread::Thread() {
    pthread_create(&thread_, NULL, Thread::run_thread, (void *) this);
}


void* Thread::run_thread(void* arg) {
    Thread* obj = (Thread*) arg;
    obj->run();
}


void Thread::join() {
    void *res_dummy;
    pthread_join(thread_, &res_dummy);
}


void Thread::wait(unsigned int ms) {
    usleep(ms * 1000);
}


void Thread::send(unsigned long id, Message* msg) {
    queue_.send(id, msg);
}

