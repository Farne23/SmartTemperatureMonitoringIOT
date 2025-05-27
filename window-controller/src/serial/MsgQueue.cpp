#include "MsgQueue.h"

void MsgQueue::enqueue(Msg *msg) {
    // insert last, if possible
    if (this->count < MAX) {
        this->messages[this->count++] = msg;
    }
    else {
        delete msg;
    }
}

Msg * MsgQueue::dequeue() {
    if (this->count > 0) {
        Msg *first = this->messages[0];
        for (int i = 1; i < count; i++) {
            this->messages[i - 1] = this->messages[i];
        }
        //update count
        count--;
        return first;
    }
    return NULL;
}

int MsgQueue::size() {
    return this->count;
}

MsgQueue::MsgQueue() {
    this->count = 0;
}