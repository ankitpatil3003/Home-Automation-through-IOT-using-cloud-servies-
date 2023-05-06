#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
 
#define FIREBASE_HOST "cloud-toggle-switch-default-rtdb.firebaseio.com" // Firebase host
#define FIREBASE_AUTH "27AQqqZRfYKXNmDK1OcL2hOmcxQWUKl8k4DPauDp" //Firebase Auth code
#define WIFI_SSID "Ankit" //Enter your wifi Name
#define WIFI_PASSWORD "12344321" // Enter your password
#include <RBDdimmer.h>
String fireStatus;
//#define Relay1 14  //D5
const int zeroCrossPin  = D1;
const int acdPin  = D2;
int MIN_POWER  = 0;
int MAX_POWER  = 80;
int POWER_STEP  = 2;
int power  = 0;
//Objects
dimmerLamp acd(acdPin,zeroCrossPin);
 
void setup() {
  Serial.begin(9600);
  pinMode(D1, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Connected.");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Firebase.set("Led1Status/status", "on");
  //pinMode(Relay1,OUTPUT);
  //digitalWrite(Relay1,HIGH);
  acd.begin(NORMAL_MODE, OFF);
}
 
void loop() {
  if (Firebase.failed()){
    //Serial.println("not connected");
    Serial.println(Firebase.error());
  }
  else
  {
    //Serial.println("firebase connected");
  }
  fireStatus = Firebase.getString("Led1Status/status");
  if (fireStatus == "on") {
    Serial.println("Led Turned ON");
    //digitalWrite(D1, HIGH);
    delay(500);
    acd.setState(ON);
    //digitalWrite(Relay1,LOW);
    int sensorValue = analogRead(A0);
    String sensor = String(sensorValue);
    Firebase.setString("Led1Status/les", sensor);
    Serial.println(sensorValue);
    
    if (sensorValue >=50)
    {
      Serial.println("light off");
      acd.setState(OFF);
    }
    else if (sensorValue >50 && sensorValue <=51)
    {
      power = 50;
      acd.setPower(power);
      Serial.println("loght on at 50%");
    }else if (sensorValue <=45)
   {
      acd.setState(ON);
      Serial.println("light on");
    }
    
    
    
    //correctValue(); 
   /*int outVal = map(sensorValue, 22, 1023, 100, 0);
   Serial.println("dimerr value = ");
   Serial.print(outVal);
   acd.setPower(sensorValue);
    /*float voltage = sensorValue * (5.0 / 1023.0);
    Serial.println("voltage");
    Serial.print(voltage);
    if(voltage ==0.24)

    
    {
      power == 80;
      acd.setPower(power);
    }
    else if(voltage == 5)
    {
      digitalWrite(D1, LOW);
    }
    
    else if (voltage == 2)
    {
      power == 50;
      acd.setPower(power);
    }
    */
    
    delay(500);
    //testDimmer();
  }
  else if (fireStatus == "off") {
    Serial.println("Led Turned OFF");
    
    acd.setState(OFF);
    delay(10);
   
  }
  else {
    Serial.println("Command Error! Please send 0/1");
  }
} 


void testDimmer(){/* function testDimmer */ 
////Sweep light power to test dimmer
  for(power=MIN_POWER;power<=MAX_POWER;power+=POWER_STEP){
    acd.setPower(power); // setPower(0-100%);
      Serial.print("lampValue -> ");
      Serial.print(acd.getPower());
      Serial.println("%");
    delay(100);
  }

  for(power=MAX_POWER;power>=MIN_POWER;power-=POWER_STEP){
    acd.setPower(power); // setPower(0-100%);
      Serial.print("lampValue -> ");
      Serial.print(acd.getPower());
      Serial.println("%");
      Firebase.set("Led1Status/dimmer", acd.getPower());
    delay(100);
  }
}