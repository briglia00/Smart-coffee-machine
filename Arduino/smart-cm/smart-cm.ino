#include <avr/sleep.h>
#include <avr/power.h>
#include "PrintTask.h"
#include "Scheduler.h"
#include "ServoTimer2.h"
#include "Task.h"
#include "MachineState.h"
#include "MachineManager.h"
#include "DisplayManager.h"
#include "ButtonImpl.h"
#include "Pir.h"
#include "Sonar.h"
#include "ServoManager.h"
#include "MsgService.h"
#include "TempSensor.h"


#define ECHO_PIN 9
#define TRIG_PIN 10
#define PIR_PIN 2
#define B_UP 5
#define B_DOWN 3
#define B_MAKE 4
#define TEMP_PIN A1
#define NTEA 1
#define NCOF 5
#define NCHOC 10
#define SERVO_PIN 11
Scheduler sched;

void setup() {
  sched.init(100);
  Pir* pir = new Pir(PIR_PIN);
  Sonar* sonar = new Sonar(ECHO_PIN, TRIG_PIN);
  Button* b_up = new ButtonImpl(B_UP);
  Button* b_down = new ButtonImpl(B_DOWN);
  Button* b_make = new ButtonImpl(B_MAKE);
  TempSensor* t = new TempSensor(A1);
  MachineState* mstate = new MachineState(NTEA, NCOF, NCHOC);
  Task* mmanager = new MachineManager(mstate, b_up, b_down, b_make, pir, sonar);
  DisplayManager* display = new DisplayManager(mstate);
  ServoManager* servomanager = new ServoManager(mstate, SERVO_PIN, t);
  MsgServiceClass* msg = new MsgServiceClass(mstate);
  msg->init(60);
  display->init(90);
  mmanager->init(50);
  servomanager->init(90);
  sched.addTask(mmanager);
  sched.addTask(display);
  sched.addTask(servomanager);
  sched.addTask(msg);
  Serial.begin(9600);
}

void loop() {
  sched.schedule();
}
