#define button1Pin 35
#define button2Pin 34
#define LedPin 32   

int button1State = 0;        
int button2State = 0;

void setup() {
  Serial.begin(115200);

  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(LedPin, OUTPUT);
} 

void loop() {

  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);

  if (button1State == HIGH) {
    digitalWrite(LedPin, LOW);
    Serial.println("UP Button Pressed");
  } else {
    digitalWrite(LedPin, HIGH);
    Serial.println("UP Button Not Pressed");
  }

  if (button2State == HIGH) {
    Serial.println("DOWN Button Pressed");
  } else {
    Serial.println("DOWN Button Not Pressed");
  }
  Serial.println();
  delay(300);

}