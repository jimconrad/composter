/*
 * pinAssignments.h for Arduino pro micro
 * 
 * This file defines how the composter is wired to the arduino bus pins
 *
 *  Created on: Apr 5, 2016
 *      Author: jimconrad
 */

#ifndef PINASSIGNMENTS_H_
#define PINASSIGNMENTS_H_

//Arduino analog pin assignments for the composter system
#define pinBattery A0  //Analog pin A0 (18 on board) measures the battery voltage

//Reserved pins for the I2C communications bus (used for hw rtc)
#define pinSDA      2   //I2C SDA
#define pinSCL      3   //I2C SCL

//Control panel buttons must be on external (level) interrupt pins to wake-up processor
#define pinB1  		  0		//Digital INT pin for Button 1 
#define pinB2  		  1 	//Digital INT pin for Button 2 
#define pinB3       7   //Digital INT pin for BUtton 3

//LED Indicators
#define pinSkedLED  4   //Scheduler pgmd to autorun composter drum
#define pinOvrLED   5   //Battery overcharged LED
#define pinDisLED  6    //Battery discharged LED

//Motor controller pins (requires a PWM)
#define pinMotorDir 8 	//Digital pin for motor direction
#define pinMotorPwm 9   //Digital PWM pin for motor speed
#define pinMotorPwr 10	//Digital pin for motor controller power supply

//Reserved pins for the SPI serial bus in case we ever install an LCD panel
#define pinAudio    16  //Speaker

#endif /* PINASSIGNMENTS_H_ */
