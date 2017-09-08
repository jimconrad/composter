/*
 * MotorController.cpp
 *
 * Developed for the Robot Shop RB-Cyt-133 30A 5-30V Single Brushed DC Motor Driver
 * 
 * The motor controller can determine if the motor is running, start the motor in a
 * particular direction, and stop the motor.
 * 
 * The motor starts and stops somewhat slowly to prevent damage to the mechanical drive
 * train.  The accel/decel feature is implemented here.
 * 
 * This implementation was developed for a Robot Shop RB-Cyt-133 30A 5-30V Single Brushed
 * DC Motor Driver.  See:  http://www.robotshop.com/en/30a-5-30v-single-brushed-dc-motor-driver.html 
 * 
 * This implementation relies on the Arduino analogWrite() method which implements a pulse-width
 * modulator whose duty cycle varies from 0..255.  The motor direction is controlled by a separate
 * digital output pin.  Because the motor controller draws a significant standby current, we use a
 * relay (controlled by yet another digital output pin) to power down the controller when it's not
 * needed.  We provide a brief delay after powering-up the controller for the relay to settle and
 * the controller's logic to initialize before we start the motor.
 *
 *  Created on: Apr 3, 2016
 *      Author: kq7b
 */

#include "Arduino.h"
#include "Composter.h"
#include "PDebug.h"
#include "MotorController.h"
#include "Battery.h"


/**
 * apwmPin - arduino pwm pin assigned to the motor controller
 * adirPin - arduino digital pin assigned to control the motor's direction
 * arelayPin - arduino digital pin (or 0 if none) assigned to power-up/down the motor controller
 */
MotorController::MotorController(byte apwmPin, byte adirPin, byte arelayPin) : 
  timer1(TIMER1_MS), timer2(MCMS) {
	pwmPin = apwmPin;
	dirPin = adirPin;
	relayPin = arelayPin;
	state = MOTORSTANDBY;
	pinMode(pwmPin,OUTPUT);			//Config PWM pin for output.
	pinMode(dirPin,OUTPUT);			//Config motor direction pin for output.
	if (arelayPin != 0) pinMode(relayPin,OUTPUT);
}

/**
 * start() is the API for starting the motor
 * 
 * Note:  The motor will not start if the battery is low.
 */
 void MotorController::start(bool direction) {

  DPRINT("Start");
  update();   //Bring state upto date

  //Ignore attempts to start motor when battery voltage is low
  if (Battery::isLow()) {
    DPRINT("start ignored --- vBat low");
    return;
  }

  //To avoid abrupt and potentially damaging drive train loads, the motor can only be
  //started when it is not rotating.  Invalid requests are ignored for safety.
  switch(state) {
    //Motor controller must be awakened from standby before use.
    case MOTORSTANDBY:
      state = MOTORAWAKENING;
      DPRINT("MOTORAWAKENING");
      dir = direction;                //Record new motor direction
      digitalWrite(relayPin,HIGH);    //Start the controller awakening
      timer1.start();                 //Motor will start after timer expires
      break;
    //Waiting for timer1 to expire before starting motor
    case MOTORAWAKENING:
      dir = direction;                //Record new motor direction
      break;
    //Start the motor immediately as it's not currently running.
    case MOTORSTOPPED:
    DPRINT("start Starting");
      dir = direction;                //New motor direction
      startMotor();                   //Starts motor immediately and changes state to MOTORRUNNING
      break;
    //These are the invalid states in which the motor cannot be started.  Ignore command.
    case MOTORRUNNING:
    case MOTORSTOPPING:
    default:
      break;              
  }
 }

 /**
  * Decelerate and stop the motor
  * 
  * To avoid damage to the drive train, we always decel the motor slowly using
  * timer1 to pace the speed reduction steps.
  */
  void MotorController::stop() {
    DPRINT("stop");
    update();   //State

    switch(state) {

      //If the motor is awakening then we'll need to clean things up before decel begins
      case MOTORAWAKENING:
        timer1.reset();                                   //Stop the awakening timer then fall thru to MOTORRUNNING

      //Begin decelerating (which continues in update())
      case MOTORRUNNING:
        state = MOTORSTOPPING;                            //Enter the deceleration state
        currentSpeed -= ACCEL_STEP_SIZE;                  //Slower speed
        if (currentSpeed < 0) currentSpeed = 0;           //Min speed is 0
        analogWrite(pwmPin,currentSpeed);                 //Slow it down
        timer1.start();                                   //Timer1 informs update() when speed can be further reduced
        break;
        
      //Sometimes, there's nothing to do
      case MOTORSTOPPING:
      case MOTORSTOPPED:
      case MOTORSTANDBY:
      default:
        break;
    }
    
  }

/**
 * update() checks the state and performs whatever, if anything, needs to be done.  update() can
 * be invoked at any time.
 */
 void MotorController::update() {
  
  switch(state) {

    //Motor is in power-save standby (powered-off) mode and is not currently needed.  Currently nothing to do.
    case MOTORSTANDBY:
    break;

    //Motor is awakening from standby and will be ready for use after controller's power relay settles and logic initializes
    case MOTORAWAKENING:
      if (timer1.isExpired()) {
        DPRINT("M update");
        state = MOTORSTOPPED;       //Timer has expired.  Motor is now stopped and ready to start.
        startMotor();               //Start the motor
      }
    break;

    //Motor is ready for use but no request has been made for it to run.  Shall we put it to sleep or keep waiting for something to do?
    case MOTORSTOPPED:
      if (timer2.isExpired()) {         //Can we enter standby mode yet to save power?
        digitalWrite(relayPin,LOW);     //Yes, Power-down the motor controller
        state = MOTORSTANDBY;           //The motor is officially asleep to save power
      }
    break;

    //Motor is accelerating or running in direction indicated by instance variable, dir
    case MOTORRUNNING:
      if (timer1.isExpired()) {
        int newSpeed = currentSpeed + ACCEL_STEP_SIZE;
        currentSpeed = (newSpeed>MOTOR_MAX_SPEED) ? MOTOR_MAX_SPEED : newSpeed;
        DPRINT(currentSpeed);
        analogWrite(pwmPin,currentSpeed);
        timer1.start();
      }
    break;

    //Motor is decelerating to a stop.  Either slow it further or bring it to a stop.
    case MOTORSTOPPING:
      if (timer1.isExpired()) {                           //Time to slow it some more?
        if (currentSpeed >= ACCEL_STEP_SIZE ) {           //Are we still slowing it down?
          currentSpeed -= ACCEL_STEP_SIZE;                //Yes, calc slower speed
          timer1.start();                                 //Timer1 notifies update() when we can slow it more
        } else {
          currentSpeed=0;                             //Stop the motor
          state=MOTORSTOPPED;
          timer2.start();                             //Start the standby timer             
        }
        DPRINT(currentSpeed);
        analogWrite(pwmPin,currentSpeed);           //Slow down the motor
      }
    break;
  }
  
 }

 //Get the motor state
 MotorState MotorController::getState() {
  return state;
 }

 //Is motor running?
 bool MotorController::isRunning() {
  return (state==MOTORRUNNING)||(state==MOTORAWAKENING);
 }

 //Is motor stopped or sleeping?
 bool MotorController::isStopped() {
  return (state==MOTORSTOPPED)||(state==MOTORSTANDBY);
 }

//Private method for starting the motor.  Motor must currently be stopped.
//New state will become MOTORRUNNING.  Motor will begin accelerating in the requested direction.
void MotorController::startMotor() {
  //Verify motor can be started from the current state
  switch(state) {
    //Start the motor if it's currently stopped
    case MOTORSTOPPED:
      timer2.reset();                       //Cancel timer that would have powered-down the motor
      state = MOTORRUNNING;                 //Motor is now running
      digitalWrite(dirPin,dir);             //Program controller with requested direction
      currentSpeed = MOTOR_STARTING_SPEED;  //Start the motor at this speed
      analogWrite(pwmPin,currentSpeed);     //Program pwm with the current speed
      timer1.start();                       //Timer informs us when speed can be increased
      break;
    //Motor cannot be started while in invalid states
    default:      
      break;
  }
  
 }

