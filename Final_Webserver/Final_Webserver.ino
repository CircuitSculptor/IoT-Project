#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <DFRobot_B_LUX_V30B.h>
#include <Adafruit_AHTX0.h>
#include "DHT.h"
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ThingSpeak.h>
#include "homepage.h"
#include "secrets.h"

#define DHT11PIN 18 
#define LEDPIN 32
#define BUTTON_PIN1 34
#define BUTTON_PIN2 35

WiFiClient client;
WebServer server(80);

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

unsigned long myChannelNumber2 = SECRET_CH_ID2;
const char * myWriteAPIKey2 = SECRET_WRITE_APIKEY2;

Adafruit_AHTX0 aht;               
DFRobot_B_LUX_V30B myLux(19);     
DHT dht(DHT11PIN, DHT11);         

float temperatureDHT = 0;
float humidityDHT = 0;
float luxValue = 0; 

bool button1ToggleState = false;
bool button2ToggleState = false;
bool lastButton1State = LOW;
bool lastButton2State = LOW;

unsigned long previousThingSpeakTime = 0;
const long thingSpeakInterval = 20000;    // Changed from default 20 second delay

void handleRoot() {
  String message = homePagePart1;
  server.send(200, "text/html", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {

  Serial.begin(115200);
  //WiFi.mode(WIFI_STA);

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);

  server.begin();

  // Initialize DHT11 Sensor
  dht.begin();

  pinMode(BUTTON_PIN1, INPUT);
  pinMode(BUTTON_PIN2, INPUT);
  pinMode(LEDPIN, OUTPUT);

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks

  handleButton();
  // Handle ThingSpeak updates
  if (millis() - previousThingSpeakTime >= thingSpeakInterval) {
    previousThingSpeakTime = millis();
    sendDataToThingSpeak();
  }
}

void handleButton() {   // Reads input from both buttons
  bool button1State = digitalRead(BUTTON_PIN1);
  bool button2State = digitalRead(BUTTON_PIN2);

  // Code to toggle the button state to ON or OFF
  if (button1State == HIGH && lastButton1State == LOW) {
    delay(50); // Debounce delay
    if (digitalRead(BUTTON_PIN1) == HIGH) {
      button1ToggleState = !button1ToggleState; 
      Serial.print("Button 1 toggled: ");
      Serial.println(button1ToggleState? "ON" : "OFF");
    }
  }
  lastButton1State = button1State;

  if (button2State == HIGH && lastButton2State == LOW) {
    delay(50); // Debounce delay
    if (digitalRead(BUTTON_PIN2) == HIGH) {
      button2ToggleState = !button2ToggleState; 
      Serial.print("Button 2 toggled: ");
      Serial.println(button2ToggleState? "ON" : "OFF");
    }
  }
  lastButton2State = button2State;
}

void sendDataToThingSpeak() {

  // Gather data from AHT20, DHT11 and LUX sensor
  if (!aht.begin()) {
    Serial.println("Could not find AHT sensor.");
    while (1) delay(10);
  }
  sensors_event_t humidityEvent, tempEvent; 
  resetAHT20();
  aht.getEvent(&humidityEvent, &tempEvent);
  float temperatureAHT = 0;
  float humidityAHT = 0;
  temperatureAHT = tempEvent.temperature;
  humidityAHT = humidityEvent.relative_humidity;

  Serial.println("********************************************");
  Serial.println("Inside Sensor");
  Serial.print("Temperature: "); Serial.print(temperatureAHT); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidityAHT); Serial.println("% rH\n");
  //Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  //Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH\n");

  //dht.begin();
  float temperatureDHT = dht.readTemperature();
  float humidityDHT = dht.readHumidity(); 
  Serial.println("Outside Sensor");
  Serial.print("Temperature: "); Serial.print(temperatureDHT); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidityDHT); Serial.println("% rH\n");
  
  myLux.begin();
  float luxValue = myLux.lightStrengthLux(); 
  Serial.print("Lux value: "); Serial.println(luxValue); 

  //******************************************************************* REMOVE AND IMPLEMENT ACTUAL CODE*******************
  // Random value 0 or 1 generator for demo
  int LED = esp_random() % 2;
  Serial.print("LED Status: "); Serial.println(LED); 
  digitalWrite(LEDPIN, LED);
  
  // Set the fields with the values
  ThingSpeak.setField(1, temperatureAHT);
  ThingSpeak.setField(2, humidityAHT);
  ThingSpeak.setField(3, temperatureDHT);
  ThingSpeak.setField(4, humidityDHT);
  ThingSpeak.setField(5, luxValue);     
  ThingSpeak.setField(6, button1ToggleState ? 1 : 0);
  ThingSpeak.setField(7, button2ToggleState ? 1 : 0);
  ThingSpeak.setField(8, LED);

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel 1 update successful.");
  }
  else{
    Serial.println("Problem updating channel 1. HTTP error code " + String(x));
  }

  // Second ThingSpeak Channel

  //******************************************************************* REMOVE AND IMPLEMENT ACTUAL CODE*******************
  // Random value 0 or 1 generators for demo
  int Vent = esp_random() % 2;
  Serial.print("Vent Status: "); Serial.println(Vent);

  int Motion = esp_random() % 2;
  Serial.print("Motion Status: "); Serial.println(Motion);

  int Blinds = esp_random() % 2;
  Serial.print("Blinds Status: "); Serial.println(Blinds);

  ThingSpeak.setField(1, Vent);
  ThingSpeak.setField(2, Motion);
  ThingSpeak.setField(3, Blinds);

  // write to the ThingSpeak channel
  int y = ThingSpeak.writeFields(myChannelNumber2, myWriteAPIKey2);
  if(y == 200){
    Serial.println("Channel 2 update successful.");
  }
  else{
    Serial.println("Problem updating channel 2. HTTP error code " + String(y));
  }
}

void resetAHT20() {
  Wire.beginTransmission(0x38);
  Wire.write(0xBA);
  Wire.endTransmission();
  delay(20);
}
