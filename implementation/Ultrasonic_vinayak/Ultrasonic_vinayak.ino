#define trigger 5
#define echo 18
#define RED 13
#define Buzzer 18
#define White 12
#define Green 14



float calibrate;  //to set fix distance (Threshold)
int Person_count = 0;  // to count people in room  
int Full_Tank = 10; //setting full tank level distance 10cm
int Mid_Tank = 100; // tank filed at middle level
int Empty_Tank = 200;// tank is empty

int STOP_Car = 100; // car stop at 100 CM from sensor
long T;
float distanceCM;

void Ultra_Sonic_Distance();
void Ultra_Sonic_Car_Parking();

void setup() {
  
  Serial.begin(115200);
  pinMode(trigger,OUTPUT);
  pinMode(echo,INPUT);
  pinMode(Buzzer,OUTPUT);  
}

void loop() {
  // put your main code here, to run repeatedly:
  Ultra_Sonic_Distance();
 // Ultra_Sonic_Car_Parking();
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
      digitalWrite(RED,HIGH); // turn off switch
      //thingspeak
  }
  else
  if(distanceCM <= Mid_Tank)
  {
      Serial.println("Tank is Mid Level turning ON switch");
      digitalWrite(White,HIGH); // turn off switch
      //thingspeak
  }
  else
  if(distanceCM <= Empty_Tank)
  {
      Serial.println("Tank is Empty turning ON switch");
      digitalWrite(Green,HIGH); // turn off switch
      //thingspeak
  } 
  
}


void Ultra_Sonic_Car_Parking()
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

  if(distanceCM <= STOP_Car)
  {
      Serial.println("STOP The Car");
      digitalWrite(Buzzer,HIGH); // turn on Buzzer
      //thingspeak
  }
     
}
