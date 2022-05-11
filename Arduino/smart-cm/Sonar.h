#ifndef __SONAR__
#define __SONAR__

class Sonar {

public:  
  Sonar(int echoPin, int trigPin);
  float getDistance();
  
private:
    const float vs = 331.5 + 0.62 * 20;
    int echoPin, trigPin;
};

#endif 
