#include <Stepper.h>

const int stepsPerRevolution = 2048; // Number of steps per revolution

// ULN2003 Motor Driver Pins
#define IN1 4
#define IN2 16
#define IN3 17
#define IN4 5

// Initialize the stepper library
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

// Specify the desired number of revolutions
float desiredRevolutions = 1;

void setup() {
    // Initialize the serial port
    Serial.begin(115200);

    BlindsMovement();

}

void BlindsMovement() {
      
      // Set the speed at 10 RPM
      myStepper.setSpeed(10);

      // Calculate the total number of steps
      int totalSteps = desiredRevolutions * stepsPerRevolution;

      // Move the stepper motor the desired number of steps
      Serial.println("Moving stepper motor...");
      myStepper.step(totalSteps);
      Serial.println("Motion complete.");
}

void loop() {
  // Nothing to do here since motion is complete in `setup`
}


