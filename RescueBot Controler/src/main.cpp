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

// Variables
byte ledValue;

// Functions
void HandleWebsite();
void HandleData();



void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);


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

  //pins for wheels
  int leftForward = 14;
  int leftBackward = 12;
  int rightForward = 13;
  int rightBackward = 15;

  bool forward = server.arg("top"); //forward
  bool backward = server.arg("bottom"); //backward
  bool left = server.arg("left"); //go to left
  bool right = server.arg("right"); //go to right

  if (left) {
    digitalWrite(HIGH, leftForward);
    digitalWrite(LOW, leftBackward);
    digitalWrite(LOW, rightForward);
    digitalWrite(HIGH, rightBackward);
  } else if (right) {
    digitalWrite(LOW, leftForward);
    digitalWrite(HIGH, leftBackward);
    digitalWrite(HIGH, rightForward);
    digitalWrite(LOW, rightBackward);
  } else if (backward) {
    digitalWrite(LOW, leftForward);
    digitalWrite(HIGH, leftBackward);
    digitalWrite(LOW, rightForward);
    digitalWrite(HIGH, rightBackward);
  } else if(forward) { 
    digitalWrite(HIGH, leftForward);
    digitalWrite(LOW, leftBackward);
    digitalWrite(HIGH, rightForward);
    digitalWrite(LOW, rightBackward);
  } else {
    //default
    digitalWrite(LOW, leftForward);
    digitalWrite(LOW, leftBackward);
    digitalWrite(LOW, rightForward);
    digitalWrite(LOW, rightBackward);
  }
  

  //TO Do use controll info
  if (valW > 0 && valW <= 10){
    ledValue = valW;
    analogWrite(ledPin, map(valW, 1, 10, 0, 1023));
  }

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

  String json = "{\"top\": " + forward + ",\"bottom\":" + backward + ",\"left\":" + left + ",\"right\":" + right +"}";
  server.send(200, "text/json", json);
}
