#include "ServoManager.h"
#include "MachineState.h"

#define MOTOR_POS_MIN 545
#define MOTOR_POS_MAX 2400
#define MOTOR_DELTA 20
#define TMIN 15
#define TMAX 40

ServoManager::ServoManager(MachineState* mstate, int pin, TempSensor* t){
  this->mstate = mstate;
  this->pin = pin;
  this->pos = MOTOR_POS_MIN;
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
      if(this->pos != MOTOR_POS_MIN){
        this->pos = MOTOR_POS_MIN;
        this->servo->write(this->pos-1);
      }
      break;
    case(MAKING):
      this->servo->write(pos);
      this->pos = this->pos + this->delta;
      if(pos >= MOTOR_POS_MAX - MOTOR_DELTA){
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
      if(pos >= MOTOR_POS_MAX - MOTOR_DELTA && this->delta > 0){
        this->delta = - this->delta;
      }
      if(pos <= MOTOR_POS_MIN + MOTOR_DELTA && this->delta < 0){
        noInterrupts();
        this->delta = -delta;
        this->mstate->madeCheck();
        float x = temp->TemperatureDetected();
        if(x < TMAX || x > TMIN){
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