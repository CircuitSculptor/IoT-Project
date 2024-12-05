#include <ESP32Servo.h>

Servo myServo;  // Create a Servo object

void setup() {
  myServo.setPeriodHertz(50);  // Standard servo frequency is 50 Hz
  myServo.attach(33);          // Attach the servo to GPIO 13 (change as needed)
}

void loop() {
  myServo.write(0);    // Move servo to 0 degrees
  delay(1000);         // Wait for 1 second

  myServo.write(90);   // Move servo to 90 degrees
  delay(1000);         // Wait for 1 second

  myServo.write(180);  // Move servo to 180 degrees
  delay(1000);         // Wait for 1 second
}
