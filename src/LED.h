/**
 * LED.h     
 * 
 *      Author: kq7b
 */

#ifndef LED_H_
#define LED_H_


class LED {
public:
  LED(byte); 
  void update();
  void doOn();
  void doOff();
  void set(bool);        //Set on/off
  
private:
  byte pin;
  
};

#endif /* LED_H_ */
