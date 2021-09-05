/* Essentials */
#include<DHT.h>
#define SERIAL_BAUD_RATE 115200

/******************************************************************/

/* Sensors */

#define PIN_LDR 17

#define PIN_DHT 15

/******************************************************************/

/* Actuators */

#define PIN_LED_LDR 5

/******************************************************************/

/* Values */

int value_LDR = 0;
float value_humidity;
float value_temperature;

/* Constants */

/******************************************************************/
/**
   Function prototypes
*/
void Init_SerialMonitor();
void Loop_UploadAllData();

void Init_PinModesAndSensors();

void Loop_GatherData_LDR();
void Loop_TakeDecision_LIGHTING(int ldr_value, bool isMotionDetected);

bool Loop_GatherData_DHT11();
void Loop_TakeDecision_DHT11();

void Loop_UploadAllData();

/* Initialization functions */
DHT dht(PIN_DHT, DHT11);

/**
   Initializes the Serial Monitor
*/
void Init_SerialMonitor()
{
  Serial.begin(SERIAL_BAUD_RATE);
}

/**
   Initializes pin modes and sensors
*/
void Init_PinModesAndSensors()
{
  //LDR init
  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_LED_LDR, OUTPUT);
  //DHT Init
  pinMode(PIN_DHT, INPUT);
  dht.begin();

}

/******************************************************************/
/* Looping Functions */

/*
   Gather analog data from LDR Sensor
*/
void Loop_GatherData_LDR()
{

  value_LDR = analogRead(PIN_LDR);
  Serial.print("LDR Sensor Data: ");
  Serial.println(value_LDR);
  Serial.println();
}

/**
   Take Decision for room lighting
*/
void Loop_TakeDecision_LIGHTING(int ldr_value, bool isMotionDetected)
{
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
  value_humidity = dht.readHumidity();
  value_temperature = dht.readTemperature();
  if (isnan(value_humidity) || isnan(value_temperature))
  {
    Serial.println("Failed to read data from DHT11");
    return false;
  }
  else
  {
    Serial.print("Temprature: ");
    Serial.println(value_temperature);
    Serial.print("Humidity: ");
    Serial.println(value_humidity);
    Serial.println();
    return true;
  }
}

void Loop_TakeDecision_DHT11()
{
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
}


void Loop_UploadAllData()
{
  //TODO implement
}

/* */

void setup() {
  Init_SerialMonitor();
  Init_PinModesAndSensors();


}

void loop() {
  Loop_GatherData_LDR();
  Loop_TakeDecision_LIGHTING(value_LDR, true);//

  if (Loop_GatherData_DHT11())
  {
    Loop_TakeDecision_DHT11();
  }

  Loop_UploadAllData();
}
