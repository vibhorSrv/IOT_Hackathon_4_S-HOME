#define trigger 5
#define echo 18
#define LED 2

float calibrate;  //to set fix distance (Threshold)
int Person_count = 0;  // to count people in room  
int Full_Tank = 10; //setting full tank level distance 10cm
int Mid_Tank = 100; // tank filed at middle level
int Empty_Tank = 200;// tank is empty

long T;
float distanceCM;

void Ultra_Sonic_Distance();

void setup() {
  
  Serial.begin(115200);
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
   
}

void loop() {
  // put your main code here, to run repeatedly:
  Ultra_Sonic_Distance();
  delay(1000);
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
  else
  if(distanceCM <= Mid_Tank)
  {
      Serial.println("Tank is Mid Level turning ON switch");
      digitalWrite(LED,HIGH); // turn off switch
      //thingspeak
  }
  else
  if(distanceCM <= Empty_Tank)
  {
      Serial.println("Tank is Empty turning ON switch");
      digitalWrite(LED,HIGH); // turn off switch
      //thingspeak
  } 
  
}
