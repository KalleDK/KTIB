#include "MessageThread.h"


void MessageThread::run() {
    unsigned long event_id;
    Message* msg;
    
    init();
    
    running_ = true;
    
    while(running_) {
        msg = queue_.recieve(event_id);
        dispatch(event_id, msg);
        delete msg;
    }
}


void MessageThread::send(unsigned long id, Message* msg) {
    queue_.send(id, msg);
}
