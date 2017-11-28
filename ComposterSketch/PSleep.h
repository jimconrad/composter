/**
 * Manages all aspects of placing the processor down for a nap to save power
 */

class PSleep {
 
public:
  PSleep();
  void sleepNow();
  bool isIdleTimerActive();
  bool isIdleTimerExpired();
  void resetIdleTimer();
  void startIdleTimer();

private:

 
};

