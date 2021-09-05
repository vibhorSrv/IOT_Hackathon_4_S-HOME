

 
//Interfacing PIR sensor using ESP module/
#include <WiFi.h> ///change according to your board
#include "ThingSpeak.h"

const char* ssid= "Galaxy M30s"; //wifi ssid
const char* pass= "12340987"; ///wifi password

WiFiClient client;

unsigned long mychannelid= 1493115;
const char * mywriteapikey = "MU5V1NWLYHL8F7QJ";
int mychannelfield= 1;

int LED= 26;
int sensor=27;
int state=LOW;
int val=0;


void setup() {
  // put your setup code here, to run once:
pinMode(LED,OUTPUT);
pinMode(sensor,INPUT);
Serial.begin(115200);
WiFi.mode(WIFI_STA);
ThingSpeak.begin(client);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status() !=WL_CONNECTED)
  {
    Serial.print("Trying to connect");
    while(WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid,pass);
      delay(500);
    }
    Serial.print("\nConnected");
    }
    val=digitalRead(sensor);
    if(val==HIGH)
    {
      Serial.print("PIR Sensor is high: ");
      Serial.println(val);
      digitalWrite(LED,LOW);
      ThingSpeak.writeField(mychannelid,mychannelfield,val,mywriteapikey);
      delay(500);
    }
    if(val==LOW)
    {
      Serial.print("PIR Sensor is  low: ");
      Serial.println(val);
      digitalWrite(LED,HIGH);
      ThingSpeak.writeField(mychannelid,mychannelfield,val,mywriteapikey);
      delay(500);
    }
   
  }
