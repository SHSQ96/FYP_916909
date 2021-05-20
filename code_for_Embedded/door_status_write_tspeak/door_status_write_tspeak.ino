
#include <ESP8266WiFi.h>


const char *ssid =  "salem_net";
const char *pass =  "ismash1234";
// end of home network

const char * apiKey = "EUBLAYYE4IRCKO2L";     
unsigned long channelNumber = 1314542; 
#include <ThingSpeak.h>;
WiFiClient client;




#define echoPin 4 
#define trigPin 0 


long duration;
int distance; 



void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  Serial.begin(115200); 
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); 
  Serial.println("with Arduino UNO R3");
  
  delay(10);
   
       
 Serial.println("Connecting to ");
 Serial.println(ssid);


 WiFi.begin(ssid, pass);

while (WiFi.status() != WL_CONNECTED) 
{
      delay(500);
      Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");

  ThingSpeak.begin(client);
    Serial.println("ThingSpeak begins");
  
}

int getdist(){
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
  
}




void send_val(float val, int field_num){
  ThingSpeak.writeField(channelNumber, field_num,val, apiKey); 
}  


void loop() {
  distance=getdist();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if (distance<20 and distance>0){
    send_val (1,1);
  }
  else{
    send_val (0,1);
  }
  delay(16000);
}
