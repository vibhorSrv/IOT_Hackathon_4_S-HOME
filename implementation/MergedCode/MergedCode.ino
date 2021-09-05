/* Essentials */
#define SERIAL_BAUD_RATE 115200

/******************************************************************/

/* Sensors */

#define PIN_LDR 17

/******************************************************************/

/* Actuators */

#define PIN_LED_LDR 5

/******************************************************************/

/* Values */

int value_LDR = 0;


/* Constants */

/******************************************************************/

/* Initialization functions */

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
void Init_PinModesAndSensors_LDR()
{
  //LDR init
  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_LED_LDR, OUTPUT);

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

/* */

void setup() {
  Init_SerialMonitor();
  Init_PinModesAndSensors_LDR();


}

void loop() {
  Loop_GatherData_LDR();
  Loop_TakeDecision_LIGHTING(value_LDR, true);//
}
