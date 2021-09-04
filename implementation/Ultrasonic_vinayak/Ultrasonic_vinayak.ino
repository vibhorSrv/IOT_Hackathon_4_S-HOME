#define trigger 5
#define echo 18
#define LED 2

float calibrate;  //to set fix distance (Threshold)
int Person_count = 0;  // to count people in room  
int Full_Tank = 10; //setting full tank level distance 10cm
long T;
float distanceCM;

void Ultra_Sonic_Distance();
int Ultra_Sonic_Count();

void setup() {
Serial.begin(115200);
pinMode(trigger,OUTPUT);
pinMode(echo,INPUT);

//Set first distance as a refference distnce to calculate object count
  digitalWrite(trigger,LOW);
  delay(1);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  T=pulseIn(echo,HIGH);
  calibrate = T * 0.034;
  calibrate = calibrate / 2;
  Serial.print("calibrate : ");
  Serial.println(calibrate);

}

void loop() {
  // put your main code here, to run repeatedly:
  //Ultra_Sonic_Count();
  Ultra_Sonic_Distance();
  delay(1000);
}

int Ultra_Sonic_Count()
{
  digitalWrite(trigger,LOW);
  delay(1);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  T=pulseIn(echo,HIGH);
  distanceCM =T * 0.034;
  distanceCM = distanceCM / 2;
  Serial.print("Distance in CM : ");
  Serial.println(distanceCM);
  
  if(distanceCM < calibrate)
  {
    Person_count++;
    Serial.print("Number of people in Room : ");
    Serial.println(Person_count);
  } 
}

void Ultra_Sonic_Distance()
{
  digitalWrite(trigger,LOW);
  delay(1);
  digitalWrite(trigger,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger,LOW);
  T=pulseIn(echo,HIGH);
  distanceCM =T * 0.034;
  distanceCM = distanceCM / 2;
  Serial.print("Distance in CM : ");
  Serial.println(distanceCM);

  if(distanceCM <= Full_Tank)
  {
      Serial.println("Tank is Full turning off switch");
      digitalWrite(LED,HIGH); // turn off switch
      //thingspeak
  } 
}
