/******************************************************************************************************************
 * SoundMaker --- Generates the various sounds enabling the control panel to provide audible feedback to user
 * 
 ******************************************************************************************************************/
 #include "Arduino.h"
 #include "SoundMaker.h"
 #include "Composter.h"


 /**
  * Constructor memorizes the pin number for the speaker
  */
  SoundMaker::SoundMaker(byte pinNumber) {
    pin = pinNumber;
  }


 /**
  * Generate a click... something emulating the sound of a mechanical button being pressed
  */
void  SoundMaker::doClick() {
    tone(pin,FREQC,1);
    delay(1);
  }


 /**
  * Generate a beep
  */
  void SoundMaker::doBeep() {
    tone(pin,FREQEF,500L);
    delay(500);
  }


 /**
  * Generate a beep at frequency f
  */
  void SoundMaker::doBeep(int f) {
    tone(pin,f,500L);
    delay(500);
  }
