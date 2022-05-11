#ifndef __MACHINEMANAGER__
#define __MACHINEMANAGER__

#include "Task.h"
#include "MachineState.h"
#include "Arduino.h"
#include "ButtonImpl.h"
#include "Pir.h"
#include "Sonar.h"
#include <avr/sleep.h>
#include <avr/power.h>
#include <LowPower.h>


class MachineManager: public Task {
private:
  volatile status machineState;

  ButtonImpl* b_up;
  ButtonImpl* b_down;
  ButtonImpl* b_make;
  MachineState* mstate;
  Pir* pir;
  Sonar* sonar;
  unsigned long timeSinceLastEvent;
  void goSleep();
  int t;

public:
  MachineManager(MachineState* mstate, ButtonImpl* b_up, ButtonImpl* b_down, ButtonImpl* b_make, Pir* pir, Sonar* sonar);
  void init(int period);
  void tick();
  static void wakeUp();
};

#endif