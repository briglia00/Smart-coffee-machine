#ifndef __SERVOMANAGER__
#define __SERVOMANAGER__

#include "Task.h"
#include "MachineState.h"
#include "ServoTimer2.h" 
#include "TempSensor.h"

class ServoManager: public Task {
  MachineState* mstate;
  int pin;
  int pos;
  int delta;
  volatile status lastState;
  ServoTimer2* servo;
  TempSensor* temp;

public:
  ServoManager(MachineState* mstate, int pin, TempSensor* t);
  void init(int period);  
  void tick();
};

#endif