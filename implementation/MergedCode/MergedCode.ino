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
unsigned long thingspeak_ChannelID = 000000;
const char * thingspeak_WriteAPIKey = "keykeykey";

unsigned long thingspeak_ReadChannelID = 000000;
const char * thingspeak_ReadAPIKey = "keykeykey";
/******************************************************************/

/* Sensors */

#define PIN_LDR 17

#define PIN_DHT 15

#define PIN_ULTRASONIC_TRIG 4
#define PIN_ULTRASONIC_ECHO 18

#define PIN_PIR 27

/******************************************************************/

/* Actuators */

#define PIN_LED_LDR 5

/******************************************************************/
/** Values Downloaded from cloud */

int status_light1;
int status_light2;
int status_fan1;
int status_fan2;


/* Values to be Uploaded */

int value_LDR = 0;

float value_humidity = 0.0f;
float value_temperature = 0.0f;

int Full_Tank = 10; //setting full tank level distance 10cm
long T;
float value_distanceCM;

int value_pir = LOW;


/* Constants */

/******************************************************************/
/**
   Function prototypes
*/
void Init_SerialMonitor();
void ConnectToWiFi();
void Loop_UploadAllData();
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

void Loop_GatherData_PIR();
void Take_Decision_PIR();

void Loop_UploadAllData();

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

}

/******************************************************************/
/* Looping Functions */


bool Loop_ThingSpeakReadAllFields()
{
  LOG(__func__)

  if (ThingSpeak.readMultipleFields(thingspeak_ReadChannelID, thingspeak_ReadAPIKey) == 200)
  {
    status_light1 = ThingSpeak.getFieldAsInt(1);
    status_light2 = ThingSpeak.getFieldAsInt(2);
    status_fan1 = ThingSpeak.getFieldAsInt(3);
    status_fan2 = ThingSpeak.getFieldAsInt(4);
    return true;
  }
  return false;

}

void Loop_UpdateApplianceStates()
{
  LOG(__func__)

  //digitalWrite(PIN_LIGHT1, (uint8_t)status_light1);
  Serial.println("Setting LIGHT 1 :->" + booleanToString(status_light1));

  //digitalWrite(PIN_LIGHT2, (uint8_t)status_light2);
  Serial.println("Setting LIGHT 1 :->" + booleanToString(status_light2));

  //digitalWrite(PIN_FAN1, (uint8_t)status_fan1);
  Serial.println("Setting FAN 1 :->" + booleanToString(status_fan1));

  //digitalWrite(PIN_FAN2, (uint8_t)status_fan2);
  Serial.println("Setting FAN 2 :->" + booleanToString(status_fan2));

  Serial.println();

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
    digitalWrite(PIN_LED_LDR, HIGH);
    Serial.print(ldr_value);
    Serial.println(";Lights ON");
  }
  else
  {
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
  }
  else if (value_temperature < 30 && value_temperature > 22)
  {
    Serial.println("Comfortable : Fan Speed -> Normal");
  }
  else
  {
    Serial.println("Cold : Fan Speed -> OFF");
  }

  //Decision Humidity
  if (value_humidity < 80)
  {
    Serial.println("Comfortable : AC Temperature -> 27");
  }
  else
  {
    Serial.println("Too Humid : AC Temperature -> 25");
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
  delay(1);
  digitalWrite(PIN_ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
  T = pulseIn(PIN_ULTRASONIC_ECHO, HIGH);
  value_distanceCM = T * 0.034;
  value_distanceCM = value_distanceCM / 2;
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
  }
  else
  {
    Serial.println("Tank is NOT Full Yet");
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
  value_pir = digitalRead(PIN_PIR);
  Serial.println();
  delay(500);
}

/**
   Take Decision Motion Sensor value
*/
void Loop_TakeDecision_PIR()
{
  LOG(__func__)
  if (value_pir == HIGH)
  {
    Serial.print("PIR Sensor is high: "); //Motion Detected
    Serial.println(value_pir);
  }
  else if (value_pir == LOW)
  {
    Serial.print("PIR Sensor is  low: ");//No Motion Detected
    Serial.println(value_pir);
  }
  Serial.println();
  delay(1000);
}


void Loop_UploadAllData()
{
  LOG(__func__)
  ThingSpeak.setField(1, value_LDR); // light intensitty
  ThingSpeak.setField(2, value_temperature); // room temperature
  ThingSpeak.setField(3, value_humidity); // humidity
  ThingSpeak.setField(4, value_distanceCM); // room temperature
  ThingSpeak.setField(5, 0); //Gas Sensor value
  ThingSpeak.setField(6, value_pir); //Motion sensor value
  ThingSpeak.writeFields(thingspeak_ChannelID, thingspeak_WriteAPIKey);
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
  if (Loop_ThingSpeakReadAllFields())
  {
    Loop_UpdateApplianceStates();
    delay(5000);
  }

  if (Loop_GatherData_DHT11())
  {
    Loop_TakeDecision_DHT11();
  }

  Loop_Ultra_Sonic_Distance();
  Loop_TakeDecision_UltraSonic();

  Loop_GatherData_PIR();
  Loop_TakeDecision_PIR();

  Loop_GatherData_LDR();
  Loop_TakeDecision_LIGHTING(value_LDR, value_pir);

  Loop_UploadAllData();
  Serial.println("======= cycle complete =======\n");
  delay(10000);//10 seconds

}
