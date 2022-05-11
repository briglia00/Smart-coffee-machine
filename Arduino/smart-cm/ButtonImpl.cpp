#include "ButtonImpl.h"
#include "Arduino.h"

#define DEBOUNCING_TIME 20

ButtonImpl::ButtonImpl(int pin){
  this->pin = pin;
  pinMode(pin, INPUT);  
  lastEventTime = millis();
} 

bool ButtonImpl::isPressed(){
  long newEvent = millis();
  if(newEvent > lastEventTime + 50){
    lastEventTime = millis();
    return digitalRead(pin) == HIGH;
  }
  
}
