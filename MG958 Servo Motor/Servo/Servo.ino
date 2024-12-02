/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/
/*

#include <ESP32Servo.h>

Servo myservo;  // create Servo object to control a servo
// twelve Servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  myservo.attach(25);  // attaches the servo on pin 9 to the Servo object
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    //delay(15);                       // waits 15 ms for the servo to reach the position
  }
  delay(1000);
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    //delay(15);                       // waits 15 ms for the servo to reach the position
  }
  delay(1000);
}
*/

#include <ESP32Servo.h>  

Servo myServo;  

int servoPin = 25; 

void setup() {
  Serial.begin(115200);  
  myServo.attach(servoPin);  
}

void loop() {

  for ()

  if (Serial.available() > 0) {
    int position = Serial.parseInt();  

    if (position >= 0 && position <= 180) {
      myServo.write(position); 
      Serial.print("Servo position set to: ");
      Serial.println(position);
    } else {
      Serial.println("Invalid input. Please enter a value between 0 and 180.");
    }
  }
}

