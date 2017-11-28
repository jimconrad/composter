/*****************************************************************************************************************
 * Schedule.cpp --- Implementation of the composter's schedule determining when to toss the drum
 * 
 * Note:  The implementation assumes that it owns the SparkFun DS1307 Real-Time Clock (RTC).
 * Note:  Only enabled, not finished, is kept in EEPROM to avoid wearing out EEPROM write cycles
 * as the machine toggles every day to finished and not.
 * 
 ****************************************************************************************************************/
 
#include <Wire.h>
#include <SparkFunDS1307RTC.h>
#include <EEPROM.h> 

#include "Composter.h"
#include "PTimer.h"
#include "PDebug.h"
#include "Schedule.h"


/**
 * Constructor does nothing for now
 * 
 */
 Schedule::Schedule() {
  
 }


/**
 * Initialize composter's scheduler
 */
void Schedule::start() { 
  
  //Boot-up time initialization of the RTC 
  rtc.begin();                    //Setup I2C communication with RTC
  rtc.writeSQW(SQW_LOW);          //Disable the battery-sucking SQW feature
  rtc.set24Hour(true);            //Configure RTC for 24-hour service

  //Assume we haven't finished running today
  finished=false;
  
#if DEBUG==1
  //While debugging the code, we initialize the RTC NVM with the TOD each time the program starts-up
  rtc.autoTime();                 //Set the TOD to be the compile time. 
  DPRINT(String("RTC TOD set to ")+String(__DATE__)+String(" ")+String(__TIME__));
  DPRINT("Start at "+String(rtc.getHour())+":"+String(rtc.getMinute()));

  //In debugging mode, we initialized the scheduler disabled
  bool enabled=false;
  EEPROM.put(EESKEDEN,enabled);   
#else
  //We assume that the RTC NVM has been initialized and is running continuously on its battery after debugging
#endif

}


/**
 * Update scheduler/rtc status 
 */
 void Schedule::update() {
  rtc.update();
 }



/**
 * Program the composter to run daily at the current TOD
 */
 void Schedule::setStartTime() {

  long startingSecond;        //Second past midnight when composter will start (if enabled)
  bool enabled=false;         //Record of whether scheduler is enabled or not

  //Initializing finished to false will cause the composter to run after pressing B3
  finished=false;

  DPRINT("setStartTime()");

  //Calculate the current time as seconds elapsed since last midnight 
  long currentSecond = rtc.getHour() * 3600L + rtc.getMinute()*60L + rtc.getSecond();

#if DEBUG==1
  //If we're debugging, schedule the starting time for a minute from now so we wont wait a day
  startingSecond = (currentSecond < 86340L) ? currentSecond+60L : 0L;
#else
  //Don't configure the starting time in the last minute of the day because it will wrap around at midnight
  startingSecond = (currentSecond < 86340L) ? currentSecond : 0L;   //If it's close to midnight then use midnight for startingSecond
#endif

  //Enable the scheduled autorun
  enabled=true;

  //Record the startingSecond and enabled in non-volatile memory
  EEPROM.put(EESKEDSTART,startingSecond);
  EEPROM.put(EESKEDEN,enabled);
      
  }


  /**
   * Is it time to start the composter running?
   * 
   * Note:  The code assumes that isTimeToStart() will be invoked at least once during the last minute of the day
   * as the time-of-day wraps around to 0 at midnight.  That's why setStartTime() refused to configure the startingSecond
   * in the last minute of the day (to ensure that isTimeToStart() will be called at least once before the time wraps).
   */
 bool Schedule::isTimeToStart() {

    bool enabled=false;
    long startingSecond;
    
    //Look-up state variables from non-volatile (EEPROM) memory
    EEPROM.get(EESKEDSTART,startingSecond); //Second past midnight when autorun starts if scheduler is enabled
    EEPROM.get(EESKEDEN,enabled);         //true if scheduler is enabled, false otherwise
    
    //We'll only consider starting the composter if the scheduler is actually enabled
    if (enabled) {

      //Calculate the current time as seconds elapsed since last midnight
      long currentSecond = rtc.getHour() * 3600L + rtc.getMinute()*60L + rtc.getSecond();

      //Reset the scheduler just before midnight
      if (currentSecond >= 86340L) finished=false;

      //Is it time to run?
      if (finished) return false;                   //Avoid running more than once/day
      else return (currentSecond>=startingSecond);  //Time?

  } else {
      return false;                             //Composter's autoRun schedule isn't enabled
  }
 }



/**
 * Finished running today
 */
 void Schedule::setFinished() {
  finished=true;
 }


 /**
  * enabled --- Is scheduler enabled?
  */
  bool Schedule::enabled() {
    bool enabled;
    return EEPROM.get(EESKEDEN,enabled);
  }


  /**
   * Reset the composter's schedule (disables the auto-run program)
   */
   void Schedule::disable() {
    DPRINT("disable()");
    bool enabled=false;
    EEPROM.put(EESKEDEN,enabled);             //Record in non-volatile memory
   }

   /**
    * Get hour
    */
    byte Schedule::getHour() {
            //Calculate the current time as seconds elapsed since last midnight
      return rtc.getHour();
    }

    /**
     * Get minute
     */
     byte Schedule::getMinute() {
      return rtc.getMinute();
     }

     

