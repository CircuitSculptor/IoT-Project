const int LeftButtonPin = 2; 
const int RightButtonPin = 15;

int LeftButtonState = 0;  
int RightButtonState = 0;

void setup() {
  // initialize the pushbutton pin as an input:
  pinMode(LeftButtonPin, INPUT);
  pinMode(RightButtonPin, INPUT);
  Serial.begin(115200);
}

void loop() {

  LeftButtonState = digitalRead(LeftButtonPin);
  RightButtonState = digitalRead(RightButtonPin);

  if (LeftButtonState == HIGH) {
    Serial.println("Left Button Pressed");
  } else {
    Serial.println("Left Button Released");
  }

  if (RightButtonState == HIGH) {
    Serial.println("Right Button Pressed");
  } else {
    Serial.println("Right Button Released");
  }
  delay(1000);
}
