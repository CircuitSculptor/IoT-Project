#include <WiFi.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <DFRobot_B_LUX_V30B.h>
#include <Adafruit_AHTX0.h>
#include "DHT.h"
#include "secrets.h"

#define DHT11PIN 18 

WiFiClient client;
WiFiServer server(80);

char ssid[] = SECRET_SSID; 
char pass[] = SECRET_PASS;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

Adafruit_AHTX0 aht;               
DFRobot_B_LUX_V30B myLux(19);     
DHT dht(DHT11PIN, DHT11);         

float temperatureAHT = 0;
float humidityAHT = 0;
float temperatureDHT = 0;
float humidityDHT = 0;
float luxValue = 0; 

unsigned long previousThingSpeakTime = 0;
const long thingSpeakInterval = 20000; 

void setup() {
  Serial.begin(115200);

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

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Handle ThingSpeak updates
  if (millis() - previousThingSpeakTime >= thingSpeakInterval) {
    previousThingSpeakTime = millis();
    sendDataToThingSpeak();
  }

  // Handle web server requests
  handleClientRequests();
}

void sendDataToThingSpeak() {

  // Get data from sensors
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

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

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
            client.println("</div>");
            client.println("<div style='display: flex; justify-content: space-around;'>");
            client.println("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/2712996/charts/3?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&round=2&title=Outside+Temperature+%C2%B0C&type=line&yaxismax=35&yaxismin=15'></iframe>");
            client.println("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src='https://thingspeak.com/channels/2712996/charts/4?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&round=0&title=Outside+Humidity+rH%25&type=line&yaxismax=105&yaxismin=0'></iframe>");
            client.println("</div>");
            client.println("<div style='text-align: center;'>");
            client.println("<iframe width='450' height='260' style='border: 1px solid #cccccc;' src=https://thingspeak.com/channels/2712996/charts/5?bgcolor=%23ffffff&color=%23d62020&dynamic=true&results=60&title=Lux+Meter&type=line&yaxismax=600'></iframe>");
            client.println("</div>");
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
