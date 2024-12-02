#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

const int pirPin = 19;
int PIR = 0;

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

String myStatus = "";

void setup() {

  pinMode(pirPin, INPUT);
  Serial.begin(115200);  //Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
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
  
  // Read PIR sensor state
  int state = digitalRead(pirPin);
  if (state == HIGH) {
    Serial.println("Movement Detected");
    PIR = 1;
  } else {
    Serial.println("Monitoring...");
    PIR = 0;
  }

  // set the fields with the values
  ThingSpeak.setField(4, PIR);
  
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

/*
#include <WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // Always include ThingSpeak header file after other header files and custom macros

const int pirPin = 19;
int PIR = 0;            // Holds the current PIR state
int lastPIR = 0;        // Holds the last PIR state sent to ThingSpeak

char ssid[] = SECRET_SSID;   // Your network SSID (name) 
char pass[] = SECRET_PASS;   // Your network password
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char *myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  pinMode(pirPin, INPUT);
  Serial.begin(115200);  // Initialize serial
  while (!Serial) {
    ; // Wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }
  
  // Read PIR sensor state
  int state = digitalRead(pirPin);
  if (state == HIGH) {
    Serial.println("Movement Detected");
    PIR = 1; // Set PIR to 1 when movement is detected
  } else if (PIR == 1) {
    // Retain the PIR value until it is sent
    Serial.println("Waiting to send movement data...");
  } else {
    Serial.println("Monitoring...");
    PIR = 0; // Reset PIR only if no movement detected and data has been sent
  }

  // Only send to ThingSpeak if the PIR value has changed or 20 seconds have passed
  static unsigned long lastUpdateTime = 0;
  if (millis() - lastUpdateTime > 20000 || PIR != lastPIR) {
    ThingSpeak.setField(4, PIR);  // Send PIR state to field 4

    // Write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200) {
      Serial.println("Channel update successful.");
      lastPIR = PIR;         // Update the last sent PIR value
      lastUpdateTime = millis(); // Reset the update time
      if (PIR == 1) {
        PIR = 0;  // Reset PIR after successfully sending movement data
      }
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
  }

  delay(100); // Small delay to avoid busy-waiting
}

*/
