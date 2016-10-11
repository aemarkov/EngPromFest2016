#include <Arduino.h>
#include "coordinate\coordinate.h"
//

//char json[] = "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
//SaticJsonBuffer<200> jsonBuffer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  Serial.println("Hello!");

  Coordinate coord;
  GetCoord(&coord);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println("test");
  //digitalWrite(13,HIGH);
  //delay(100);
  //digitalWrite(13,LOW);
  //delay(100);
}
