/*
  Author: Daniël van der Drift

  Project:
  Ledstrip controler
*/

// Libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//#include <WiFiUdp.h>

// Wifi settings
const char *ssid = "TI1D_4_RB1";
const char *password = "123123123";
ESP8266WebServer server(80);

// Webpages html
#include "Page_Controler_Test.h"

// IO
const byte ledPin = LED_BUILTIN; // LED_BUILTIN

// Pins for wheels
int leftForward = D0;
int leftBackward = D1;
int rightForward = D2;
int rightBackward = D3;

// Pins for ultrasoon sensor
int trigPin; // Todo set pin
int trigPin2; // Todo set pin
int echoPin; // Todo set pin

// Pins for ir sensor
int ProxSensor; // Todo set pin

// Variables
byte ledValue;
bool autoDrive = false;

// Functions
void HandleWebsite();
void HandleData();
void getSensorsUpdate();
long getDistance(int trigPin);

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT); // Question: hebben we een LED nodig

  //Movement
  pinMode(leftForward, OUTPUT);
  pinMode(leftBackward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightBackward, OUTPUT);

  //Ultrasoon Sensor
  pinMode (echoPin, INPUT);
  pinMode (trigPin, OUTPUT);
  pinMode (trigPin2, OUTPUT);

  //IR-Sensor
  pinMode(13, OUTPUT); // Todo Change pin 13
  pinMode(ProxSensor,INPUT);

  /*Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
  Serial.println("WiFi connected");*/

  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(ip);

  server.on("/", HandleWebsite);
  server.on("/data", HandleData);
  server.begin();

  Serial.println("Server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  server.handleClient();

  if (autoDrive){
    // Self drive code
    getUltrasoonUpdate();
  }
}

void HandleWebsite()
{
  // /

  String html = String(page_Controler_Test);
  server.send(200, "text/html", html);
}

void HandleData()
{
  // /data?top=___&bottom=___&left=___&right=___
  // /data?autodrive=___

  bool forward = server.arg("forward").toInt();     //forward
  bool backward = server.arg("backward").toInt(); //backward
  bool left = server.arg("left").toInt();       //go to left
  bool right = server.arg("right").toInt();     //go to right

  autoDrive = server.arg("autodrive").toInt();

  if (!autoDrive)
  {

    if (forward)
    {
      digitalWrite(leftForward, HIGH);
      digitalWrite(leftBackward, LOW);
      digitalWrite(rightForward, HIGH);
      digitalWrite(rightBackward, LOW);
      Serial.println("Forward");
    }
    else if (backward)
    {
      digitalWrite(leftForward, LOW);
      digitalWrite(leftBackward, HIGH);
      digitalWrite(rightForward, LOW);
      digitalWrite(rightBackward, HIGH);
      Serial.println("Backward");
    }
    else if (left)
    {
      digitalWrite(leftForward, LOW);
      digitalWrite(leftBackward, HIGH);
      digitalWrite(rightForward, HIGH);
      digitalWrite(rightBackward, LOW);
      Serial.println("Left");
    }
    else if (right)
    {
      digitalWrite(leftForward, HIGH);
      digitalWrite(leftBackward, LOW);
      digitalWrite(rightForward, LOW);
      digitalWrite(rightBackward, HIGH);
      Serial.println("Right");
    }
    else
    {
      //default
      digitalWrite(leftForward, LOW);
      digitalWrite(leftBackward, LOW);
      digitalWrite(rightForward, LOW);
      digitalWrite(rightBackward, LOW);
      Serial.println("default");
    }
  }

  /*//TO Do use controll info
  if (valW > 0 && valW <= 10){
    ledValue = valW;
    analogWrite(ledPin, map(valW, 1, 10, 0, 1023));
  }*/

  /*if (valId > 0)
  {
    switch (valId)
    {
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    }
  }*/

  String json = "{\"top\": " + (String)forward + ",\"bottom\": " + (String)backward + ",\"left\": " + (String)left + ",\"right\": " + (String)right + "}";
  server.send(200, "text/json", json);
}

void getSensorsUpdate(){
  long distance;

  distance = getDistance(trigPin);
  delay(100);
  distance2 = getDistance(trigPin2);
  delay(100);
  getIRReaction();
  delay(100);
}

long getDistance(int trigPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  long duration = pulseIn (echoPin, HIGH);

  //This gives us distance in cm
  long distance = duration/58.2;
  Serial.print("Dit is de meting voor pin "+ trigPin +": ");
  Serial.println (distance);

  return distance;
}

int getIRReaction(){ //Todo fix this stupid name
  int inputValue = 0;

  // Check the sensor output
  if(digitalRead(ProxSensor) == HIGH){
    // Set the LED on
    digitalWrite(13, HIGH); // Todo Change pin 13
  }else{
    // Set the LED off
    digitalWrite(13, LOW); // Todo Change pin 13 
  }
  inputValue = digitalRead(ProxSensor);
  Serial.println(inputValue);

  return inputValue;
}
