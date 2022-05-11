#include "DisplayManager.h"
#include "Arduino.h"


DisplayManager::DisplayManager(MachineState* mstate){
  this->mstate = mstate; 
  this->lcd.init();
  this->lcd.backlight();
  status lastCase = INITIALIZING;
}

void DisplayManager::init(int period){
  Task::init(period);
}

void DisplayManager::tick(){
  noInterrupts();
  status machinestate = this->mstate->getStatus();
  interrupts();
  if(lastCase != machinestate){
    lastCase = machinestate;
    lcd.clear();
  }
  switch(machinestate){
    case(INITIALIZING):
      lcd.setCursor(0, 0);
      lcd.print("Initialization...");
      break;
    case(READY):
      lcd.setCursor(0, 0);
      lcd.print("Select Product:");
      lcd.setCursor(0, 1);
      lcd.print(String(this->mstate->getCurrentItemName()) + "            ");
      break;
    case(MAKING):
      lcd.setCursor(0, 0);
      lcd.print("Making");
      lcd.setCursor(0, 1);
      lcd.print(String(this->mstate->getCurrentItemName()) + "            ");
      break;
    case(DONE):
      lcd.setCursor(0, 0);
      lcd.print(String(this->mstate->getCurrentItemName()) + " Ready.");
      break;
    case(MAINTENANCE):
      lcd.setCursor(0, 0);
      lcd.print("Assistance");
      lcd.setCursor(0, 1);
      lcd.print("Required");
      break;
    case(SLEEP):
      lcd.setCursor(0, 0);
      lcd.print("Sleep");
      break;
    case(TEST):
      lcd.setCursor(0, 0);
      lcd.print("Testing Machine");
      break;
    case(BROKEN):
      lcd.setCursor(0, 0);
      lcd.print("Assistance");
      lcd.setCursor(0, 1);
      lcd.print("Required");
      break;
  }
}
  