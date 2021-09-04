#define LDR_PIN 15 
#define LED 2

int LDR_Value = 0;
void LDR_Switch();

const int freq = 5000;// frequency to 5000Hz
const int ledChannel = 0;//using channel 0
const int resolution = 16;// setting resolution of 16 bit to get clean variationn


void setup() {
Serial.begin(115200); 

ledcSetup(ledChannel, freq, resolution);  //configure PWM with required settings
ledcAttachPin(5, ledChannel);  //attaching PWM channel to GPIO pin

pinMode(LDR_PIN,INPUT);
pinMode(LED,OUTPUT);
}
void loop() {
LDR_Switch();
//LDR_intensity();
delay(100);
}

void LDR_Switch()   // LDR as a switch
{  
  
LDR_Value = analogRead(LDR_PIN);  
Serial.println(LDR_Value);

  if(LDR_Value > 500 ) // and motion detected then turn on light
  {
    digitalWrite(LED,HIGH);
    Serial.println(LDR_Value);
    Serial.println("LED ON\n");
    //thingspeak  
  }
  else
  {
    Serial.println(LDR_Value);
    digitalWrite(LED,LOW);
    Serial.println("LED OFF\n");
    //thingspeak
  }
  delay(100); 
}

void LDR_intensity()  // based on LDR value change LED value.
{
  LDR_Value = analogRead(LDR_PIN);
  
  Serial.print("LDR_Value :");
  Serial.println(LDR_Value);
  ledcWrite(ledChannel, LDR_Value);
  delay(100);
  //thingspeak 
}
