#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "SubFunction.h"
#define BUTTON_PIN1      5
#define BUTTON_PIN2      14

Ticker buttonPress;
Ticker timeLasting;
Ticker valueDisplay;

void setup() { 
  SystemInitialize(); 
  Serial.begin(115200);
  buttonPress.attach_ms(10,ReadButton_func);
  timeLasting.attach_ms(10,PressedLastingTime);
  valueDisplay.attach_ms(10,ValueDisplay);
  //attachInterrupt(BUTTON_PIN, ReadButton_func, LOW);
  //attachInterrupt(BUTTON_PIN, LED_Display, LOW);
}
void loop(){}
