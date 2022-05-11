#include "ServoManager.h"
#include "MachineState.h"

ServoManager::ServoManager(MachineState* mstate, int pin, TempSensor* t){
  this->mstate = mstate;
  this->pin = pin;
  this->pos = 545;
  this->delta = 20;
  this->servo = new ServoTimer2();
  this->servo->attach(pin);
  this->servo->write(this->pos-1);
  this->temp = t;
  lastState = INITIALIZING;
  

}

void ServoManager::init(int period){
  Task::init(period);
}

void ServoManager::tick(){
  noInterrupts();
  volatile status currentState = mstate->getStatus();
  interrupts(); 
  if(lastState != currentState){
    lastState = currentState;
  }
  switch(currentState){
    case(READY):
      if(this->pos!=545){
        this->pos = 545;
        this->servo->write(this->pos-1);
      }
      break;
    case(MAKING):
      this->servo->write(pos);
      this->pos = this->pos + this->delta;
      if(pos >= 2370){
        noInterrupts();
        this->mstate->setStatus(DONE);
        interrupts();
      }
      break;
    case(DONE):
      break;
    case(TEST):
      this->servo->write(pos);
      this->pos = this->pos + this->delta;
      if(pos >= 2370 && this->delta > 0){
        this->delta = - this->delta;
      }
      if(pos <= 565 && this->delta < 0){
        noInterrupts();
        this->delta = -delta;
        this->mstate->madeCheck();
        float x = temp->TemperatureDetected();
        if(x < 10 || x > 50){
          this->mstate->setStatus(BROKEN);
        } else {
          this->mstate->setStatus(READY);
        }
        
        interrupts();
      }
      break;
    case(BROKEN):
      break;
  }
  
}