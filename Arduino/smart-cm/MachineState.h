#ifndef __MACHINESTATE__
#define __MACHINESTATE__

#define TMAKING 10
#define TOUT 5
#define TCHECK 10
#define TSLEEP 60
#define TSTART 5
#define POT_PIN A0

enum status {
  INITIALIZING, MAINTENANCE, READY, MAKING, DONE, SLEEP, TEST, BROKEN
};

class MachineState {
private:
  char* items[3] = {"Tea", "Coffee", "Chocolate"};
  int quantities[3] = {0, 0, 0};
  status machineState;
  int sugarValue;
  int currentItem;
  char* currentItemName;
  int numberTest;

public:
  MachineState(int nTea, int nCoffee, int nChocolate);
  void setSugarValue();
  void setStatus(status stat);
  status getStatus();
  bool isAvailable(int codProdotto);
  bool isAnyProductAvailable();
  void changeProduct(bool direction);
  void nextProduct();
  void prevProduct();
  void madeCheck();
  int getNumberSelfTests();
  void reduceCurrentItem();
  char* getCurrentItemName();
  void refillMachine(int e0, int e1, int e2);
  int getTeaAvailable();
  int getCoffeeAvailable();
  int getChocolateAvailable();
  void refreshProduct();
  
};

#endif