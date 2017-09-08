/**
 * SoundMaker.h  
 * 
 *      Author: kq7b
 */

#ifndef SOUNDMAKER_H_
#define SOUNDMAKER_H_


class SoundMaker {
  
public:
  SoundMaker(byte);         //Constructor
  void doClick();
  void doBeep();
  void doBeep(int);
  
private:
  byte pin;                 //Arduino pin for speaker
  
};

#endif /* SOUNDMAKER_H_ */
