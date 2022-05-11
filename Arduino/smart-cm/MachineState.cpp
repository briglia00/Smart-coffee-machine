#include "Arduino.h"
#include "MachineState.h"

MachineState::MachineState(int nTea, int nCoffee, int nChocolate){
  this->quantities[0] = nTea;
  this->quantities[1] = nCoffee;
  this->quantities[2] = nChocolate;
  this->machineState = INITIALIZING;
  this->currentItem = 0;
  this->sugarValue = 0;
  this->numberTest = 0;
  this->currentItemName = items[currentItem];
}

status MachineState::getStatus(){
  return this->machineState;
}

void MachineState::setStatus(status stat){
  this->machineState = stat;
}

bool MachineState::isAvailable(int codProdotto){
  return (codProdotto < 3 && codProdotto >= 0 && this->quantities[codProdotto] > 0);
}

bool MachineState::isAnyProductAvailable(){
  for(int i = 0; i < 3; i++){
    if(quantities[i] > 0){
      return true;
    }
  }
  return false;
}

void MachineState::nextProduct(){
  if(currentItem < 2){
    this->currentItem += 1; 
  } else {
    this->currentItem = 0;
  }
  if(this->isAnyProductAvailable()){
    if(isAvailable(this->currentItem)){
      this->currentItemName = items[currentItem];
    } else {
      nextProduct();
    }
  }
}

void MachineState::prevProduct(){
  if(currentItem > 0){
    this->currentItem -= 1; 
  } else {
    this->currentItem = 2;
  }
  if(this->isAnyProductAvailable()){
    if(isAvailable(this->currentItem)){
      this->currentItemName = items[currentItem];
    } else {
      prevProduct();
    }
  }
}

void MachineState::reduceCurrentItem(){
  int q = quantities[this->currentItem];
  if(q > 0){
    quantities[this->currentItem] = quantities[this->currentItem] - 1; 
    //Serial.println(this->quantities[this->currentItem]);
  } else {
    this->machineState = READY;
  }
}

char* MachineState::getCurrentItemName(){
  this->currentItemName = items[this->currentItem];
  return this->currentItemName;
}

void MachineState::setSugarValue(){
  this->sugarValue = map(analogRead(POT_PIN), 0, 1023, 0, 5);
}

void MachineState::refillMachine(int e0, int e1, int e2){
  this->quantities[0] = quantities[0] + e0;
  this->quantities[1] = quantities[1] + e1;
  this->quantities[2] = quantities[2] + e2;
}

int MachineState::getTeaAvailable(){
  return quantities[0];
}

int MachineState::getCoffeeAvailable(){
  return quantities[1];
}

int MachineState::getChocolateAvailable(){
  return quantities[2];
}

void MachineState::madeCheck(){
  this->numberTest = numberTest + 1;
}

int MachineState::getNumberSelfTests(){
  return this->numberTest;
}

void MachineState::refreshProduct(){
  if(this->isAnyProductAvailable()){
    if(isAvailable(this->currentItem)){
      this->currentItemName = items[currentItem];
    } else {
      nextProduct();
    }
  }
}
