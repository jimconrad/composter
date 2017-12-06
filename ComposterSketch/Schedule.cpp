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
  today = rtc.getDay();           //Remember which day we started

  //Booting up resets the scheduler's state
  composterRanToday=false;
  
}


/**
 * Update scheduler/rtc status 
 */
 void Schedule::update() {
  
  //If the day has changed then we haven't ran today
  rtc.update();                     //Update the clock
  byte thisDay = rtc.getDay();      //Get the day of the month from RTC
  if (thisDay != today) {           //Has it changed since we last checked?
    composterRanToday = false;      //Yes, then the composter hasn't ran today
    today = thisDay;                //Remember new day
  }
  
 }



/**
 * Program the composter to run daily at the current TOD
 */
 void Schedule::setStartTime() {

  long startingSecond;        //Second past midnight when composter will start (if enabled)
  composterRanToday=false;    //Re-programming the scheduler enables the composter to run (perhaps again) today

  DPRINT("setStartTime()");

  //Calculate the current time as seconds elapsed since last midnight 
  long currentSecond = rtc.getHour() * 3600L + rtc.getMinute()*60L + rtc.getSecond();

  //Don't configure the starting time in the last minute of the day because it will wrap around at midnight
  startingSecond = (currentSecond < 86340L) ? currentSecond : 0L;   //If it's close to midnight then use midnight for startingSecond

  //Record the startingSecond and enabled in non-volatile memory
  bool enabled=true;
  EEPROM.put(EESKEDSTART,startingSecond);
  EEPROM.put(EESKEDEN,enabled);
      
  }


  /**
   * Is it time to start the composter running?
   * 
   */
 bool Schedule::isTimeToStart() {

    bool enabled=false;
    long startingSecond;
    
    //Look-up state variables from non-volatile (EEPROM) memory
    EEPROM.get(EESKEDSTART,startingSecond); //Second past midnight when autorun starts if scheduler is enabled
    EEPROM.get(EESKEDEN,enabled);           //true if scheduler is enabled, false otherwise
    
    //We'll only consider starting the composter if the scheduler is actually enabled
    if (enabled) {

      //Calculate the current time as seconds elapsed since last midnight
      long currentSecond = rtc.getHour() * 3600L + rtc.getMinute()*60L + rtc.getSecond();

      DPRINT("isTimeToStart composterRanToday="+String(composterRanToday)+" Currently "+String(currentSecond)+", Scheduled "+String(startingSecond));
      
      //If the composter hasn't already ran then check to see if it's time to run
      return composterRanToday ? false : currentSecond>=startingSecond;
      
    } else {
      return false;                             //Composter's autoRun schedule isn't enabled
  }
 }



/**
 * Finished running today
 */
 void Schedule::setFinished() {
  composterRanToday=true;
 }


 /**
  * enabled --- Is scheduler enabled?
  */
  bool Schedule::enabled() {
    bool enabled;
    EEPROM.get(EESKEDEN,enabled);
    return enabled;
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
      return rtc.getHour();
    }

    /**
     * Get minute
     */
     byte Schedule::getMinute() {
      return rtc.getMinute();
     }

     

