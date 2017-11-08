/***********************************************************************************************
 * PButton.cpp --- debounced physical button 
 * 
 * Debouncing algorithm:  Button is considered pressed when contact is first detected.
 * Button is considered released after contact drops out for a specific time interval.
 * 
 * Note:  The pull-up resistors invert the input logic.  A NO button will be HIGH when
 * RELEASED and will be LOW when PRESSED.  The button should connect the Arduino pin to
 * ground via a resistor of about 330 ohms.
 *
 **********************************************************************************************/
#include "Composter.h"
#include "PDebug.h"
#include "Arduino.h"
#include "PButton.h"
#include "PTimer.h"

//#define DPRINT(x)

PButton::PButton(byte n) : debounceTimer(PBUTTON_DEBOUNCE_MS) {
	pin = n;                        //Arduino pin number
	pinMode(pin,INPUT_PULLUP);			//Button electrical contacts need a pull-up resistor
  state = PBR;                    //Buttons initialize in the released state
  debounceTimer = PTimer(PBUTTON_DEBOUNCE_MS);    //Timer used to debounce noisy button contact
}

//Public method to update timer status (debounce logic is in this method)
void PButton::update() {

  switch(state) {

    //PButton is currently released.  We ignore bounces while timer is running
    case PBR:
      if ((!debounceTimer.isRunning()) && digitalRead(pin)==PRESSED) {  //Initial contact press?
        state = PBP;                    //Yes, button pressed
        debounceTimer.start();          //Start debounce timer
        DPRINT(">PBP");
      }
    break;

    //PButton is currently pressed.  Ignore bounces until timer expires.
    case PBP:
      if (digitalRead(pin)==RELEASED) {   //This release might be noise
        state = PBI;                      //Ignore contact noise until timer expires
        DPRINT(">PBI");
      }
    break;

    //PButton is currently ignoring electrical contact noise after initial press
    case PBI:
      if (digitalRead(pin)==PRESSED) {                //If the button is pressed again...
        state = PBP;                                  //THen return to pressed state
        DPRINT(">PBP");
      } else if (debounceTimer.isExpired()) {         //If the debounce timer expired..
        state = PBR;                                  //Then consider the button released
        debounceTimer.start();                        //Causes PBR to ignore bounces
        DPRINT(">PBR");
      }
    break; 
  }
 }

 //Public method to see if button has been pressed
 bool PButton::isPressed() {
  update();                                       //Update current state of PButton object
  return (state==PBP)/*||(state==PBI)*/;
 }

 //Public method to see if button has been released
 bool PButton::isReleased() {
  update();
  return (state==PBR);                      
 }

 //Get state of button
PButtonState PButton::getState() {
  return state;
 }
  

