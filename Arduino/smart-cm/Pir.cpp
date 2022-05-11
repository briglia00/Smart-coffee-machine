#include "Pir.h"
#include "Arduino.h"

Pir::Pir(int pin){
  this->pin = pin;
  pinMode(pin, INPUT);     
} 
  
bool Pir::MovementDetected(){
  return digitalRead(pin) == HIGH;
}
