#include <Arduino.h>
#include "SubFunction.h"

//-------------------------DEFINE---------------------------
#define TRUE	1
#define FALSE	0
#define BUTTON_PIN1       4   //GPIO4 (D2)is Button1 input
#define BUTTON_PIN2       14  //GPIO14(D5)is Button2 input
#define HALF_A_SECOND     50   // 50  * 10 ms
#define ONE_TENTH_SECOND  10   // 10  * 10 ms
#define ONE_SECOND        100  // 100 * 10 ms
#define THREE_SECOND      300  // 300 * 10 ms

////---------------- READ BUTTON PARAMETERS------------ //
typedef unsigned char    BOOL;
int		TimeButtonPress	= 0;	// Count time-pressing the button
int		SubTimeCounter	= 0;	// Count time-pressing in faster increasing
int		Value			= 0;	// Count the value of process
int		SignalRead_1		= 0;	// Present Button 1 signal
int   SignalRead_2    = 0;  // Present Button 2 signal
int		OldSignalRead_1	= 0;	// Previous Button 1 signal
int   OldSignalRead_2 = 0;  // Previous Button 2 signal
BOOL	ButtonPressed1s = 0;	// Have Pressed button from 1s to less than 3s
BOOL	ButtonPressed3s = 0;	// Have Pressed button enough 3s
BOOL	HalfASecond		= 0;	  // Have Pressed button enough 0.5s  
BOOL	OneTenthSecond	= 0;	// Have Pressed button enough 0.1s
BOOL	fbutton1 = 0;			    // Presssing Button 1 or Not
BOOL  fbutton2 = 0;         // Presssing Button 2 or Not

enum eState {
	BUTTON1_STEP_INCREASE,  // Increase Value step by step
  BUTTON1_FLOATING,       // Step-by_step or faster Increasing Value
  BUTTON1_LOW_INCREASE,   // Lower-FastIncreasingValue
  BUTTON1_FAST_INCREASE,  // Faster-FastIncreasingValue
  BUTTON2_STEP_DECREASE,  // Decrease Value step by step
  BUTTON2_FLOATING,       // Step-by_step or faster Decreasing Value
  BUTTON2_LOW_DECREASE,   // Lower Fast-Decreasing Value
  BUTTON2_FAST_DECREASE   // Faster Fast-IncreasingValue
}; //All State of FSM

enum eState NowState = BUTTON1_STEP_INCREASE; //initial State of program
//--------------------------------------------------------------


////----------------------INITIAL-----------------------
void SystemInitialize() {
	pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);   // Notice that some button is(are) pressed
  digitalWrite(LED_BUILTIN, LOW); // Notice that we can start program
}
//----------------------------------------------------

////-------------------READ_BUTTON-------------------------------------
void ReadButton_func(void) {
  OldSignalRead_1 = SignalRead_1;
  OldSignalRead_2 = SignalRead_2;
  SignalRead_1 = digitalRead(BUTTON_PIN1);
  SignalRead_2 = digitalRead(BUTTON_PIN2);
 // Priority of Button 1 is higher than Button 2's
 //If button 1 is pressed 
  if(SignalRead_1){
      digitalWrite(LED_BUILTIN, LOW); // Notice that some button is pressed
      Serial.println("\t1");          // Notice that button 1 is pressed
      fbutton1 = 1;
      fbutton2 = 0;
      if(OldSignalRead_1 == SignalRead_1){ TimeButtonPress++; } //Button 1 continued pressing
      else { //Button 1 Started pressing
        SubTimeCounter = 0;
        ButtonPressed1s = 0;
        ButtonPressed3s = 0;
        TimeButtonPress = 0;
        SubTimeCounter = 0;
        OneTenthSecond = 0;
      }
    }
  else if(SignalRead_2){
      digitalWrite(LED_BUILTIN, LOW); // Notice that some button is pressed
      Serial.println("\t2");          // Notice that button 2 is pressed
      fbutton2 = 1;
      fbutton1 = 0;
      if(OldSignalRead_2 == SignalRead_2){ TimeButtonPress++; } //Button 2 continued pressing
      else { //Button 2 Started pressing
        SubTimeCounter = 0;
        ButtonPressed1s = 0;
        ButtonPressed3s = 0;
        TimeButtonPress = 0;
        SubTimeCounter = 0;
        OneTenthSecond = 0;
    }
    }
  else{
    digitalWrite(LED_BUILTIN, HIGH); // No button is pressed
    fbutton1 = 0;
    fbutton2 = 0;
    }
}
//-----------------PRESSED_LASTING_TIME------------------------------
void PressedLastingTime(){
  //Sometimes Time of PressingButton isnt exactly 3s but it's always < 3.01s 
  if((TimeButtonPress >= THREE_SECOND)&&(TimeButtonPress <= THREE_SECOND + 1)){
        ButtonPressed3s = 1;
        ButtonPressed1s = 0;
        SubTimeCounter = 0;
        HalfASecond = 0;
      }
  else if(TimeButtonPress > THREE_SECOND){
      if((SubTimeCounter % ONE_TENTH_SECOND == 0)&&(SubTimeCounter > 0)){
        //SubTimeCounter always >= 1 when get this case
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
        ButtonPressed1s = 1;
        SubTimeCounter++;
        if((SubTimeCounter % HALF_A_SECOND == 0)&&(SubTimeCounter > 0)){HalfASecond = 1;}
        else HalfASecond = 0;
        }
}
//---------------------DISPLAY_VALUE-------------------------
void ValueDisplay(){
  //if No button is pressed, the state is changed to BUTTON1_STEP_INCREASE
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
                              Serial.print(Value, DEC);
                              Serial.println("\t\t2222222");
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
                              Serial.print(Value, DEC);
                              Serial.println("\t\t3333");
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
