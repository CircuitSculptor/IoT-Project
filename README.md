# Smart Bedroom IoT Project

## Description
My project is all about controlling everything that may be found in a bedroom. 

### Components for Project
- 1x ESP32 development board
- 1x AHT20 Temperature and Humidity Sensor
- 1x DHT11 Temperature and Humidity Sensor
- 1x DFRobot_B_LUX_V30B Lux Sensor
- 1x 28byj-48 Stepper Motor
- 1x ULN2003 Stepper Motor Driver
- 1x HC-SR501 Motion Sensor
- 1x MG58 Servo Motor
- 2x Momentary Push Button (with 10kΩ pull-up resistor)
- 1x LED (with 330Ω resistor)

### Features
- In my Smart Bedroom where will be **2 temperature and humidity sensors**, one will be placed inside the bedroom where it can measure the entire      room effectively and one will be outside. To protect the sensor from rain or direct sunlight, it is common practise to use a box with vent holes 
  called a **Stevenson Screen**.
  
  - My project features 2 types of sensors to gather temperature and humidity data.
      - One is a AHT20 sensor that uses the **I²C** protocol. More on I²C [here](I²C)
      attach image
      - One is a DHT11 sensor that is less advanced so uses a 1-wire comminication standard but is easier to get running. 
        attach image
    - Both sensors have libraries that help in interfacing them to the ESP32. Please see the [Library List]() on all the required libraries to install.
  
- Gather Light Intensity from a dedicated sensor
  It is the DFRobot_B_LUX_V30B sensor.
  The data in Lux is displayed on a webserver and will be used to determine if the blinds controlled by a stepper motor will open or closed.
  If the sensor reads a low Lux value like less than 10, it is dark outside and gradually close the blinds.
  If the sensor reads a high Lux value that can max out at 200k Lux, it will open the blinds.
  I plan to use the Lux sensor as a alarm clock that will gradually open the blinds to let in the morning sun into your room and hopefully wake you up.
  (attach image here)

- The Stepper Motor controlling the blinds is a 28byj-48 model paired with a ULN2003 driver
  To Drive this motor, the esp32 sends patterns of signals on the IN1, IN2, IN3 and IN4 control pins on the ULN2003 driver. The driver needs to be powered from a external power supply as the USB power from a computer is not enough to drive the motor safely.
  (attach image here)

- Using the 2 buttons I plan to have a physical way of moving the blinds up and down besides the webserver's digital buttons
  Each button has a 10kΩ pull-up resistor. It is their to ensure that the ESP32 has a defined starting point for the state of the button.
  (attach image here)

## Library List

### I²C
- The i2c protocol is fascinating as you only need power and 2 data wires to communicate with sensors and you can daisy-chain multiple sensors on the same bus as long as you dont use 2 identical sensors, if a sensor has multiple i2c addresses then some changes to the code and the sensors will work with eachother.  
-  The data is sent usign **SDA (Serial Data)** and **SCL (Serial Clock)**.

**More to come in the future**



  
