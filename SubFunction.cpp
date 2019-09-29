#include <Arduino.h>
#include "SubFunction.h"
//#include<stdio.h> WRONG
//#include <iostream> WRONG
using namespace std;
//-------------------------DEFINE---------------------------
#define TRUE	1
#define FALSE	0
#define BUTTON_PIN1      4
#define BUTTON_PIN2     14
#define HALF_A_SECOND   50
#define ONE_TENTH_SECOND  10
#define ONE_SECOND      100
#define THREE_SECOND    300

////---------------- READ BUTTON PARAMETERS------------ //
typedef unsigned char    BOOL;
int		TimeButtonPress	= 0;	// Count time pressing the button
int		SubTimeCounter	= 0;	// Count time pressing in faster increasing
int		Value			= 0;	// Count the value of process
int		SignalRead_1		= 0;	// Present Button signal
int   SignalRead_2    = 0;  // Present Button signal
int		OldSignalRead_1	= 0;	// Before Button signal
int   OldSignalRead_2 = 0;  // Before Button signal
BOOL	ButtonPressed1s = 0;	// Press button enough 1s and less than 3s
BOOL	ButtonPressed3s = 0;	// Press button enough 3s
BOOL	HalfASecond		= 0;	// Press button 0.5s extra
BOOL	OneTenthSecond	= 0;	// Press button 0.1s extra
BOOL	fbutton1 = 0;			// Press Button or Not
BOOL  fbutton2 = 0;     // Press Button or Not
enum eState {
	BUTTON1_STEP_INCREASE,
  BUTTON1_FLOATING,
  BUTTON1_LOW_INCREASE,
  BUTTON1_FAST_INCREASE,
  BUTTON2_STEP_DECREASE,
  BUTTON2_FLOATING,
  BUTTON2_LOW_DECREASE,
  BUTTON2_FAST_DECREASE
}; //All State of FSM
enum eState NowState = BUTTON1_STEP_INCREASE;
//--------------------------------------------------------------


////----------------------INITIAL-----------------------
void SystemInitialize() {
	pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}
//----------------------------------------------------

////-------------------READ_BUTTON-------------------------------------
void ReadButton_func(void) {
  OldSignalRead_1 = SignalRead_1;
  OldSignalRead_2 = SignalRead_2;
  SignalRead_1 = digitalRead(BUTTON_PIN1);
  SignalRead_2 = digitalRead(BUTTON_PIN2);
  if(SignalRead_1){
      digitalWrite(LED_BUILTIN, LOW);
      Serial.print("     ");
      Serial.println(1);
      fbutton1 = 1;
      fbutton2 = 0;
      if(OldSignalRead_1 == SignalRead_1){ TimeButtonPress++; }
      else {
        SubTimeCounter = 0;
        ButtonPressed1s = 0;
        ButtonPressed3s = 0;
        TimeButtonPress = 0;
        SubTimeCounter = 0;
        OneTenthSecond = 0;
      }
    }
  else if(SignalRead_2){
      digitalWrite(LED_BUILTIN, LOW);
      Serial.print("     ");
      Serial.println(2, DEC);
      fbutton2 = 1;
      fbutton1 = 0;
      if(OldSignalRead_2 == SignalRead_2){ TimeButtonPress++; }
      else {
        SubTimeCounter = 0;
        ButtonPressed1s = 0;
        ButtonPressed3s = 0;
        TimeButtonPress = 0;
        SubTimeCounter = 0;
        OneTenthSecond = 0;
    }
    }
  else{
    digitalWrite(LED_BUILTIN, HIGH);
    fbutton1 = 0;
    fbutton2 = 0;
    }
}
//-----------------PRESSED_LASTING_TIME------------------------------
void PressedLastingTime(){
  if(TimeButtonPress == THREE_SECOND){
        ButtonPressed3s = 1;
        ButtonPressed1s = 0;
        SubTimeCounter = 0;
      }
  else if(TimeButtonPress > THREE_SECOND){
      if((SubTimeCounter % ONE_TENTH_SECOND == 0)&&(SubTimeCounter > 0)){
         OneTenthSecond = 1;
         HalfASecond = 0;
         SubTimeCounter = 1;
        }
      else {
         OneTenthSecond = 0;
         SubTimeCounter++;
        }
      }
  else if(TimeButtonPress >= ONE_SECOND){
        //ButtonPressed3s = 0;
        ButtonPressed1s = 1;
        SubTimeCounter++;
        if((SubTimeCounter % HALF_A_SECOND == 0)&&(SubTimeCounter > 0)){HalfASecond = 1;}
        else HalfASecond = 0;
        }
}
//---------------------DISPLAY_VALUE-------------------------
void ValueDisplay(){
  switch (NowState) {
  case BUTTON1_STEP_INCREASE:
                       if (fbutton1) {
                              Value++;
                              NowState = BUTTON1_FLOATING;
                              Serial.println(Value, DEC);
                       }
                       else if(fbutton2) {
                              NowState = BUTTON2_STEP_DECREASE;
                       }
                       break;
  case BUTTON1_FLOATING:
                       if (fbutton1 == 0){
                              if(fbutton2)NowState = BUTTON2_STEP_DECREASE;
                              else NowState = BUTTON1_STEP_INCREASE;
                              }
                       else if (ButtonPressed1s) NowState = BUTTON1_LOW_INCREASE;
                       break;
  case BUTTON1_LOW_INCREASE:
                       if (HalfASecond) {
                              Value++;
                              Serial.println(Value, DEC);
                              }
                        if (fbutton1 == 0){
                              if(fbutton2)NowState = BUTTON2_STEP_DECREASE;
                              else NowState = BUTTON1_STEP_INCREASE;
                              }
                        else if (ButtonPressed3s) NowState = BUTTON1_FAST_INCREASE;
                        break;
  case BUTTON1_FAST_INCREASE:
                        if (fbutton1 == 0){
                              if(fbutton2)NowState = BUTTON2_STEP_DECREASE;
                              else NowState = BUTTON1_STEP_INCREASE;
                              }
                        else if (OneTenthSecond) {
                              Value++;
                              Serial.println(Value, DEC);
                              }
                        break;
   case BUTTON2_STEP_DECREASE:
                        if (fbutton2)  {
                                  Value--;
                                  NowState = BUTTON2_FLOATING;
                                  Serial.println(Value, DEC);
                                  }
                        else NowState = BUTTON1_STEP_INCREASE;
                        break;
  case BUTTON2_FLOATING:
                        if (fbutton2 == 0) NowState = BUTTON1_STEP_INCREASE;
                        else if (ButtonPressed1s) NowState = BUTTON2_LOW_DECREASE;
                        break;
        
  case BUTTON2_LOW_DECREASE:
                        if (HalfASecond) {
                                 Value--;
                                 Serial.println(Value, DEC);
                                 }
                        if (fbutton2 == 0) NowState = BUTTON1_STEP_INCREASE;
                        else if (ButtonPressed3s) NowState = BUTTON2_FAST_DECREASE;
                        break;
  case BUTTON2_FAST_DECREASE:
                        if (fbutton2 == 0) NowState = BUTTON1_STEP_INCREASE;
                        else if (OneTenthSecond) {
                                 Value--;
                                 Serial.println(Value, DEC);
                                 }
                        break;
  }
}
//--------------------------------------------------------------------
