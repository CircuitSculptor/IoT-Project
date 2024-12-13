# Smart Bedroom IoT Project

## Description
My project is all about controlling everything that may be found in a bedroom. 

### Components
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
- In my Smart Bedroom where will be **2 temperature and humidity sensors**, one will be placed inside the bedroom where it can measure the entire room effectively and one will be outside. To          protect the sensor from rain or direct sunlight, it is common practise to use a box with vent holes called a **Stevenson Screen**.
  
  - My project features 2 types of sensors to gather temperature and humidity data.
      - One sensor is a AHT20 that uses the **I²C** protocol. More on I²C [here](#i2c)
      attach image
      - One  sensor is a DHT11 that is less advanced so uses a 1-wire comminication standard but is easier to get running. 
        attach image
    - Both sensors have libraries that help in interfacing them to the ESP32.


  - In my project I wanted to determine if its getting dark outside or if its the morning. Later on in the project I will use this data to drive a stepper motor that will open and close roller blinds that I have in my bedroom.
      - The sensor I will be using is the DFRobot_B_LUX_V30B light intensity sensor. It also uses the **I²C** protocol. This is the beauty of **I²C**, the ESP32 only needs 2 wires to connect 2 sensors and I can add more **I²C** sensors to my project, give them power and connect them to the **I²C** bus. More on I²C [here](#i2c)
        
      - The light value that it collects is called Lux. The sensor can can measure from 0-200k Lux, my phones flashlight gets to above 100k Lux, which is impressive. I have not yet tested the sensor at night so cannot say what will be the lowest value I will accept as being dark outside. I will be doing some tests and simulations in the future.
      - When I will find the range of Lux values that I can work with, I plan to use this sensor to automatically close the blinds when the sun sets and their is no point of having them open. Annd the same for the morning, the sensor will start to read that the Lux value is slowly climbing, I will use the data with a delay to open the blinds and wake you up
  
      - It would be cool to make a simple alarm clock using the Lux value or get time data from a **NTP Server (Network Time Protocol)** to open the blinds and help in waking you up when the sun is shining into your eyes, especially in the summer monrings.


  - For controlling the blinds for my project, I settled on using this inexpesive stepper motor and comes with a driver board that will interface with the ESP32 to control speed and direction of the motor.
    - The Stepper Motor is a 28byj-48 model paired with a ULN2003 driver. The motor will then interface somehow into my blinds. I have looked into 3D-Printing a bracket and gears to drive the blinds directly where they hang from the wall.
    - To Drive this motor, the esp32 sends patterns of signals on the IN1, IN2, IN3 and IN4 control pins on the ULN2003 driver. The driver needs to be powered from a external power supply as the USB power from a computer is not enough to drive the motor safely.


  - Using the 2 buttons I plan to have a physical way of moving the blinds up and down besides the webserver's digital buttons
    - Each button has a 10kΩ pull-up resistor. It is their to ensure that the ESP32 has a defined starting point for the state of the button.
  (attach image here)


## I2C
- The I²C protocol is fascinating as you only need power and 2 data wires to communicate with sensors and you can daisy-chain multiple sensors on the same bus as long as you dont use 2 identical      sensors, if a sensor has multiple i2c addresses then some changes to the code and the sensors will work with eachother.  
- The data is sent using **SDA (Serial Data)** and **SCL (Serial Clock)**.

**More to come in the future**



  
