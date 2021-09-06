- ## Context: Smart Home Automation

  - Smart home appliances
  - IoT
  - Cloud Connectivity
  - Sensors

- ## Step 0: Ideation: 

  - #### Basic ideas

    - Smart control of home appliances
    - Automatic as well as Manual Control of appliances over the network
    - Detection and prevention of threats using sensors and alert mechanism
    - Making user's life easier by controlling appliances remotely
    - Energy saving (switching off appliances when there is no human presence or when lights/fan is not required)

  - #### Specific Use Cases

    - Environmental Conditions (Parameters)
      1. **Sunlight availability/Light intensity**
         
         - Can be used to -
           - turn on/off outdoor lights at sunset/sunrise (to save power/automation)
           - turn OFF indoor lights (if they are ON) and noone is present in room (to save power)
         
      2. **Room Temperature & Humidity**
         
         - Can be used to -
           - regulate fan speed (user comfort)
           - regulate Air Conditioning/Room Cooler speed (user comfort/power saving)
           - *also be used to detect fire in extreme cases* (user safety)
         
      3. **Motion**
         
         - Can be used to -
           - detect Human presence (intruder alert/user safety)
           - presence of human in a room (switch off appliances automatically)
         
      4. **Distance**
         
         - Can be used to -
           - detect water level in water storage tanks
           - detect level of garbage in garbage bins
           - car parking
         
      5. **Gas**
      
         - Can be used to - 
      
           - Detect any gas leakage happeing in the house and alert the user
      
           

  - #### Basic Flow Chart

    ![image-20210903101955919](res/flowchart.png)

  

- ## Step 1: Selection of Processor

  - Espressif Systems **ESP32**

    

- ## Step 2: Sensors, Actuators & Networking Link

  - #### Parameters to measure : Sensors
    
    - Temperature and Humidity : **DHT11** (Vibhor)
    - Light Intensity Sensing : **LDR Sensor** (Vinayak)
    - Gas Leakage: **MQ2 / MQ35** (Zeba)
    - Human Presence Detection: **PIR Sensor** (Abhishek)
    - Count the Number of people: **UltraSonic Sensor** (Vinayak)
  - #### Actuators
    
    - For Alarming the user: **Buzzer**
    - **Relay:** Fan/light/other appliances on off state
    - PWM we can control light intensity/fan speed (demo LED)
  - #### Network Link
    
    - Wifi Module inbuilt in ESP32

  

- ## Step 3: Cloud

  - #### Thingspeak Cloud

    - Single channel, Multiple fields 

  - #### Cloud Alternatives - 

    - Arduino Cloud

    - Blynk Cloud

    - Adafruit IO

    - AWS IoT (Amazon Web Services)

    - Firebase

      

  - #### Localhost Web Server
    
    - in case when there is no internet
    
    - Bluetooth connectivity
    
      

- ## Step 4: S/W Development

  - Programming separately for each sensor

  - Merging separate programs into one single code.

    

- ## Step 5: 

