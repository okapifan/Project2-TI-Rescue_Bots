/*
	Vak: Project 2 (TINPRO02-2)
	Klas: TI1D
  Groep: 4
	  Annelot Jansen
    Daniël van der Drift
    Daylan de Lange
    Zoë Zegers

	RescueBot 2
*/

// Libraries
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//#include <WiFiUdp.h>

// Wifi settings
const char *ssid = "TI1D_4_RB2";
const char *password = "123123123";
ESP8266WebServer server(80);

// Webpages html
#include "Page_Controler_Test.h"

// Pins for wheels
int leftForwardPin = D1;
int leftBackwardPin = D0;
int rightForwardPin = D5;
int rightBackwardPin = D2;

// Variables
byte ledValue;
bool autoDrive = false;
bool forward = false;
bool backward = false;
bool left = false;
bool right = false;



// Functions
void getAllSensors();
void selfDrive();
void idle();
void driveforward();
void drivebackward();
void driveleft();
void driveright();


void setup()
{
  Serial.begin(115200);
  
  //Movement
  pinMode(leftForwardPin, OUTPUT);
  pinMode(leftBackwardPin, OUTPUT);
  pinMode(rightForwardPin, OUTPUT);
  pinMode(rightBackwardPin, OUTPUT);

  Serial.print("Setting AP (Access Point)");
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

  
    if (forward) {driveforward();}
    else if (backward) {drivebackward();}
    else if (left) {driveleft();}
    else if (right) {driveright();}
    else {idle();}

  String json = "{\"top\": " + (String)forward + ",\"bottom\": " + (String)backward + ",\"left\": " + (String)left + ",\"right\": " + (String)right + "}";
  server.send(200, "text/json", json);
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