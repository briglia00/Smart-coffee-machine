#include "MachineManager.h"

MachineManager::MachineManager(MachineState* mstate, ButtonImpl* b_up, ButtonImpl* b_down, ButtonImpl* b_make, Pir* pir, Sonar* sonar){
  this->machineState = INITIALIZING;
  this->mstate = mstate;
  this->b_up = b_up;
  this->b_down = b_down;
  this->b_make = b_make;
  this->pir = pir;
  this->sonar = sonar;
  this->timeSinceLastEvent = millis();
  t = 0;
}

void MachineManager::init(int period){
  Task::init(period);
}

void MachineManager::tick(){
  if(!mstate->isAnyProductAvailable()){
    noInterrupts();
    mstate->setStatus(MAINTENANCE);
    interrupts();
  }
  if (mstate->getStatus() != this->machineState){
    this->timeSinceLastEvent = millis();
    this->mstate->refreshProduct();
  }
  noInterrupts();
  this->machineState = mstate->getStatus();
  interrupts();
  
  switch(this->machineState){
    case(INITIALIZING): 
      if(millis() > this->timeSinceLastEvent + TSTART * 1000){
        noInterrupts();
        mstate->setStatus(READY);
        interrupts();
        this->timeSinceLastEvent = millis();
      }
      break;
    case(READY):
      this->t = t + 1;
      if(b_up->isPressed()){
        mstate->nextProduct();
        this->timeSinceLastEvent = millis();
      }
      if(b_down->isPressed()){
        mstate->prevProduct();
        this->timeSinceLastEvent = millis();
      }
      if(b_make->isPressed()){
        noInterrupts();
        mstate->setStatus(MAKING);
        interrupts();
        mstate->setSugarValue();
        this->timeSinceLastEvent = millis();
      }
      if(millis() > this->timeSinceLastEvent + TSLEEP * 1000 && !this->pir->MovementDetected()){
        goSleep();
      }
      if(t > TCHECK * 50){
        noInterrupts();
        mstate->setStatus(TEST);
        interrupts();
      }
      break;
    case(MAKING):
      this->t = t + 1;
      break;
    case(DONE):
      this->t = t + 1;
      //Serial.println(this->sonar->getDistance());    
      if((millis() > (this->timeSinceLastEvent + TOUT * 1000)) || this->sonar->getDistance() > 0.40){
        mstate->setStatus(READY);
        mstate->reduceCurrentItem();
        this->mstate->nextProduct();
      }
      break;
    case(MAINTENANCE):
      if(mstate->isAnyProductAvailable()){
        noInterrupts();
        this->mstate->setStatus(READY);
        interrupts();
      }
      break;
    case(SLEEP):
      break;
    case(TEST):
      t = 0;
      break;
    case(BROKEN):
      break;
  }
}

static void MachineManager::wakeUp(){
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(2));
  
}

void MachineManager::goSleep(){
  mstate->setStatus(SLEEP);
  int interrupt_pin = 2;
  
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), wakeUp, RISING);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  mstate->setStatus(READY);
  this->timeSinceLastEvent = millis();
}

