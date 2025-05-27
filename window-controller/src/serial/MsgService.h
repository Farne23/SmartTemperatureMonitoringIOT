#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "MsgQueue.h"

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

class Pattern {
public:
  virtual boolean match(const Msg& m) = 0;  
};

class MsgServiceClass {
    
public: 
  
  //Msg* currentMsg;
  MsgQueue *queue;
  bool msgAvailable;

  void init();  

  bool isMsgAvailable();
  Msg* receiveMsg();

  //bool isMsgAvailable(Pattern& pattern);
  //Msg* receiveMsg(Pattern& pattern);
  
  void sendMsg(const Msg& msg);
  void sendMsg(const String& msg);
};

extern MsgServiceClass MsgService;

#endif
