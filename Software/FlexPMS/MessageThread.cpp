#include "MessageThread.h"
#include "Message.h"


void MessageThread::run() {
    unsigned long event_id;
    Message* msg;
    
    running_ = true;
    
    while(running_) {
        msg = queue_.recieve(event_id);
        dispatch(event_id, msg);
        if(msg != NULL)
            delete msg;
    }
}


void MessageThread::send(unsigned long id, Message* msg) {
    queue_.send(id, msg);
}
