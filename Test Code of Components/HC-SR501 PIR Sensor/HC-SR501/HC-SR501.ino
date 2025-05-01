#define pirPin 2
#define ledPin 32

bool PIRstate = false;

unsigned long lastPirTime = 0;
const unsigned long PirTime = 20000;

void setup() {
  pinMode(pirPin, INPUT); 
  pinMode(ledPin, OUTPUT); 
  Serial.begin(115200);      
}

void loop() {
  bool PIRstate = digitalRead(pirPin);

  if (PIRstate) {
    lastPirTime = millis();
    Serial.println("Motion Detected");
  }

  if (millis() - lastPirTime < PirTime) {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON");
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF"); 
  }
  delay(1000);
}

/*
void loop() {
  state = digitalRead(pirPin);      
  if (state == true) {                
    Serial.println("LED ON");  
  } else {
    Serial.println("LED OFF");
  }
  delay(1000);
}
*/

