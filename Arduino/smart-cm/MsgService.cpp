#include "Arduino.h"
#include "MsgService.h"

String content;

//MsgServiceClass MsgService;

MsgServiceClass::MsgServiceClass(MachineState* mstate){
  this->mstate = mstate;
}

bool MsgServiceClass::isMsgAvailable(){
  return msgAvailable;
}

Msg* MsgServiceClass::receiveMsg(){
  if (msgAvailable){
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;  
  } else {
    return NULL; 
  }
}

void MsgServiceClass::init(int period){
  //Serial.begin(9600);
  content.reserve(256);
  content = "";
  currentMsg = NULL;
  msgAvailable = false;
  Task::init(period);
}

void MsgServiceClass::sendMsg(const String& msg){
  Serial.println(msg);  
}

void MsgServiceClass::serialEvent() {
  while (Serial.available()) {
    char ch = (char) Serial.read();
    if (ch == '\n'){
      currentMsg = new Msg(content);
      msgAvailable = true;      
    } else {
      content += ch;      
    }
  }
}

bool MsgServiceClass::isMsgAvailable(Pattern& pattern){
  return (msgAvailable && pattern.match(*currentMsg));
}

Msg* MsgServiceClass::receiveMsg(Pattern& pattern){
  if (msgAvailable && pattern.match(*currentMsg)){
    Msg* msg = currentMsg;
    msgAvailable = false;
    currentMsg = NULL;
    content = "";
    return msg;  
  } else {
    return NULL; 
  }
  
}

void MsgServiceClass::tick(){
  serialEvent();  
  if (isMsgAvailable()){
    
    Msg* msg = receiveMsg();
    String s = msg->getContent().substring(0,6);
    if(s == "GETTEA"){
      sendMsg(String(mstate->getTeaAvailable()));
    } else if(s == "GETCOF"){
      sendMsg(String(mstate->getCoffeeAvailable()));
    } else if(s == "GETCHO"){
      sendMsg(String(mstate->getChocolateAvailable()));
    } else if(s == "GETSTS"){
      sendMsg(String(mstate->getStatus()));
    } else if(s == "REFCOF"){
      String temp = msg->getContent().substring(7);
      this->mstate->refillMachine(0, temp.toInt(), 0);
    } else if(s == "REFTEA"){
      String temp = msg->getContent().substring(7);
      this->mstate->refillMachine(temp.toInt(), 0, 0);
    } else if(s == "REFCHO"){
      String temp = msg->getContent().substring(7);
      this->mstate->refillMachine(0, 0, temp.toInt());
    } else if(s == "GETSLF"){
      sendMsg(String(mstate->getNumberSelfTests()));
    } else if(s == "RECOVR"){
      this->mstate->setStatus(READY);
    }
    delete msg;
  }
}
