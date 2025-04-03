#define BUTTON_PIN 35  
#define LED_PIN 32     

bool ledState = false; 
bool lastButtonState = LOW; 

void setup() {
  pinMode(BUTTON_PIN, INPUT);     
  pinMode(LED_PIN, OUTPUT);        
  digitalWrite(LED_PIN, HIGH);    
}

void loop() {
  bool buttonState = digitalRead(BUTTON_PIN);

  if (buttonState == HIGH && lastButtonState == LOW) {
    delay(50); 
    if (digitalRead(BUTTON_PIN) == HIGH) {
      ledState = !ledState; 
      digitalWrite(LED_PIN, ledState ? LOW : HIGH); 
    }
  }
  lastButtonState = buttonState; 
}
