/**
 * These are debugging definitions used in the ComposterSketch
 */
 
//Define class thru which debug msgs flow
#define  DOUT   Serial
 
//Output a debug message through DOUT
#if DEBUG == 1
#define  DPRINT(s) DOUT.println(s);
#else
#define  DPRINT(s)
#endif

//Logging/startup messages (always written)
#define  LOG(s) DOUT.println(s);
