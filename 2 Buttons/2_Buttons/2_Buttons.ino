const int button1Pin = 32;
const int button2Pin = 33;     
const int Led1Pin = 25;
const int Led2Pin = 26;

int button1State = 0;        
int button2State = 0;

void setup() {
  Serial.begin(115200);

  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  pinMode(Led1Pin, OUTPUT);
  pinMode(Led2Pin, OUTPUT);
}

void loop() {

  button1State = digitalRead(button1Pin);
  button2State = digitalRead(button2Pin);

  if (button1State == HIGH) {
    // turn LED on:
    digitalWrite(Led1Pin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(Led1Pin, LOW);
  }

  if (button2State == HIGH) {
    // turn LED on:
    digitalWrite(Led2Pin, HIGH);
  } else {
    // turn LED off:
    digitalWrite(Led2Pin, LOW);
  }


}