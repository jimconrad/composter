/**
 * Schedule.h --- Definitions for the composter schedule determining when to toss the drum
 * 
 * TODO:  Move the state variables into EEPROM so the composter doesn't lose the schedule
 * following a power-failure.
 *
 * 
 */

#ifndef SCHEDULE_H_
#define SCHEDULE_H_

class Schedule {
public:
  Schedule();                 //Constructor
  void start();               //Start communication with the RTC
  void setStartTime();        //Program composter to start daily at the current TOD
  bool isTimeToStart();       //Is it time to start the composter running?
  void disable();             //Disable the composter's programmed schedule
  void update();              //Update status
  void setFinished();         //Finished running today
  bool enabled();             //Scheduler enabled?
  byte getHour();             //Current time of day
  byte getMinute();           //Current time of day
  
private:
  bool finished;              //Composter has finished running on schedule today
};

#endif /* SCHEDULE_H_ */
