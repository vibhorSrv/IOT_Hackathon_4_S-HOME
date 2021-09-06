/**
   IOT Smart Home Automation Project

   Authors:- Vibhor, Vinayak, Abhishek, Zeba, Anjali

*/

/* Essentials */
#include<DHT.h>
#include<WiFi.h>
#include<ThingSpeak.h>
#define SERIAL_BAUD_RATE 115200
#define DEBUG 1
#define LOG(msg) { if(DEBUG) Serial.println(String("**")+msg);}

/* WIFI Properties */
WiFiClient client;
const char *WiFi_SSID = "NETGEAR33";
const char *WiFi_Password = "sweetelephant067";

/* ThingSpeak Properties */
unsigned long thingspeak_ChannelID = 1497698;
const char * thingspeak_WriteAPIKey = "4MGWZM2JL5YNKEFY";

unsigned long thingspeak_ReadChannelID = 1497857;
const char * thingspeak_ReadAPIKey = "OHBL04EQUGVURB54";

/******************************************************************/

/* Sensors Pins */

#define PIN_LDR 36

#define PIN_DHT 15

#define PIN_GAS 14

#define PIN_ULTRASONIC_TRIG 4
#define PIN_ULTRASONIC_ECHO 18

#define PIN_PIR 27

/******************************************************************/

/* Actuators */

#define PIN_LED_LDR 5         //LED controlled by LDR input value
#define PIN_LED 19            //LED/Buzzer to notify water tank level
#define PIN_LIGHT1 16
#define PIN_LIGHT2 21

/******************************************************************/
/** Values Downloaded from cloud */

int status_light1;            //Current state of light1 switch as received from cloud
int status_light2;            //Current state of light2 switch as received from cloud
int status_fan1;              //Current state of fan1 switch as received from cloud
int status_fan2;              //Current state of fan2 switch as received from cloud


/* Values to be Uploaded */

int value_LDR = 0;            //Light intensity measured by LDR

float value_humidity = 0;     //humidity value measured by DHT11
float value_temperature = 0;  //temperature value measured by DHT11

float value_gas;              //gas value measured by gas sensor

int Full_Tank = 10;           //setting full tank level distance 10cm
int Mid_Tank = 100;           // tank filed at middle level
int Empty_Tank = 200;
long T;
float value_distanceCM;       //distance measured by ultrasonic sensor



int value_pirMotionStatePrevious = LOW; // previous  state of motion sensor's pin
int value_pirMotionStateCurrent = LOW; // current  state of motion sensor's pin


/* Constants */

/******************************************************************/
/**
   Function prototypes
*/
void Init_SerialMonitor();
void ConnectToWiFi();
void ThingSpeakBegin();

bool Loop_ThingSpeakReadAllFields();
void Loop_UpdateApplianceStates();

void Init_PinModesAndSensors();

void Loop_GatherData_LDR();
void Loop_TakeDecision_LIGHTING(int ldr_value, bool isMotionDetected);

bool Loop_GatherData_DHT11();
void Loop_TakeDecision_DHT11();

void Loop_Ultra_Sonic_Distance();
void Loop_TakeDecision_UltraSonic();

void Loop_GatherData_Gas();
void Loop_TakeDecision_Gas();

void Loop_GatherData_PIR();
void Take_Decision_PIR();

void Loop_UploadAllData();

String booleanToString(bool b);

/* Initialization functions */
DHT dht(PIN_DHT, DHT11);

/**
   Initializes the Serial Monitor
*/
void Init_SerialMonitor()
{
  Serial.begin(SERIAL_BAUD_RATE);
  LOG(__func__)
}

/**
   Connects the ESP32 to the WiFi
*/
void ConnectToWiFi()
{
  LOG(__func__);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WiFi_SSID, WiFi_Password);
  Serial.print("Connecting to ");
  Serial.println(WiFi_SSID);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(500);
  }
  Serial.println("WiFi Connected!!");
  Serial.print("My IP address is: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.println();
}

void ThingSpeakBegin()
{
  ThingSpeak.begin(client);
}
/**
   Initializes pin modes and sensors
*/
void Init_PinModesAndSensors()
{
  LOG(__func__)
  //LDR init
  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_LED_LDR, OUTPUT);
  //DHT Init
  pinMode(PIN_DHT, INPUT);
  dht.begin();
  //Ultra sonic
  pinMode(PIN_ULTRASONIC_TRIG, OUTPUT);
  pinMode(PIN_ULTRASONIC_ECHO, INPUT);
  //PIR
  pinMode(PIN_PIR, INPUT);
  //GAS
  pinMode(PIN_GAS, INPUT);
  //Lights
  pinMode(PIN_LIGHT1, OUTPUT);
  pinMode(PIN_LIGHT2, OUTPUT);
  Serial.println();

}

/******************************************************************/
/* Looping Functions */


/**
    Read All the fields from the ThingSpeak Channel (It is for user remote control)

    returns true if successfully read all the fields else false
*/
bool Loop_ThingSpeakReadAllFields()
{
  LOG(__func__)

  if (ThingSpeak.readMultipleFields(thingspeak_ReadChannelID, thingspeak_ReadAPIKey) == 200)
  {
    status_light1 = ThingSpeak.getFieldAsInt(1);
    status_light2 = ThingSpeak.getFieldAsInt(2);
    status_fan1 = ThingSpeak.getFieldAsInt(3);
    status_fan2 = ThingSpeak.getFieldAsInt(4);
    Serial.println("Reading Data Successful from cloud!\n");
    return true;
  } else
  {
    Serial.println("Reading Data Failed from cloud!\n");
    return false;
  }

}

/**
   Updates all the appliance's on off state based on the data received from the ThingSpeak cloud
*/
void Loop_UpdateApplianceStates()
{
  LOG(__func__)

  digitalWrite(PIN_LIGHT1, (uint8_t)status_light1); //This will be a relay connected to Appliance's switch
  Serial.println("Setting LIGHT 1 :->" + booleanToString(status_light1));

  digitalWrite(PIN_LIGHT2, (uint8_t)status_light2);
  Serial.println("Setting LIGHT 1 :->" + booleanToString(status_light2));

  //digitalWrite(PIN_FAN1, (uint8_t)status_fan1);
  Serial.println("Setting FAN 1 :->" + booleanToString(status_fan1));

  //digitalWrite(PIN_FAN2, (uint8_t)status_fan2);
  Serial.println("Setting FAN 2 :->" + booleanToString(status_fan2));


}

String booleanToString(bool b)
{
  return (b ? String("ON") : String("OFF"));
}
/*
   Gather analog data from LDR Sensor
*/
void Loop_GatherData_LDR()
{
  LOG(__func__)
  value_LDR = analogRead(PIN_LDR);
  Serial.print("LDR Sensor Data: ");
  Serial.println(value_LDR);
  Serial.println();
  delay(500);
}

/**
   Take Decision for room lighting
*/
void Loop_TakeDecision_LIGHTING(int ldr_value, bool isMotionDetected)
{
  LOG(__func__)
  if (ldr_value > 500 && isMotionDetected) // and motion detected then turn on light
  {
    //Call the Actuator
    digitalWrite(PIN_LED_LDR, HIGH);
    Serial.print(ldr_value);
    Serial.println(";Lights ON");

  }
  else
  {
    //Call the Actuator
    digitalWrite(PIN_LED_LDR, LOW);
    Serial.print(ldr_value);
    Serial.println(";Lights OFF");
  }
  Serial.println();
  delay(1000);
}

/**
   Read Data from the DHT11 Sensor
*/
bool Loop_GatherData_DHT11()
{
  LOG(__func__)
  value_humidity = dht.readHumidity();
  value_temperature = dht.readTemperature();
  bool retval = false;
  if (isnan(value_humidity) || isnan(value_temperature))
  {
    Serial.println("Failed to read data from DHT11");
    Serial.println();
    retval = false;
  }
  else
  {
    Serial.print("Temprature: ");
    Serial.println(value_temperature);
    Serial.print("Humidity: ");
    Serial.println(value_humidity);
    Serial.println();
    retval = true;
  }
  delay(500);
  return retval;
}


/**
   Take Decision based on DHT11 Sensor
*/
void Loop_TakeDecision_DHT11()
{
  LOG(__func__)
  //Decision Temperature
  if (value_temperature > 30)
  {
    Serial.println("Hot : Fan Speed -> Fast");
    //Call the Actuator
  }
  else if (value_temperature < 30 && value_temperature > 22)
  {
    Serial.println("Comfortable : Fan Speed -> Normal");
    //Call the Actuator
  }
  else
  {
    Serial.println("Cold : Fan Speed -> OFF");
    //Call the Actuator
  }

  //Decision Humidity
  if (value_humidity < 80)
  {
    Serial.println("Comfortable : AC Temperature -> 27");
    //Call the Actuator
  }
  else
  {
    Serial.println("Too Humid : AC Temperature -> 25");
    //Call the Actuator
  }
  Serial.println();
  delay(1000);
}

/**
   Gather Data from Ultra sonic sensor
*/
void Loop_Ultra_Sonic_Distance()
{
  LOG(__func__)
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
  T = pulseIn(PIN_ULTRASONIC_ECHO, HIGH);
  value_distanceCM = T * 0.034 / 2;
  //  value_distanceCM = value_distanceCM / 2;/
  Serial.print("Distance in CM : ");
  Serial.println(value_distanceCM);
  Serial.println();
  delay(500);
}

/**
   Take Decision based on Untra sonic sensor output
*/
void Loop_TakeDecision_UltraSonic()
{
  LOG(__func__)
  if (value_distanceCM <= Full_Tank)
  {
    Serial.println("Tank is Full turning off switch");
    digitalWrite(PIN_LED, HIGH); // turn off switch
    //thingspeak
  }
  else if (value_distanceCM <= Mid_Tank)
  {
    Serial.println("Tank is Mid Level turning ON switch");
    digitalWrite(PIN_LED, HIGH); // turn off switch
    //thingspeak
  }
  else if (value_distanceCM >= Empty_Tank)
  {
    Serial.println("Tank is Empty turning ON switch");
    digitalWrite(PIN_LED, HIGH); // turn off switch
    //thingspeak
  }
  Serial.println();
  delay(1000);
}


/**
   Read values from Gas Sensor
*/
void Loop_GatherData_Gas()
{
  LOG(__func__)
  Serial.println("Read From GAS Sensor");
  float gas_analog_value = analogRead(PIN_GAS);
  value_gas = ((gas_analog_value / 1023) * 100);
  Serial.println(value_gas);
  Serial.println();
  delay(500);
}

/**
   Take Decision bases on Gas Sensor
*/
void Loop_TakeDecision_Gas()
{
  LOG(__func__)
  if (value_gas > 40.0)
  {
    Serial.print("Alert!! Gas LEVEL is HIGH: "); //Gas Detected
    Serial.println(value_gas);
    //Call the Actuator if needed
  }
  else
  {
    Serial.print("Gas LEVEL is Normal");
  }
  Serial.println();
  delay(1000);
}

/**
   Read values from PIR motion Sensor
*/
void Loop_GatherData_PIR()
{
  LOG(__func__)
  Serial.println("Read From PIR");
  value_pirMotionStatePrevious = value_pirMotionStateCurrent;// store old state
  value_pirMotionStateCurrent  = digitalRead(PIN_PIR); // read new state
  Serial.println(value_pirMotionStateCurrent);
  Serial.println();
  delay(500);
}

/**
   Take Decision Motion Sensor value
*/
void Loop_TakeDecision_PIR()
{
  LOG(__func__)
  if (value_pirMotionStatePrevious == LOW && value_pirMotionStateCurrent == HIGH) // pin state change: LOW -> HIGH
  {
    Serial.print("Motion detected!"); //Motion Detected
    Serial.println(value_pirMotionStateCurrent);
    //Call the Actuator if needed
  }
  else if (value_pirMotionStatePrevious == HIGH && value_pirMotionStateCurrent == LOW) // pin state change: HIGH -> LOW
  {
    Serial.print("Motion stopped!");//No Motion Detected
    Serial.println(value_pirMotionStateCurrent);
    //Call the Actuator if needed
  }
  else {
    Serial.print("No Motion detected!");//No Motion Detected
    Serial.println(value_pirMotionStateCurrent);
  }
  Serial.println();
  delay(1000);
}

/**
   Sets all fields to the thingspeak instance and uploads them all at once
   This Function should be called only when data has been collected from all the sensors
*/
void Loop_UploadAllData()
{
  LOG(__func__)
  ThingSpeak.setField(1, value_LDR); // light intensitty
  ThingSpeak.setField(2, value_temperature); // room temperature
  ThingSpeak.setField(3, value_humidity); // humidity
  ThingSpeak.setField(4, value_distanceCM); // room temperature
  ThingSpeak.setField(5, value_gas); //Gas Sensor value
  ThingSpeak.setField(6, value_pirMotionStateCurrent); //Motion sensor value
  if (ThingSpeak.writeFields(thingspeak_ChannelID, thingspeak_WriteAPIKey) == 200)
  {
    Serial.println("All Data fields Successfully uploaded!");
  }
  else
  {
    Serial.println("Data Uploading Failed");
  }
  Serial.println();
}

/* */

void setup() {
  Init_SerialMonitor();
  ConnectToWiFi();
  ThingSpeakBegin();
  Init_PinModesAndSensors();

}

void loop() {
  //Control
  if (Loop_ThingSpeakReadAllFields())
  {
    Loop_UpdateApplianceStates();
    delay(5000);
  }
  //Monitoring
  if (Loop_GatherData_DHT11())
  {
    Loop_TakeDecision_DHT11();
  }

  Loop_Ultra_Sonic_Distance();
  Loop_TakeDecision_UltraSonic();

  Loop_GatherData_PIR();
  Loop_TakeDecision_PIR();

  Loop_GatherData_LDR();
  Loop_TakeDecision_LIGHTING(value_LDR, value_pirMotionStateCurrent);

  Loop_GatherData_Gas();
  Loop_TakeDecision_Gas();

  Loop_UploadAllData();
  Serial.println("======= Cycle Complete =======\n");
  delay(10000);//10 seconds

}
