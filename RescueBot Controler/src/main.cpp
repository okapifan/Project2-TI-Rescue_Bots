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
const int leftForwardPin = D0;
const int leftBackwardPin = D1;
const int rightForwardPin = D2;
const int rightBackwardPin = D5;

// Pins for ultrasoon sensor
const int trigPin1 = D3; // Links
const int trigPin2 = D6; // VoorLinks
const int trigPin3 = D7; // VoorRechts
const int trigPin4 = D8; // Rechts
const int echoPin = D4;

// Pins for ir sensor
const int IRPin1 = D9;
const int IRPin2 = D10;

// Pins for reed sensors
const int ReedPin = A0;


// Variables
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
void driveIdle();
void driveForward();
void driveBackward();
void driveLeft();
void driveRight();
void rotate(String leftOrRight);


void setup()
{
  //Serial.begin(115200);

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

  //getAllSensors(); // Debug,  Get all sensor value without self driving enables

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
    if (forward) {driveForward();}
    else if (backward) {driveBackward();}
    else if (left) {driveLeft();}
    else if (right) {driveRight();}
    else {driveIdle();}
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
  //usVoorLinks = checkObject(getDistance(trigPin2));
  usVoorRechts = checkObject(getDistance(trigPin3));
  usRechts = checkObject(getDistance(trigPin4));
  irRechts = getIR(IRPin1);
  irLinks = getIR(IRPin2);
  reedVoor = getReed(ReedPin);
}
void selfDrive(){

  // //check of er slachtoffer ligt, zo ja wacht
  // if(reedVoor){delay(1500);}

  // //check of naar voren rijden kan, zo ja rijd naar voren
  // else if (!(usVoorLinks || usVoorRechts)) {driveForward();}

  // //check of je naar links kan, zo ja rijd naar links
  // else if (!(usLinks || irLinks)) {driveLeft();}

  // //check of je naar rechts kan, zo ja rijd naar rechts
  // else if (!(usRechts || irRechts)) {driveRight();}

  // //niks mogelijk rijd naar achteren
  // else {driveBackward();}


  
  if(reedVoor) { // Check of er slachtoffer ligt, zo ja wacht
    driveIdle();
    delay(1500);
  }
  else if (usVoorLinks || usVoorRechts) { // Check of er een obstakel voor ligt
    driveBackward();
    delay(1000);
    driveLeft();//driveRight();
    delay(400);
  }

  else if (irRechts) { // Check of er een obstakel rechts ligt
    driveBackward();
    delay(400);
    driveLeft();
    delay(600);
  }
  else if (irLinks) { // Check of er een obstakel links ligt
    driveBackward();
    delay(400);
    driveRight();
    delay(600);
  }
  else if (usLinks) { // Check of er een obstakel links ligt
    driveRight();
  }
  else if (usRechts) { // Check of er een obstakel rechts ligt
    driveLeft();
  }
  else { // Niets aan de hand
    driveForward();
  }
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
  if((distance > 0 && distance < 10)/* || distance > 1050*/) {
    return true;
  }
  else {
    return false;
  }
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


void driveIdle() {
  digitalWrite(leftForwardPin, LOW);
  digitalWrite(leftBackwardPin, LOW);
  digitalWrite(rightForwardPin, LOW);
  digitalWrite(rightBackwardPin, LOW);
}
void driveForward() {
  digitalWrite(leftForwardPin, HIGH);
  digitalWrite(leftBackwardPin, LOW);
  digitalWrite(rightForwardPin, HIGH);
  digitalWrite(rightBackwardPin, LOW);
}
void driveBackward() {
  digitalWrite(leftForwardPin, LOW);
  digitalWrite(leftBackwardPin, HIGH);
  digitalWrite(rightForwardPin, LOW);
  digitalWrite(rightBackwardPin, HIGH);
}
void driveRight() {
  digitalWrite(leftForwardPin, LOW);
  digitalWrite(leftBackwardPin, HIGH);
  digitalWrite(rightForwardPin, HIGH);
  digitalWrite(rightBackwardPin, LOW);
}
void driveLeft() {
  digitalWrite(leftForwardPin, HIGH);
  digitalWrite(leftBackwardPin, LOW);
  digitalWrite(rightForwardPin, LOW);
  digitalWrite(rightBackwardPin, HIGH);
}
