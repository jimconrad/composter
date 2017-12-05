/****************************************************************
 * 
 * mos --- The Modest Operating System
 * 
 * The Modest Operating System implements a multi-tasking kernel
 * over the familiar Arduino runtime environment:
 * 
 *  This mos.ino sketch implements the mos initialization and
 *  main task scheduler loop --- this is not the user-supplied
 *  guest application program.  But it starts that app.
 *  
 *  A mos user supplies a guest application as a more-or-less 
 *  familiar C program (e.g. HelloWorld.c).  Mos starts the
 *  user-supplied program by invoking the mosMain() function.
 *  
 *  Definitions:
 *  Task --- A task is a mos process.  Each mos task has a
 *  priority (amongst other tasks), its own stack, and a
 *  currentState.
 */

 //MOS Includes
 #include "MOSTask.h"

  //Get a reference to the user-supplied app's main method
 extern void mosMain(void);

 //Definitions and global variables
 #define PRIORITY  128
 #define STACKSIZE 256
 static MOSTask mainTask = MOSTask(PRIORITY,STACKSIZE,mosMain);



/*
 * This is the mos initialization code, invoked by the Arduino
 * runtime environment.
 */
void setup() {
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
