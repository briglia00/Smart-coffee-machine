#ifndef __DISPLAYMANAGER__
#define __DISPLAYMANAGER__

#include "Task.h"
#include "MachineState.h"
#include <LiquidCrystal_I2C.h> 

class DisplayManager: public Task {
  MachineState* mstate;
  LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);
  status lastCase;

public:
  DisplayManager(MachineState* mstate);  
  void init(int period);  
  void tick();
};

#endif
