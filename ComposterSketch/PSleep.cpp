/******************************************************************************************************************
 * This module is responsible for entering and awakening from the processor's sleep-mode
 * 
 * Notes:  There are many choices re. what to disable in the processor.  We are keeping these elements 
 * running:
 * 
 *  Timer0      The foundation for hte milis() and other wiring methods
 *  WDT         Interrupts an idle processor following an 8-second nap
 *  interrupts  Level change interrupts awaken processor when a button is pressed
 *  
 ******************************************************************************************************************/

 #include "Composter.h"
 #include "PDebug.h"
 #include "PTimer.h"
 #include "LowPower.h"
 #include "PSleep.h"



  //This is the idle timer used to measure the duration in ms of periods of inactivity
  static PTimer it = PTimer(IAMS);


/*
 * Constructor currently does nothing
 * 
 */

PSleep::PSleep() {

}


/**
 * Start the idle timer measuring a period of inactivity 
 */
 void PSleep::startIdleTimer() {
  DPRINT("startIdleTimer");
  it.start();  
 }



 /**
  * Determines if the idle period has expired
  */
  bool PSleep::isIdleTimerExpired() {
    //DPRINT("isIdleTimerExpired");
    return it.isExpired();    
  }



/**
 * Is the idle timer actively running?
 */
 bool PSleep::isIdleTimerActive() {
  //DPRINT("isIdleTimerActive");
  return it.isActive();
 }



  /**
   * Reset the idle timer (after composter has become active again)
   */
  void PSleep::resetIdleTimer() {
    DPRINT("resetIdleTimer");
    it.reset();
  }


 /**
  * Take a nap by configuring the processor to awaken in response to certain interrupts or WDT, place the
  * processor in some power-saving mode, and restore the operating environment following the nap.
  * 
  * Note:  Requests to sleep are ignored if battery voltage is excessive.  The idea is to drain the 
  * excessive charge.
  */
  void PSleep::sleepNow() {
    DPRINT("sleepNow()");

    DWAITUSB(1);              //Wait for usb when debugging

    //Stop the CPU and many functions while awkening after 8 seconds using the WDT or another specified interrupt
    TXLED0;                   //Snuff the TX Data LED
    RXLED0;                   //Snuff the RX Data LED
    LowPower.idle(SLEEP_8S,ADC_OFF,TIMER4_OFF,TIMER3_OFF,TIMER1_OFF,TIMER0_OFF,SPI_OFF,USART1_OFF,TWI_OFF,USB_OFF);

    //Awaken following a nap
    DWAITUSB(1);
    DPRINT("Awakening");

  }

