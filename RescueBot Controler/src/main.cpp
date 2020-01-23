/*
	Vak: Project 2 (TINPRO02-2)
	Klas: TI1D
  Groep: 4
	  Annelot Jansen
    Daniël van der Drift
    Daylan de Lange
    Zoë Zegers

	RescueBot 1
*/

// Libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wifi settings
const char *ssid = "TI1D_4_RB1";
const char *password = "123123123";
ESP8266WebServer server(80);

// Webpages html
#include "Page_Controller.h"

// Pins for wheels
int leftForwardPin = D0;
int leftBackwardPin = D1;
int rightForwardPin = D2;
int rightBackwardPin = D5;

// Pins for ultrasoon sensor
int trigPin1 = D3; // Links
int trigPin2 = D6; // VoorLinks
int trigPin3 = D7; // VoorRechts
int trigPin4 = D8; // Rechts
int echoPin = D4;

// Pins for ir sensor
int IRPin1 = D9;
int IRPin2 = D10;

// Pins for reed sensors
int ReedPin = A0;


// Variables
byte ledValue;
bool autoDrive = false;
bool forward = false;
bool backward = false;
bool left = false;
bool right = false;

// Sensors
bool usLinks;
bool usVoorLinks;
bool usVoorRechts;
bool usRechts;
bool irLinks;
bool irRechts;
bool reedVoor;

// Functions
void HandleWebsite();
void HandleData();
void HandleDebug();

long getDistance(int trigPin);
bool checkObject(int distance);
bool getIR(int s);
bool getReed(int s);

void getAllSensors();
void selfDrive();
void idle();
void driveforward();
void drivebackward();
void driveleft();
void driveright();
void rotate(String leftOrRight);


void setup()
{
  Serial.begin(115200);

  //Movement
  pinMode(leftForwardPin, OUTPUT);
  pinMode(leftBackwardPin, OUTPUT);
  pinMode(rightForwardPin, OUTPUT);
  pinMode(rightBackwardPin, OUTPUT);

  //Ultrasoon Sensor
  pinMode (echoPin, INPUT);
  pinMode (trigPin1, OUTPUT);
  pinMode (trigPin2, OUTPUT);
  pinMode (trigPin3, OUTPUT);
  pinMode (trigPin4, OUTPUT);

  //IR-Sensor
  pinMode(IRPin1,INPUT);
  pinMode(IRPin2,INPUT);

  //Reed-Sensor
  pinMode(ReedPin,INPUT);

  Serial.print("Setting AP (Access Point)");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress ip = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(ip);

  server.on("/", HandleWebsite);
  server.on("/data", HandleData);
  server.on("/debug", HandleDebug);
  server.begin();

  Serial.println("Server started");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  server.handleClient();

  // getAllSensors(); // Debug,  Get all sensor value without self driving enables

  if (autoDrive){
    getAllSensors();
    // Self drive code
    selfDrive();
  }
}

void HandleWebsite()
{
  //

  String html = String(page_Controller);
  server.send(200, "text/html", html);
}
void HandleData()
{
  // /data?forward=___&backward=___&left=___&right=___
  // /data?autodrive=___

  // Parse button input
  if (server.arg("forward").toInt() > 0) {
    forward = server.arg("forward").toInt() - 1; //forward
    backward = server.arg("backward").toInt() - 1; //backward
    left = server.arg("left").toInt() - 1; //go to left
    right = server.arg("right").toInt() - 1; //go to right
  }

  // Parse autodrive toggle
  if (server.arg("autodrive").toInt() > 0) {
    autoDrive = server.arg("autodrive").toInt() - 1;
  }

  // Remote controlled driving
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

  String json = "{\"top\": " + (String)forward + ",\"bottom\": " + (String)backward + ",\"left\": " + (String)left + ",\"right\": " + (String)right + ",\"autoDrive\": " + (String)autoDrive + "}";
  server.send(200, "text/json", json);
}
void HandleDebug()
{
  // /debug
  
  String text = "U Links: " + (String)usLinks + 
  "\nU VoorLinks: " + (String)usVoorLinks + 
  "\nU VoorRechts: " + (String)usVoorRechts + 
  "\nURechts: " + (String)usRechts + 
  "\nIR Links: " + (String)irLinks + 
  "\nIR Rechts: " + (String)irRechts +
  "\nReed: " + (String)reedVoor + "\n";
  server.send(200, "text", text);
}


void getAllSensors(){
  usLinks = checkObject(getDistance(trigPin1));
  usVoorLinks = checkObject(getDistance(trigPin2));
  usVoorRechts = checkObject(getDistance(trigPin3));
  usRechts = checkObject(getDistance(trigPin4));
  irRechts = getIR(IRPin1);
  irLinks = getIR(IRPin2);
  reedVoor = getReed(ReedPin);
}
void selfDrive(){

  //check of er slachtoffer ligt , zo ja wacht
  //check of naar voren rijden kan, zo ja rijd naar voren
  //check of je naar links kan, zo ja rijd naar links
  //check of je naar rechts kan, zo ja rijd naar rechts
  //niks mogelijk rijd naar achteren

  if(reedVoor){delay(1500);}
  else if (!(usVoorLinks || usVoorRechts)) {driveforward();} 
  else if (!(usLinks || irLinks)) {driveleft();}
  else if (!(usRechts || irRechts)) {driveright();}
  else {drivebackward();}
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
  Serial.println(trigPin + ": " + distance);
  return distance;
}
bool checkObject(int distance) {
  if((distance > 0 && distance < 20)/* || distance > 1050*/) {return true;}
  else {return false;}
}
bool getIR(int s){
  bool inputValue = digitalRead(s);
  return inputValue;
}
bool getReed(int s){
  int inputValue1 = analogRead(s);
  bool inputValue = (inputValue1 < 512);
  return inputValue;
}


void idle() {
  digitalWrite(leftForwardPin, LOW);
  digitalWrite(leftBackwardPin, LOW);
  digitalWrite(rightForwardPin, LOW);
  digitalWrite(rightBackwardPin, LOW);
  Serial.println("Idle");
}
void driveforward() {
  digitalWrite(leftForwardPin, HIGH);
  digitalWrite(leftBackwardPin, LOW);
  digitalWrite(rightForwardPin, HIGH);
  digitalWrite(rightBackwardPin, LOW);
  Serial.println("Forward");
}
void drivebackward() {
  digitalWrite(leftForwardPin, LOW);
  digitalWrite(leftBackwardPin, HIGH);
  digitalWrite(rightForwardPin, LOW);
  digitalWrite(rightBackwardPin, HIGH);
  Serial.println("Backward");
}
void driveleft() {
  digitalWrite(leftForwardPin, HIGH);
  digitalWrite(leftBackwardPin, LOW);
  digitalWrite(rightForwardPin, LOW);
  digitalWrite(rightBackwardPin, HIGH);
  Serial.println("Left");
}
void driveright() {
  digitalWrite(leftForwardPin, LOW);
  digitalWrite(leftBackwardPin, HIGH);
  digitalWrite(rightForwardPin, HIGH);
  digitalWrite(rightBackwardPin, LOW);
  Serial.println("Right");
}

// Rotates 45 degrees
// void rotate(String leftOrRight){
//   drivebackward();
//   delay(500); //Todo
//   if(leftOrRight = "left"){
//     driveleft();
//   } else if (leftOrRight = "right"){
//     driveright();
//   }
//   delay(500);
// }
