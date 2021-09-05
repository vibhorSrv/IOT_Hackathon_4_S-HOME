#include "ThingSpeak.h"
#include <WiFi.h>

//Replace your wifi credentials here
const char* ssid     = "Koshti_4G";
const char* password = "Koshti@11";

//change your channel number here
unsigned long channel =1494791;

unsigned int led1 = 1;

WiFiClient  client;


void setup() {
  Serial.begin(115200);
  delay(100);
  
  pinMode(2, OUTPUT);
  digitalWrite(2, 1);
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  ThingSpeak.begin(client);

}

void loop() {
 
  //get the last data of the fields
  int led_1 = ThingSpeak.readFloatField(channel, led1);
  Serial.print("led_1 : ");
  Serial.println(led_1);
  if(led_1 == 1){
    digitalWrite(2,HIGH);
    Serial.println("D1 is On..!");
  }
  else if(led_1 == 0){
    digitalWrite(2,LOW );
    Serial.println("D1 is Off..!");
  }
    
  Serial.println(led_1);
 
  delay(10000);
}
