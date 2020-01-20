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
int ProxSensor = D9; // Todo set pin
int ProxSensor2 = D10;

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
void fullAuto();
long getDistance(int trigPin);
int readIr(int s);
void driveforward();
void drivebackward();
void driveleft();
void driveright();
void idle();
bool checkObject(int distance);


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
    fullAuto();
  }
}

void HandleWebsite()
{
  //

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
    if (forward) {driveforward();}
    else if (backward) {drivebackward();}
    else if (left) {driveleft();}
    else if (right) {driveright();}
    else {idle();}
  }
  else{
    forward = backward = left = right = false;
  }

  String json = "{\"top\": " + (String)forward + ",\"bottom\": " + (String)backward + ",\"left\": " + (String)left + ",\"right\": " + (String)right + "}";
  server.send(200, "text/json", json);
}

void fullAuto(){
  bool usLinks;
  bool usVoorLinks;
  bool usVoorRechts;
  bool usRechts;
  bool irLinks;
  bool irRechts;

  usLinks = checkObject(getDistance(trigPin));
  usVoorLinks = checkObject(getDistance(trigPin2));
  usVoorRechts = checkObject(getDistance(trigPin3));
  usRechts = checkObject(getDistance(trigPin4));
  irLinks = checkObject(readIr(ProxSensor));
  irRechts = checkObject(readIr(ProxSensor2));

  
  if(usRechts) {driveleft();}
  else if (usVoorLinks || usVoorRechts) {drivebackward();} 
  else if (irLinks) {rotate("left");}
  else if (irRechts) {rotate("right");}
  else {driveforward();}
  


 }

long getDistance(int trigPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  long duration = pulseIn(echoPin, HIGH);

  //This gives us distance in cm (5-35 cm)
  long distance = duration/58.2;

  return distance;
}

int readIr(int s){ //Todo fix this stupid name
  int inputValue = digitalRead(s);
  return inputValue;
}

void driveforward() {
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightBackward, LOW);
  Serial.println("Forward");
}

void drivebackward() {
  digitalWrite(leftForward, LOW);
  digitalWrite(leftBackward, HIGH);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightBackward, HIGH);
  Serial.println("Forward");
}

void driveleft() {
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightBackward, HIGH);
  Serial.println("Forward");
}

void driveright() {
  digitalWrite(leftForward, LOW);
  digitalWrite(leftBackward, HIGH);
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightBackward, LOW);
  Serial.println("Forward");
}

void idle() {
  digitalWrite(leftForward, LOW);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightBackward, LOW);
  Serial.println("Forward");
}

// Rotates 45 degrees
void rotate(string leftOrRight){
  drivebackward();
  delay(500); //Todo
  if(leftOrRight = "left"){
    driveleft();
  } else if (leftOrRight = "right"){
    driveright();
  }
  delay(500); //Todo
}

bool checkObject(int distance) {
  if((distance > 0 && distance < 15) || distance > 1050) {return true;}
  else {return false;}
}