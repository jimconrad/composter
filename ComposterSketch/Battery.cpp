/**
 * Battery.cpp --- Monitor the composter's battery charge
 * 
 * Note:  The battery is assumed to be a single 12V AGM lead-acid of about 8AH capacity and
 * is responsible for operating the processor as well as the motor driving the drum.
 * 
 * Note:  The arduino reads the battery's voltage using an analog input pin and scales the
 * reading so that an indicated value of 100 represents 10.0 Volts.  An external resistor
 * network divides the battery's voltage by 10 before applying it to the analog pin.
 * 
 * Note:  This implementation assumes that the battery is charged by a small solar panel.
 * If the solar panel manages to over-charge the battery, the associated firmware will
 * avoid sleeping the processor and will extend the duration of the scheduled autorun to
 * drain the charge.  When the battery is considered discharged, the associated firmware
 * will avoid running the motor to preserve what remains.
 * 
 * Note:  All the methods are implemented as statics because we assume the Composter has a
 * single battery (no need for Battery objects).
 * 
 * Misc:  The gear motor draws 6.0A continuous and is rated for 60A peak.  A one minute
 * run will consume 0.10AH.
 * 
 */

 #include <Arduino.h>
 #include "Composter.h"
 #include "PDebug.h"
 #include "PinAssignments.h"
 #include "Battery.h"


 //Define the min..max battery voltage range
 #define VMIN 110                   //11.0 Volts:  The battery is discharged.
 #define VMAX 140                   //14.0 Volts:  The battery is fully charged


 /**
  * getVoltage --- Returns the battery voltage scaled so that 100 represents 10.0 Volts
  */
  int Battery::getVoltage() {

    int vx10 = analogRead(pinBattery) / 2;      //Scaled such that 100 == 10.0 Volts
    //DPRINT(String("getVoltage=")+String(vx10));
    return vx10;    
  }


  /**
   * isLow --- Determines if the battery voltage is excessively low
   */
   bool Battery::isLow() {
    return getVoltage()<VMIN;
   }


   /**
    * isHigh --- Determines if the battery voltage is excessively high
    */
    bool Battery::isHigh() {
      return getVoltage()>VMAX;
    }

