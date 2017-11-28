/**
 * This is a standalone utility for programming the composter's Real Time Clock (RTC)
 * with a specified date and time.
 * 
 * The program will ask the user (via Serial connection to host Serial Monitor) to
 * input the month, day, year, hour and minute.
 */
#include <Wire.h>
#include <SparkFunDS1307RTC.h>

void setup() {
 
  //Boot-up time initialization of the RTC 
  rtc.begin();                    //Setup I2C communication with RTC
  rtc.writeSQW(SQW_LOW);          //Disable the battery-sucking SQW feature
  rtc.set24Hour(true);            //Configure RTC for 24-hour service

  //While debugging the code, we initialize the RTC NVM with the TOD each time the program starts-up
  rtc.autoTime();                 //Set the TOD to be the compile time. 
  Serial.println(("Start at "+String(rtc.getHour())+":"+String(rtc.getMinute())));

}

void loop() {

  //Read the date
  Serial.print("Enter MM:DD:YYYY date:  ");
  byte month = Serial.parseInt();
  byte day = Serial.parseInt();
  unsigned year = Serial.parseInt();

  //Read the time
  Serial.print("Enter HH:MM:SS 24-hour time:  ");
  byte hour = Serial.parseInt();
  byte minute = Serial.parseInt();
  byte second = Serial.parseInt();

  //Echo and confirm

}
