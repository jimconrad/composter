/*
 * PTimer.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: kq7b
 */

#include "Composter.h"
#include "PDebug.h"
#include "Arduino.h"
#include "PTimer.h" 
#include <avr/power.h>

PTimer::PTimer(long mS) {
	state = TIMERIDLE;
	expirationTime = 0;
	duration = mS;
}

/**
 * State the timer for the specified duration (milliseconds).  The timer will enter the TIMERACTIVE state, or, if
 * the millis() unsigned long will overflow before the timer expires then it enters the TIMERWILLWRAP state.
 */
void PTimer::start() {
	unsigned long currentTime = millis();		//What time is it now (milliseconds elapsed)?
	expirationTime = currentTime + duration;	//This summation may overflow if we've been running for a long while
	state = expirationTime>currentTime ? TIMERACTIVE : TIMERWILLWRAP; //Check for potential overflow
}

/**
 * This method is where Arduino-style code polls the timer
 */
void PTimer::update() {
	unsigned long currentTime = millis();		//What time is it now?
	switch(state) {
	case TIMERWILLWRAP:
		if (currentTime <= expirationTime) state = TIMERACTIVE;		//The millis() unsigned long has overflowed
		break;
	case TIMERACTIVE:
		if (currentTime >= expirationTime) state = TIMEREXPIRED; 	//The timer has expired
		break;
	case TIMERIDLE:
	default:
	  break; //Nothing to do
	}
}

/**
 * Check to see if the timer has expired
 */
bool PTimer::isExpired() {
	update();	//Update the timer's state
	if (state==TIMEREXPIRED) return true;
	else return false;
}

/**
 * Check to see if the timer is idle or expired
 */
 bool PTimer::isActive() {
  return !(state==TIMERIDLE || state==TIMEREXPIRED);
 }

/**
 * Is the timer running?
 * 
 */
 bool PTimer::isRunning() {
  update();
  return (state==TIMERACTIVE)||(state==TIMERWILLWRAP);
 }

/**
 * Reset the timer to mimic a new timer object
 */
void PTimer::reset() {
	state = TIMERIDLE;
	expirationTime = 0;
}


