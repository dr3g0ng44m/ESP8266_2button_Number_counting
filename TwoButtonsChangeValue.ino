#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "SubFunction.h"

Ticker buttonPress;
Ticker timeLasting;
Ticker valueDisplay;

void setup() { 
  SystemInitialize(); 
  Serial.begin(115200);
  buttonPress.attach_ms(10,ReadButton_func);
  timeLasting.attach_ms(10,PressedLastingTime);
  valueDisplay.attach_ms(10,ValueDisplay);
}
void loop(){}
