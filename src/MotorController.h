/*
 * MotorController.h
 *
 *  Created on: Apr 3, 2016
 *      Author: kq7b
 */
#include "Arduino.h"
#include "PTimer.h"
#ifndef MOTORCONTROLLER_H_
#define MOTORCONTROLLER_H_

//Define the max/starting "speed" (duty) values we send to the pulse-width modulator.  We
//start the motor at a minimum (though non-zero) value and accelerate it to a max value.
//Note that the direction of the motor is stored in the state, not in the current speed.
#define MOTOR_MAX_SPEED       255   //The maximum duty at which we'll run the motor (255 is full throttle)
#define MOTOR_STARTING_SPEED   10   //The duty at which we start the motor
#define MOTOR_ACCEL_MS        500   //Milliseconds during which motor will accel/decel
#define ACCEL_STEP_SIZE       5     //PWM increment/decrement for accel/decl motor

//Define the time-out intervals
#define TIMER1_MS   10L             //Used for awakening the controller and between speed adjustments     

//Motor direction
#define MCW  true                    //Clockwise
#define MCCW false                   //Counterclockwise

//Motor states
  enum MotorState  {MOTORSTANDBY,   //Motor controller has been powered-off
                    MOTORAWAKENING, //Motor controller is powering-up but is not yet ready for use
                    MOTORSTOPPED,   //The controller is powered-up but the motor is not running
                    MOTORRUNNING,   //The motor is running in the direction indicated by dir
                    MOTORSTOPPING}; //The motor is decelerating to a stop

class MotorController {
private:

	byte pwmPin;	        //Pulse-Width Modulator pin controls motor's speed
	byte dirPin;	        //Direction pin controls motor's rotation direction
	byte relayPin;        //Relay pin powers-up the motor controller when needed
	MotorState state;     //Motor Controller object's state
  byte currentSpeed;    //The motor's current speed (255 == full throttle)
  bool dir;             //The motor's direction if running
  PTimer timer1;        //Provides delay for speed adjustments and awakening from standby
  PTimer timer2;        //Provides long delay for placing controller in standby when drum is idle
  void startMotor();    //Accelerates motor from stop to MOTOR_MAX_SPEED
public:
	MotorController(byte,byte,byte);
	bool isRunning();
  bool isStopped();
	void stop();
	void start(bool);
  void update();
  MotorState getState();
};

#endif /* MOTORCONTROLLER_H_ */
