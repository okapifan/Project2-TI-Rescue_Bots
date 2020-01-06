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
const char *ssid = "esp8266";
const char *password = "123123123";
ESP8266WebServer server(80);

// Webpages html
#include "Page_Controler_Test.h"

// IO
const byte ledPin = LED_BUILTIN; // LED_BUILTIN
//pins for wheels
int leftForward = 0;
int leftBackward = 1;
int rightForward = 2;
int rightBackward = 3;

// Variables
byte ledValue;

// Functions
void HandleWebsite();
void HandleData();



void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(leftForward, OUTPUT);
  pinMode(leftBackward, OUTPUT);
  pinMode(rightForward, OUTPUT);
  pinMode(rightBackward, OUTPUT);


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
}

void HandleWebsite()
{
  // /

  String html = String(page_Controler_Test);
  server.send(200, "text/html", html);
}

void HandleData()
{
  // /data?id=___&w=___
  //byte valId = server.arg("id").toInt();



  bool forward = server.arg("top"); //forward
  bool backward = server.arg("bottom"); //backward
  bool left = server.arg("left"); //go to left
  bool right = server.arg("right"); //go to right

  forward = true;

   if (left) {
    digitalWrite(leftForward, HIGH);
    digitalWrite(leftBackward, LOW);
    digitalWrite(rightForward, LOW);
    digitalWrite(rightBackward, HIGH);
    Serial.println("left");
  } else if (right) {
    digitalWrite(leftForward, LOW);
    digitalWrite(leftBackward, HIGH);
    digitalWrite(rightForward, HIGH);
    digitalWrite(rightBackward, LOW);
    Serial.println("right");
  } else if (backward) {
    digitalWrite(leftForward, LOW);
    digitalWrite(leftBackward, HIGH);
    digitalWrite(rightForward, LOW);
    digitalWrite(rightBackward, HIGH);
    Serial.println("backward");
  } else if(forward) { 
    digitalWrite(leftForward, HIGH);
    digitalWrite(leftBackward, LOW);
    digitalWrite(rightForward, HIGH);
    digitalWrite(rightBackward, LOW);
    Serial.println("forward");
  } else {
    //default
    digitalWrite(leftForward, LOW);
    digitalWrite(leftBackward, LOW);
    digitalWrite(rightForward, LOW);
    digitalWrite(rightBackward, LOW);
    Serial.println("default");
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

  String json = "{\"top\": " + (String)forward + ",\"bottom\": " + (String)backward + ",\"left\": " + (String)left + ",\"right\": " + (String)right +"}";
  server.send(200, "text/json", json);
}
