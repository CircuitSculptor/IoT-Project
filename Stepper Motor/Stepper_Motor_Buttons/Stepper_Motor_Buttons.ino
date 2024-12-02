#include <Stepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution

// ULN2003 Motor Driver Pins
#define IN1 4
#define IN2 16
#define IN3 17
#define IN4 5
#define Up 2
#define Down 15

int LeftButtonState = 0;  
int RightButtonState = 0;

// initialize the stepper library
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

void setup() {

  pinMode(Up, INPUT);
  pinMode(Down, INPUT);
  Serial.begin(115200);

  // set the speed at 5 rpm
  myStepper.setSpeed(10);
}

void loop() {
  LeftButtonState = digitalRead(Up);
  RightButtonState = digitalRead(Down);

  if (LeftButtonState == HIGH) { // Button pressed
    Serial.println("Up Button Pressed");
    myStepper.step(stepsPerRevolution); // Move stepper forward
  } else if (RightButtonState == HIGH) { // Button pressed
    Serial.println("Down Button Pressed");
    myStepper.step(-stepsPerRevolution); // Move stepper backward
  } else {
    Serial.println("No Button Pressed");
  }
  Serial.println("");

  delay(100); // Small delay to debounce the buttons
}

/*
void loop() {
  LeftButtonState = digitalRead(Up);
  RightButtonState = digitalRead(Down);
  if (Up == LOW) {
      Serial.println("Left Button Pressed");
      myStepper.step(stepsPerRevolution);
  } else {
      Serial.println("Left Button Released");
      myStepper.step(0);
  }
  if (Down == LOW) {
      Serial.println("Right Button Pressed");
      myStepper.step(-stepsPerRevolution);
  } else {
      Serial.println("Right Button Released");
      myStepper.step(0);
  }
  Serial.println("");
  delay(1000);
}*/
