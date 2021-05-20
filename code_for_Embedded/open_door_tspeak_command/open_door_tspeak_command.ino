

#include <ESP8266WiFi.h>


const char *ssid =  "salem_net";     
const char *pass =  "ismash1234";


const char * apiKey = "A3VVWF6817BPOQNC";   
unsigned long channelNumber = 1336910; 
const int FieldNumber1=1;
#include <ThingSpeak.h>;
WiFiClient client;


const int moPin = 4; 

void vibrate_motor(int duration){
      digitalWrite(moPin,HIGH);
      delay(duration);
      digitalWrite(moPin, LOW);
      delay(duration); 
  }

void stop_motor(){
  digitalWrite(moPin, LOW);
  }

void openDoor(int times){
  while (times>0){
    vibrate_motor(500);
    times-=1;
  }
}


  

void setup() {
  Serial.begin(115200); 
  Serial.println("Ultrasonic Sensor HC-SR04 Test");
  Serial.println("with Arduino UNO R3");
  
  pinMode(moPin,OUTPUT);
  stop_motor();
  
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







void loop() {

  
  long doorCMD = ThingSpeak.readLongField(channelNumber, FieldNumber1, apiKey);
  long statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200)
  {
    Serial.print("Door Command: ");
    Serial.println(doorCMD);
    if (doorCMD==1){
      openDoor(5);
    }
    else{
      stop_motor();
    }
    
  }
  else
  {
    Serial.println("Unable to read channel / No internet connection");
  }
  

  delay(16000);
}
