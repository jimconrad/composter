/*
 * PButton.h     
 *
 *  Created on: Apr 25, 2016
 *      Author: kq7b
 */

#ifndef PBUTTON_H_
#define PBUTTON_H_

#include "PTimer.h"

#define PBUTTON_DEBOUNCE_MS 10
#define PRESSED LOW
#define RELEASED HIGH

  enum PButtonState {
    PBI,  //Pressed and ignoring contact noise
    PBP,  //Pressed
    PBR   //Released
  };

class PButton {
public:
	PButton(byte); 
  void update();
	bool isPressed();
  bool isReleased();
  PButtonState getState();

private:
  byte pin;
  PButtonState state;
  PTimer debounceTimer;  //C++ Uniform Initialization of member variable debounceTimer
};

#endif /* PBUTTON_H_ */
