
#include "Composter.h"
#include "PDebug.h"
#include "PButton.h"
#include "PSleep.h"
#include "pinAssignments.h"
#include "MotorController.h" 
#include "Schedule.h"
#include "Battery.h"
#include "LED.h"
#include "SoundMaker.h"

/******************************************************************************************************************
 * ComposterSketch Arduino sketch, the main program, for the Composter Controller
 * 
 * Control Panel Functionality
 *  Button1 press:    Rotate drum clockwise (CW) until released
 *  Button2 press:    Rotate drum counterclockwise (CCW) until released
 *  Button3 press:    Program drum to autorun periodically starting 24 hours from now
 *  Button3 hold:     Cancel drum scheduler's autorun program
 *  
 * Other Functionality
 *  Sleep:            Microprocessor naps after period of inactivity
 *  Awaken:           Microprocessor awakens after sleeping
 *  Battery:          Sleeps and ignores autorun schedule if discharged, sucks power if overcharged
 *  Sound:            Audible feedback (clicks and beeps) to control panel user
 *  
 * Resource Usage:
 *  arduino pins      Defined in pinAssignments.h
 *  timer0            millis and delay
 *  timer1            PWM controlling motor speed on pin 9
 *  WDT               Watchdog timer awakens processor from nap with an interrupt after 8 seconds of idleness
 *  
 *  References
 *  Sketch for C/C++  https://github.com/arduino/Arduino/wiki/Build-Process 
 *  Arduino Pro Micro https://www.sparkfun.com/products/12640
 *  Pro Micro Pins    https://cdn.sparkfun.com/datasheets/Dev/Arduino/Boards/ProMicro16MHzv1.pdf
 *  Sleep             https://github.com/rocketscream/Low-Power
 *  ATmega32u4 Sleep  https://arduino.stackexchange.com/questions/10408/starting-usb-on-pro-micro-after-deep-sleep-atmega32u4
 *  RTC               https://www.sparkfun.com/products/12708
 *  Motor Controller  http://www.robotshop.com/en/cytron-30a-5-30v-single-brushed-dc-motor-driver.html
 *  Relay Controller  https://www.sparkfun.com/products/13815
 *  Gear Motor:       http://www.surpluscenter.com/Electric-Motors/DC-Gearmotors/DC-Gearmotors/100-RPM-12-Volt-DC-Gearmotor-5-1649.axd
 * 
 *****************************************************************************************************************/

//Define the composter states
enum comState {
  IDL,             //Machine is idle (motor stopped)
  RCW,             //Running CW
  RCC,             //Running CCW
  DCL,             //Decelerating to a stop
  B3W,             //Button 3 wait
  ARN,             //Autorunning the drum
  NAP              //Processor is napping to save power
};

//Define objects referenced by composter controller
static Schedule sked = Schedule();               //Autorun scheduler
static PSleep nap = PSleep();                    //Governs nap time
static MotorController motor(pinMotorPwm,pinMotorDir,pinMotorPwr);
static PButton b1(pinB1);                        //CW button
static PButton b2(pinB2);                        //CCW button
static PButton b3(pinB3);                        //Auto/Cancel button sets/clears autoRun flag
static PTimer  b3t = PTimer(BHMS);               //User must press b3 this many ms to "hold" it
static PTimer  art = PTimer(ARMS);               //Autorun duration (how long the drum rotates)
static LED lowBattery = LED(pinDisLED);          //The Low (discharged) Battery LED
static LED highBattery = LED(pinOvrLED);         //The High (overcharged) Battery LED
static LED scheduled = LED(pinSkedLED);          //The Autorun Scheduled LED
static SoundMaker audio = SoundMaker(pinAudio);  //The speaker


//Composter state variable.  The FSM analyzes the control panel button activity.
comState state;                           //This is the FSM's state var

//Initialize the composter system
void setup() {
  
  state=IDL;                              //Initial state is IDL

  //Try to setup 32U4 USB communication with host computer if present, blinking 3X or until USB is ready
  Serial.begin(57600);
  for(int i=1;(i<=3)&&(!SerialUSB);i++) {
    audio.doBeep(FREQG);
    lowBattery.doOn();
    highBattery.doOn();
    scheduled.doOn();
    delay(1000);      
    lowBattery.doOff();
    highBattery.doOff();
    scheduled.doOff();
    delay(100);                    
  }

  //Setup interrupt handlers so buttons will awaken processor from a nap
  attachInterrupt(digitalPinToInterrupt(pinB1),intHan,CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB2),intHan,CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB3),intHan,CHANGE);

  //Startup the remainder
  sked.start();                                     //Scheduler

}

//Composter's activity loop executed forever
void loop() {
  
  //Poll and Update the status of objects that won't get updated otherwise
  b3t.update();
  art.update();
  motor.update();
  sked.update();

  //Update the LEDs as appropriate
  lowBattery.set(Battery::isLow());                 //Battery discharged?
  highBattery.set(Battery::isHigh());               //Battery Overcharged?
  scheduled.set(sked.enabled());                    //Autorun scheduler enabled?
  delay(10);                                        //Ensure flashing LEDs get seen

  //Composter state determines what to do with incoming events
  switch(state) {

    //Button events while machine is idle.
    case IDL:
      if (b1.isPressed()) {
        DPRINT("b1");
        audio.doClick();
        motor.start(MCW);
        state=RCW;
      } else if (b2.isPressed()) {
        DPRINT("b2");
        audio.doClick();
        motor.start(MCCW);
        state=RCC;
      } else if (b3.isPressed()) {
        DPRINT("b3");
        audio.doClick();
        state=B3W;                  //Will wait to see if B3 will be held
        b3t.start();                //Start the B3 timer
      } else if (sked.isTimeToStart()) {
        doStartMotor();             //Start the motor
      } else {                      //Composter is inactive
        if (nap.isIdleTimerExpired()) {             //If the inactive interval timer has expired then put the processor to sleep
          doNap();
        } else if (!nap.isIdleTimerActive()) {      //Start inactive timer if not already running
          DPRINT("start idle timer");
          nap.startIdleTimer();                     //This is where we begin measuring an inactive period's duration
        }
      }
    break;

    //Button events, scheduler or WDT can awaken composer from a nap
    case NAP:
      if (Battery::isLow()) {                     //Has the battery discharged while composter naps?
        DPRINT("discharged");
        audio.doBeep(FREQEF);                     //Error sound (the minor drop for Cm)
        doNap();                                  //Return to anp
      } else if (b1.isPressed()) {
        DPRINT("b1 fm nap");
        audio.doClick();
        motor.start(MCW);
        state=RCW;
      } else if (b2.isPressed()) {
        DPRINT("b2 fm nap");
        audio.doClick();
        motor.start(MCCW);
        state=RCC;
      } else if (b3.isPressed()) {
        DPRINT("b3 fm nap");
        audio.doClick();
        state=B3W;                  //Will wait to see if B3 will be held
        b3t.start();                //Start the B3 timer
      } else if (sked.isTimeToStart()) {
        doStartMotor();
      } else {
        DPRINT("Nothing to do here");
        doNap();                    //Probably WDT awoke us.  Put the composter down for a nap
      }
    break;

    //Button events while machine runs CW
    case RCW:
      if (b1.isReleased()) {      //Stop motor after B1 released
        DPRINT("rcwb1");
        motor.stop();
        state=DCL;
      }
    break;

    //Button events while machine runs CCW
    case RCC:
      if (b2.isReleased()) {      //Stop motor after B2 released
        DPRINT("rccb2");
        motor.stop();
        state=DCL;
      }
    break;

    //Any button activity or the autorun timer will stop autorunning
    case ARN:
      if (b1.isPressed()||b2.isPressed()||b3.isPressed()||art.isExpired()) {
        DPRINT("~arn");
        audio.doClick();
        motor.stop();         //Stop the motor
        state=DCL;            //Decelerate to stop
      }

    //Ignore events until motor decels to a stop then become idle
    case DCL:
      if (motor.isStopped()) {
        DPRINT("stopped");
        state=IDL;
      }
    break;

    //Awaiting B3 tapped/held decision to determine if user enabled/disabled autoRun
    case B3W:
      if (b3.isReleased()&&b3t.isRunning()) {       //Did user tap B3?
        sked.setStartTime();                        //Set now as the start time & enable the daily composter autoRun
        state=IDL;
      } else if (b3.isPressed()&&b3t.isExpired()) {  //Did user hold B3?
        audio.doBeep(FREQG);
        sked.disable();                               //Disable autoRun schedule
        state=B3W;                                    //Return to IDL later when user releases B3
        DPRINT("~A");
      } else if (b3.isReleased()&&b3t.isExpired()) {
        state=IDL;                                    //Released button
        sked.disable();                               //only for race condition
      }
    break;
    
  }
    
}

/**
 * Helper method to place the composter down for a power-saving nap
 */
void doNap() {
  DPRINT("doNap");      

  //Don't sleep the processor if battery is overcharged
  if (!Battery::isHigh()) {

    state=NAP;                                //When we awaken, we'll need to know what we were doing

    //Snuff the LEDs to save power.  They'll light up as needed in loop()
    lowBattery.doOff();
    highBattery.doOff();
    scheduled.doOff();

    //Now place CPU down for a nap
    nap.resetIdleTimer();                     //Reset the idle timer and...
    nap.sleepNow();                           //Put the CPU down for a nap to save power.
    
   }
}


/**
 * Helper method to start the drum motor in autorunning state
 */
void doStartMotor() {
        DPRINT("doStartMotor");
        motor.start(MCW);           //Start the motor
        art.start();                //Start the timer that ends autorun
        state=ARN;                  //Autorunning state
        sked.setFinished();         //Tell sked we did it today
}


/*
 * Do nothing interrupt handler.  Button interrupts are enabled only to arrange for buttons to
 * awaken the processor from a nap.
 * 
 */
 void intHan() {

 }



