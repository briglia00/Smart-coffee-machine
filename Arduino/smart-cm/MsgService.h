#ifndef __MSGSERVICE__
#define __MSGSERVICE__

#include "Task.h"
#include "Arduino.h"
#include "MachineState.h"

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

class MsgServiceClass: public Task{
    
public: 
  MsgServiceClass(MachineState* mstate);
  Msg* currentMsg;
  bool msgAvailable;

  void init(int period);  

  bool isMsgAvailable();
  Msg* receiveMsg();

  bool isMsgAvailable(Pattern& pattern);

  /* note: message deallocation is responsibility of the client */
  Msg* receiveMsg(Pattern& pattern);
  
  void sendMsg(const String& msg);
  void tick();
private:
  MachineState* mstate;
  void serialEvent();
};

#endif