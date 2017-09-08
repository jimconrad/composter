/**
 * PTimer.h --- Yet Another Timer
 *
 * This is a polled timer designed for the Arduino Environment.  Unlike most timer classes, this
 * timer does not invoke a call-back when a timer object expires.  An application must poll the
 * status of a timer to determine its state (idle, active, expired).
 *
 * Note:  The arduino's elapsed-time clock, millis(), wraps around its unsigned long after about
 * 50 days.  We mitigate this issue with two workarounds:
 *
 * 	(1) We limit the maximum duration of a PTimer to be 2,147,483 mS.
 * 	(2) We anticipate wrap-arounds with an extra state, TIMERWILLWRAP
 *
 *
 *  Created on: Apr 4, 2016
 *      Author: kq7b
 */

#ifndef PTIMER_H_
#define PTIMER_H_

class PTimer {
public:
	PTimer(long);						//Build a timer for the specified duration mS
	void start();						//Start the timer for duration mS
	bool isExpired();					//Has timer expired?
	void reset();						//Reset the timer, stopping it if active
	void update();						//Update timer status
  bool isRunning();       //true if the timer is running
  bool isActive();        //true if the timer is not idle or expired
//  static void suspend();  //suspends all timer activity while processor sleeps
//  static void resume();   //resumes timer activity
private:
	enum TimerState {TIMERIDLE,			//Timer has not been started
					TIMERACTIVE,		//Timer has been started and is still running toward expirationTime
					TIMERWILLWRAP,		//Timer anticipates a wrap-around before running toward expirationTime
					TIMEREXPIRED		//Timer has expired but has not been reset nor re-started
	};
	unsigned long expirationTime;		//Time when the running timer will expire
	unsigned long duration;				//Elapsed duration of this timer object
	TimerState state;
};

#endif /* PTIMER_H_ */
