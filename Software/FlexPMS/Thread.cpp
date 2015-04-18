#include "Thread.h"


void Thread::start() {
    pthread_create(&thread_, NULL, Thread::run_thread, (void *) this);
    enable_cancel();
}


void* Thread::run_thread(void* arg) {
    Thread* obj = (Thread*) arg;
    obj->run();
}


void Thread::cancel() {
    pthread_cancel(thread_);
}


void Thread::join() {
    void* res_dummy;
    pthread_join(thread_, &res_dummy);
}


void Thread::enable_cancel() {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
}


void Thread::disable_cancel() {
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
}


void Thread::ssleep(unsigned int sec) {
    sleep(sec);
}


void Thread::msleep(unsigned int msec) {
    usleep(msec * 1000);
}


void Thread::usleep(unsigned int usec) {
    usleep(usec);
}
