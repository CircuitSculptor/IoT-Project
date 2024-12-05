/*
Wiring Diagram (Vcc + Gnd)
AHT20         Lux           DHT11         
SCL -> D22    SCL -> D22    SIG  -> D18
SDA -> D21    SDA -> D21
              EN  -> D19
*/

#include <WiFi.h>
#include <Wire.h>
#include <DFRobot_B_LUX_V30B.h> 
#include <Adafruit_AHTX0.h>
#include "DHT.h"
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

#define DHT11PIN 18

Adafruit_AHTX0 aht;
DFRobot_B_LUX_V30B myLux(19); 
DHT dht(DHT11PIN, DHT11);

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

String myStatus = "";

float temperatureAHT = 0;
float humidityAHT = 0;
float temperatureDHT = 0;
float humidityDHT = 0;
float luxValue = 0; 

void setup() {
  Serial.begin(115200);  //Initialize serial
  Serial.print("Before !Serial");
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  Serial.print("Setup");

  // AHT20
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");

  //Serial.println(F("DHT11 Sensor Test"));

  dht.begin();

  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  sensors_event_t humidityEvent, tempEvent;
  aht.getEvent(&humidityEvent, &tempEvent); 
  temperatureAHT = tempEvent.temperature;      
  humidityAHT = humidityEvent.relative_humidity; 
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
  
  delay(20000); // Wait 20 seconds to update the channel again
}

