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

    //PButton is currently released.  
    case PBR:
      if (digitalRead(pin)==PRESSED) {  //Initial contact press?
        state = PBI;                    //Yes, button pressed
        debounceTimer.start();          //Start debounce timer
        DPRINT(">PBI");
      }
    break;

    //PButton has been initially pressed and is ignoring bounces until timer expires
    case PBI:
      if (debounceTimer.isExpired()) {  
        DPRINT("PBI isExpired"); 
        state = digitalRead(pin)==PRESSED ? PBP : PBR;
        debounceTimer.reset();
        DPRINT(state);
      }
    break; 

    //PButton is currently pressed
    case PBP:
      if (digitalRead(pin)==RELEASED) { //Released?
        state = PBX;                    //Yes, button initially released
        debounceTimer.start();          //Start debounce timer 
        DPRINT(">PBX");
      }

    //PButton has been initially released and is ignoring bounces until timer expires
    case PBX:
      if (debounceTimer.isExpired()) {
        DPRINT("PBX isExpired");
        state = digitalRead(pin)==RELEASED ? PBR : PBP;
        debounceTimer.reset();
        DPRINT(state);
      }
      break;
  }
 }

 //Public method to see if button has been pressed
 bool PButton::isPressed() {
  update();                                       //Update current state of PButton object  
  return (state==PBP);
 }

 //Public method to see if button has been released
 bool PButton::isReleased() {
  update();
  return (state==PBR);                      
 }

 //Public method to see if button is stable
 bool PButton::isStable() {
  DPRINT("isStable()");
  //update();    //race condition
  return (state==PBR||state==PBP);
 }

 //Get state of button
PButtonState PButton::getState() {
  return state;
 }
  

