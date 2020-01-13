/*
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
int rightBackward = D5;

// Pins for ultrasoon sensor
int trigPin = D3; // Todo set pin
int trigPin2 = D6; // Todo set pin
int trigPin3 = D7;
int trigPin4 = D8;
int echoPin = D4; // Todo set pin

// Pins for ir sensor
int ProxSensor; // Todo set pin
int ProxSensor2;

// Variables
byte ledValue;
bool autoDrive = false;
bool forward = false;
bool backward = false;
bool left = false;
bool right = false;

// Functions
void HandleWebsite();
void HandleData();
void getSensorsUpdate();
long getDistance(int trigPin);
int getIRReaction(int s);


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
  pinMode (trigPin3, OUTPUT);
  pinMode (trigPin4, OUTPUT);

  //IR-Sensor
  pinMode(ProxSensor,INPUT);
  pinMode(ProxSensor2,INPUT);

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
    getSensorsUpdate();
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
  // /data?forward=___&backward=___&left=___&right=___
  // /data?autodrive=___

  if (server.arg("forward").toInt() > 0) {
    forward = server.arg("forward").toInt() - 1; //forward
    backward = server.arg("backward").toInt() - 1; //backward
    left = server.arg("left").toInt() - 1; //go to left
    right = server.arg("right").toInt() - 1; //go to right
  }

  if (server.arg("autodrive").toInt() > 0) {
    autoDrive = server.arg("autodrive").toInt() - 1;
  }

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
  else{
    forward = backward = left = right = false;
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
  long us1 = 0;
  long us2 = 0;
  long us3 = 0;
  long us4 = 0;
  int ir1 = 0;
  int ir2 = 0;

  //us1 = getDistance(trigPin);
  us2 = getDistance(trigPin2);
  us3 = getDistance(trigPin3);
  us4 = getDistance(trigPin4);
  //ir1 = getIRReaction(ProxSensor);
  //ir2 = getIRReaction(ProxSensor2);
  Serial.print(us1);
  Serial.print(", ");
  Serial.print(us2);
  Serial.print(", ");
  Serial.print(us3);
  Serial.print(", ");
  Serial.println(us4);
 }

long getDistance(int trigPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  long duration = pulseIn(echoPin, HIGH);

  //This gives us distance in cm
  long distance = duration/58.2;

  return distance;
}

int getIRReaction(int s){ //Todo fix this stupid name
  int inputValue = digitalRead(s);
  return inputValue;
}
