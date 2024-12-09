#include <WiFi.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <DFRobot_B_LUX_V30B.h>
#include <Adafruit_AHTX0.h>
#include "DHT.h"
#include "secrets.h"

#define DHT11PIN 18 
#define BUTTON_PIN1 34
#define BUTTON_PIN2 35

WiFiClient client;
WiFiServer server(80);

char ssid[] = SECRET_SSID; 
char pass[] = SECRET_PASS;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

unsigned long myChannelNumber2 = SECRET_CH_ID2;
const char * myWriteAPIKey2 = SECRET_WRITE_APIKEY2;

Adafruit_AHTX0 aht;               
DFRobot_B_LUX_V30B myLux(19);     
DHT dht(DHT11PIN, DHT11);         

float temperatureAHT = 0;
float humidityAHT = 0;
float temperatureDHT = 0;
float humidityDHT = 0;
float luxValue = 0; 

bool button1ToggleState = false;
bool button2ToggleState = false;
bool lastButton1State = LOW;
bool lastButton2State = LOW;

unsigned long previousThingSpeakTime = 0;
const long thingSpeakInterval = 20000;    // Changed from default 20 second delay

//IPAddress local_IP(192, 168, 137, 200);

void setup() {
  Serial.begin(115200);

  //if (!WiFi.config(local_IP)) {
  //  Serial.println("Failed to configure static IP");
  //}

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the web server
  server.begin();

  // Initialize sensors
  if (!aht.begin()) {
    Serial.println("Could not find AHT sensor.");
    while (1) delay(10);
  }
  dht.begin();

  pinMode(BUTTON_PIN1, INPUT);
  pinMode(BUTTON_PIN2, INPUT);

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  handleButton();
  // Handle ThingSpeak updates
  if (millis() - previousThingSpeakTime >= thingSpeakInterval) {
    previousThingSpeakTime = millis();
    sendDataToThingSpeak();
  }

  // Webserver
  handleClientRequests();
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
  sensors_event_t humidityEvent, tempEvent; 
  bool success = aht.getEvent(&humidityEvent, &tempEvent);
  if (success) {
    temperatureAHT = tempEvent.temperature;
    humidityAHT = humidityEvent.relative_humidity;
  } else {
  Serial.println("AHT sensor read failed!");
  }

  Serial.println("Inside Sensor");
  Serial.print("Temperature: "); Serial.print(temperatureAHT); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidityAHT); Serial.println("% rH\n");
  
  //dht.begin();
  float temperatureDHT = dht.readTemperature();
  float humidityDHT = dht.readHumidity(); 
  Serial.println("Outside Sensor");
  Serial.print("Temperature: "); Serial.print(temperatureDHT); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidityDHT); Serial.println("% rH\n");
  
  myLux.begin();
  float luxValue = myLux.lightStrengthLux(); 
  Serial.print("Lux value: "); Serial.println(luxValue); 
  
  // Set the fields with the values
  ThingSpeak.setField(1, temperatureAHT);
  ThingSpeak.setField(2, humidityAHT);
  ThingSpeak.setField(3, temperatureDHT);
  ThingSpeak.setField(4, humidityDHT);
  ThingSpeak.setField(5, luxValue);     
  ThingSpeak.setField(6, button1ToggleState ? 1 : 0);
  ThingSpeak.setField(7, button2ToggleState ? 1 : 0);

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel 1 update successful.");
  }
  else{
    Serial.println("Problem updating channel 1. HTTP error code " + String(x));
  }

  // Second ThingSpeak Channel

  // Random value 0 or 1 generator for demo
  int LED = esp_random() % 2;

  Serial.print("LED Status: "); Serial.println(LED); 

  ThingSpeak.setField(1, LED);

  // write to the ThingSpeak channel
  int y = ThingSpeak.writeFields(myChannelNumber2, myWriteAPIKey2);
  if(y == 200){
    Serial.println("Channel 2 update successful.");
  }
  else{
    Serial.println("Problem updating channel 2. HTTP error code " + String(y));
  }
}

// Webserver to show data from Thingspeak that makes graphs for data
void handleClientRequests() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head><title>ESP32 Web Server</title></head>");
            client.println("<body>");
            client.println("<h1>ThingSpeak Dashboard</h1>");
            client.println("<div style='display: flex; justify-content: space-around;'>");
            client.println("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/2712996/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Inside+Temperature+%C2%B0C&type=line&yaxismax=35&yaxismin=15'></iframe>");
            client.println("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/2712996/charts/2?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Inside+Humidity+rH%25&type=line&yaxismax=105&yaxismin=0'></iframe>");
            client.println("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/2712996/charts/6?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Button+1&type=line&yaxis=Status'></iframe>");
            client.println("</div>");
            client.println("<div style='display: flex; justify-content: space-around;'>");
            client.println("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/2712996/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&round=2&title=Outside+Temperature+%C2%B0C&type=line&yaxismax=35&yaxismin=15'></iframe>");
            client.println("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/2712996/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&round=0&title=Outside+Humidity+rH%25&type=line&yaxismax=105&yaxismin=0'></iframe>");
            client.println("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/2712996/charts/7?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Button+2&type=line&yaxis=Status'></iframe>");
            client.println("</div>");
            client.println("<div style='display: flex; justify-content: space-around;'>");
            //client.println("<div style='text-align: center;'>");
            client.println("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/2712996/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Lux+Meter&type=line&yaxismax=600'></iframe>");
            client.println("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/2779266/charts/1?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=LED+Status&type=line'></iframe>");
            client.println("</body>");
            client.println("</html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
