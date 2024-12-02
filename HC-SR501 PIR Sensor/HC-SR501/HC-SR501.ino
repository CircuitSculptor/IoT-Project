const int pirPin = 19;

int state = 0;
int PIR = 0;

void setup() {
  pinMode(pirPin, INPUT);  
  Serial.begin(115200);      
}

void loop() {
  state = digitalRead(pirPin);      
  if (state == HIGH) {                
    Serial.println("Movement Detected");  
    PIR = 1;
  } else {
    Serial.println("Monitoring...");
    PIR = 0;
  }
  delay(1000);
}

