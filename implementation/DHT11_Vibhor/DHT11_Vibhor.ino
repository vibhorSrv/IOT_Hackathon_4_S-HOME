/*
   Program for interfacing with DHT11 Temperature and Humidity Sensor
   Created by Vibhor
*/

#include<DHT.h>

#define PIN_DHT 15


/* Data from Sensors */
float value_humidity;
float value_temperature;


/* Other initialization */
DHT dht(PIN_DHT, DHT11);

void Init_SerialMonitor()
{
  Serial.begin(115200);
}

void Init_PinAndSensors()
{
  pinMode(PIN_DHT, INPUT);
  dht.begin();
}

void Loop_GatherData_DHT11()
{
  value_humidity = dht.readHumidity();
  value_temperature = dht.readTemperature();
  if (isnan(value_humidity) || isnan(value_temperature))
  {
    Serial.println("Failed to read data from DHT11");
  }
  else
  {
    Serial.print("Temprature: ");
    Serial.println(value_temperature);
    Serial.print("Humidity: ");
    Serial.println(value_humidity);
    Serial.println();
  }
}

void Loop_TakeDecision()
{
  //Decision Temperature
  if (value_temperature > 30 && value_temperature < 35)
  {
    Serial.println("Comfortable : Fan Speed -> Fast");
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

void setup()
{
  Init_SerialMonitor();
  Init_PinAndSensors();
}

void loop()
{
  // put your main code here, to run repeatedly:
  Loop_GatherData_DHT11();
  Loop_TakeDecision();
  Loop_UploadAllData();
  Serial.println("==================================");
  delay(1000);
}
