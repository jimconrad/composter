/**
 * Global definitions for the Composter sketch 
 */

//Debug configuration
 #define  DEBUG  1

 //Wait for USB Serial port when debugging
 #define  DWAITUSB(n)    {for(int i=1;i<=n&&(!SerialUSB);i++) delay(1000);}
 
 
//How many ms to wait before deciding the B3 button has been held
#define BHMS 500

//Programmed timer durations (with special cases for debugging)
#if DEBUG==1
#define ARMS 5000L              //Autorun duration.  In debug mode, autorun duration 5 seconds
#define MCMS 5000L              //Power-off the motor controller after 5 seconds of inactivity
#define IAMS 20000L             //Inactive interval. In debug mode, place processor to sleep after 20 seconds of inactivity
#else
#define ARMS 60000L             //Autorun duration.  In production mode, autorun for 60 seconds
#define MCMS 5000L              //Power-off the motor controller after 5 seconds of inactivity
#define IAMS 30000L             //Inactive interval. In production mode, place processor to sleep after 60 seconds of inactivity
#endif

//EEPROM address assignments
#define EESKEDSTART 0          //Locations 0..3 reserved for Scheduler's long startTime
#define EESKEDEN 4             //Location 4 reserved for Scheduler's bool enabled


//Define the frequencies of some audio notes
#define FREQC   523           //Hz
#define FREQEF  622           //Eflat
#define FREQE   659           //E
#define FREQG   784           //G
