
#include <DFRobot_B_LUX_V30B.h>
DFRobot_B_LUX_V30B    myLux(19);//The sensor chip is set to 13 pins, SCL and SDA adopt default configuration

void setup() {
  Serial.begin(115200);
  myLux.begin();

}

void loop() {
  Serial.print("value: ");
  Serial.print(myLux.lightStrengthLux());
  Serial.println(" (lux).");
  delay(500);
}