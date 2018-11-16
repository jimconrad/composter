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
#define pinB1  		  0		//BWN Digital INT pin (RXI) for Button 1 
#define pinB2  		  1 	//RED Digital INT pin (TXO) for Button 2 
#define pinB3       7   //ORG Digital INT pin for BUtton 3

//LED Indicators
#define pinSkedLED  4   //YEL Scheduler pgmd to autorun LED GRN
#define pinOvrLED   5   //GRN Battery overcharged       LED BLU
#define pinDisLED   6   //BLU Battery discharged        LED RED

//Motor controller pins (requires a PWM)
#define pinMotorDir 8 	//GRY Digital pin for motor direction
#define pinMotorPwm 9   //WHT Digital PWM pin for motor speed
#define pinMotorPwr 10	//BWN Digital pin for motor controller power supply

//Reserved pins for the SPI serial bus in case we ever install an LCD panel
#define pinAudio    15  //Speaker

#endif /* PINASSIGNMENTS_H_ */
