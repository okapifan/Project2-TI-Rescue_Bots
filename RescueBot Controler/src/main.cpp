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
int trigPin2 = D6; //voorLinks
int trigPin3 = D7; //voorRechts
int trigPin4 = D8; //rechts
int echoPin = D4; // Todo set pin

// Pins for ir sensor
int ProxSensor = D9; // Todo set pin
int ProxSensor2 = D10;

// Pins for reed sensors
int ReedSensor = A0;


// Variables
byte ledValue;
bool autoDrive = false;
bool forward = false;
bool backward = false;
bool left = false;
bool right = false;

//sensors
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
void fullAuto();
long getDistance(int trigPin);
int readIr(int s);
int readReed();
void driveforward();
void drivebackward();
void driveleft();
void driveright();
void idle();
void rotate(String leftOrRight);
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

  //Reed-Sensor
  pinMode(ReedSensor,INPUT);

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
  server.on("/debug", HandleDebug);
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

void fullAuto(){


  usLinks = checkObject(getDistance(trigPin));
  usVoorLinks = checkObject(getDistance(trigPin2));
  usVoorRechts = checkObject(getDistance(trigPin3));
  usRechts = checkObject(getDistance(trigPin4));
  irRechts = checkObject(readIr(ProxSensor));
  irLinks = checkObject(readIr(ProxSensor2));
  reedVoor = digitalRead(ReedSensor);

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
  Serial.println("Backward");
}

void driveleft() {
  digitalWrite(leftForward, HIGH);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightBackward, HIGH);
  Serial.println("Left");
}

void driveright() {
  digitalWrite(leftForward, LOW);
  digitalWrite(leftBackward, HIGH);
  digitalWrite(rightForward, HIGH);
  digitalWrite(rightBackward, LOW);
  Serial.println("Right");
}

void idle() {
  digitalWrite(leftForward, LOW);
  digitalWrite(leftBackward, LOW);
  digitalWrite(rightForward, LOW);
  digitalWrite(rightBackward, LOW);
  Serial.println("Idle");
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

bool checkObject(int distance) {
  if((distance > 0 && distance < 20)/* || distance > 1050*/) {return true;}
  else {return false;}
}