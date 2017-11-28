/**
 * Monitors composter battery's charge level
 * 
 */

class Battery {

public:
  static bool isLow();          //Is the battery voltage excessively low?
  static bool isHigh();         //Is the battery voltage excessively high?
  static int getVoltage();      //Read the battery voltage
 
private:

 
};

