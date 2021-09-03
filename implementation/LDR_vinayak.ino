#define LDR_PIN 15 
#define LED 2

int LDR_Value = 0;
void LDR_Switch();

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 16;


void setup() {
Serial.begin(115200); 

ledcSetup(ledChannel, freq, resolution);
ledcAttachPin(5, ledChannel);  
pinMode(LDR_PIN,INPUT);
pinMode(LED,OUTPUT);
}
void loop() {
LDR_Switch();
//LDR_intensity();
delay(100);
}

void LDR_Switch()
{
LDR_Value = analogRead(LDR_PIN); 
Serial.println(LDR_Value);
if(LDR_Value > 500)
{
  digitalWrite(LED,HIGH);
  Serial.println(LDR_Value);
  Serial.println("LED ON\n"); 
}
  else
  {
    Serial.println(LDR_Value);
    digitalWrite(LED,LOW);
    Serial.println("LED OFF\n");
  }
  delay(100); 
}

void LDR_intensity()
{
  LDR_Value = analogRead(LDR_PIN);
  Serial.print("LDR_Value :");
  Serial.println(LDR_Value);
  ledcWrite(ledChannel, LDR_Value);
  delay(100);
}
