#include <Stepper.h>

const int stepsPerRevolution = 2048;

#define IN1 4
#define IN2 16
#define IN3 17
#define IN4 5

Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

void setup() {
  myStepper.setSpeed(10);
}

void loop() {
  myStepper.step(stepsPerRevolution);
}

**