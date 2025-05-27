#ifndef __MSGQUEUE__
#define __MSGQUEUE__

#define MAX 10
#include <Arduino.h>

class Msg {
  String content;

public:
  Msg(String content){
    this->content = content;
  }
  
  String getContent(){
    return content;
  }
};

class MsgQueue {
    public:
        MsgQueue();
        void enqueue(Msg *msg);
        Msg* dequeue();
        int size();
    private:
        Msg * messages[MAX];
        int count = 0;
};

#endif