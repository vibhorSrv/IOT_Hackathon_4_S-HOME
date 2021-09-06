const int MOTION_SENSOR_PIN = 22;   // ESP32 pin connected to the OUTPUT pin of motion sensor
const int LED_PIN           = 23;   // ESP32 pin connected to LED's pin
int motionStateCurrent      = LOW; // current  state of motion sensor's pin
int motionStatePrevious     = LOW; // previous state of motion sensor's pin

void setup() {
  Serial.begin(115200);                // initialize serial
  pinMode(MOTION_SENSOR_PIN, INPUT); // set ESP32 pin to input mode
  pinMode(LED_PIN, OUTPUT);          // set ESP32 pin to output mode
}

void loop() {
  motionStatePrevious = motionStateCurrent;             // store old state
  motionStateCurrent  = digitalRead(MOTION_SENSOR_PIN); // read new state

  if (motionStatePrevious == LOW && motionStateCurrent == HIGH) { // pin state change: LOW -> HIGH
    Serial.println("Motion detected!");
    digitalWrite(LED_PIN, HIGH); // turn on
    delay(500);
  }
  else
  if (motionStatePrevious == HIGH && motionStateCurrent == LOW) { // pin state change: HIGH -> LOW
    Serial.println("Motion stopped!");
    digitalWrite(LED_PIN, LOW);
    delay(500);
    // turn off
  }
}
