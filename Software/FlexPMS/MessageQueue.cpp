#include <queue>
#include <pthread.h>
#include "MessageQueue.h"


MessageQueue::MessageQueue() {
    pthread_mutex_init(&mtx_, NULL);
    pthread_cond_init(&cond_, NULL);
}


MessageQueue::~MessageQueue() {
    pthread_mutex_destroy(&mtx_);
    pthread_cond_destroy(&cond_);
}


void MessageQueue::send(unsigned long id, Message* msg) {
    pthread_mutex_lock(&mtx_);
    
    q_.push(new Item(msg, id));
    
    pthread_mutex_unlock(&mtx_);
    pthread_cond_signal(&cond_);
}


Message* MessageQueue::recieve_nowait(long& id) {
    Message* msg = NULL;
    pthread_mutex_lock(&mtx_);
    
    if (q_.size() > 0) {
        Item* item = q_.front();
        q_.pop();
        
        pthread_mutex_unlock(&mtx_);
        
        id = item->id;
        msg = item->msg;
        delete item;
    } else {
        pthread_mutex_unlock(&mtx_);
        id = -1;
    }
    
    return msg;
}


Message* MessageQueue::recieve(long& id) {
    pthread_mutex_lock(&mtx_);
    
    while (q_.size() < 1)
        pthread_cond_wait(&cond_, &mtx_);
    
    Item* item = q_.front();
    q_.pop();
    
    pthread_mutex_unlock(&mtx_);
    
    id = item->id;
    Message* msg = item->msg;
    delete item;
    
    return msg;
}

