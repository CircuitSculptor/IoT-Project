#include <WiFi.h>
#include <Adafruit_AHTX0.h>       // Include the AHT sensor library
#include "secrets.h"
#include "ThingSpeak.h"           // Include ThingSpeak library after other libraries and macros

// AHT20 temperature and humidity sensor
Adafruit_AHTX0 aht;       

char ssid[] = SECRET_SSID;        
char pass[] = SECRET_PASS;          
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Initialize sensor values
float temperature = 0;
float humidity = 0;                    

void setup() {
  Serial.begin(115200);              // Initialize serial communication
  while (!Serial) {
    ; // Wait for serial port to connect (needed for Leonardo native USB port only)
  }

  // Initialize the AHT sensor
  if (!aht.begin()) {
    Serial.println("Could not find AHT");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");
  
  // Initialize WiFi and ThingSpeak
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);
}

void loop() {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");

  // Set ThingSpeak fields with sensor values
  ThingSpeak.setField(1, temperature);  // Set temperature in field 1
  ThingSpeak.setField(2, humidity);     // Set humidity in field 2

  // Write to ThingSpeak
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(20000); // Wait 20 seconds before the next update
}
