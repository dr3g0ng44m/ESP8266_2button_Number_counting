//#include <ESP8266WiFi.h>
//#include <Ticker.h>
//#include "SubFunction.h"
//#define BUTTON_PIN 0
//#define BUTTON_PIN1      5
//#define BUTTON_PIN2      14
//int   SignalRead_1    = 0;  // Present Button signal
//int   SignalRead_2    = 0;  // Present Button signal
//Ticker buttonPress;
//Ticker LEDDisplay;
//Ticker yield_;
//void SystemInitialize() {
//  pinMode(BUTTON_PIN1, INPUT_PULLUP);
//  pinMode(BUTTON_PIN2, INPUT_PULLUP);
//  pinMode(LED_BUILTIN, OUTPUT);
//  digitalWrite(LED_BUILTIN, HIGH);
//}
//void ReadButton_func(void) {
//  //OldSignalRead_1 = SignalRead_1;
//  //OldSignalRead_2 = SignalRead_2;
//  SignalRead_1 = digitalRead(BUTTON_PIN1);
//  SignalRead_2 = digitalRead(BUTTON_PIN2);
//  if(SignalRead_1){digitalWrite(LED_BUILTIN, HIGH);}
//  else if(SignalRead_2){
//      digitalWrite(LED_BUILTIN, HIGH);
//  }
//  else digitalWrite(LED_BUILTIN, LOW);
//}
void setup() { 
  //SystemInitialize(); 
  Serial.begin(115200);
  //delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  //buttonPress.attach_ms(10,ReadButton_func);
  
  //LEDDisplay.attach_ms(10,LED_Display);
  //attachInterrupt(BUTTON_PIN, ReadButton_func, LOW);
  //attachInterrupt(BUTTON_PIN, LED_Display, LOW);
}
void loop(){
  delay(1000);
  yield();
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
