/****************************************************************************
 * LED.cpp --- Controller for a Light Emitting Diode
 ***************************************************************************/

#include "Arduino.h"
#include "LED.h"

//Constructor needs to know what Arduino Pin controls the LED
 LED::LED(byte pinNumber) {
  pin=pinNumber;            //Record the arduino pin number
  pinMode(pin,OUTPUT);      //Config LED pin for output.
 }


 //Illuminate the LED
 void LED::doOn() {
  digitalWrite(pin,HIGH);
 }


 //Snuff the LED
 void LED::doOff() {
  digitalWrite(pin,LOW);
 }

 //Set on/off
 void LED::set(bool x) {
  digitalWrite(pin,x);
 }

