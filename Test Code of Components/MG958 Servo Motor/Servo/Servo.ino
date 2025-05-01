#include <ESP32Servo.h>

Servo myServo;  // Create a Servo object
String input;

void setup() {
  Serial.begin(115200);
  myServo.setPeriodHertz(50);  // Standard servo frequency is 50 Hz
  myServo.attach(33);          // 33
  Serial.println("ENTER A OR B");
}

void loop() {
  if(Serial.available()){
    input = Serial.readStringUntil('\n');
    if (input == "A") {
      for(int i = 0; i <= 180; i++) {
        myServo.write(i);
        delay(22);
      }
    }
    else if (input == "B") {
      for(int i = 180; i >= 0; i--) {
        myServo.write(i);
        delay(22);
      }
    }
  }
}  

/*
void loop() {

  Serial.readString()
  myServo.write(0);    // Move servo to 0 degrees
  //Serial.println("CLOSED");
  delay(500);         // Wait

  myServo.write(90);   // Move servo to 90 degrees
  //Serial.println("OPEN");
  delay(500);         // Wait
  //Serial.println("**********");

  //myServo.write(180);  // Move servo to 180 degrees
  //delay(1000);         // Wait for 1 second
}
*/
