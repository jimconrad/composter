# composter
Arduino-based controller for an automated composter

# Author(s)
Jim Conrad, Boise, Idaho (USA), jimconrad@boisestate.edu

# Composting
A composter is a device for manufacturing compost from organic waste such
as grass clippings, leaves, etc.  Compost is an organic fertilizer, humus.
The composting process requires frequent aeration, often implemented by
"turning" the organic matter.  An automated composter incorporates a clock
and a motor to periodically and automatically aerate the organic matter.

# Contribution
This software implements an arduino-based controller for an automated
drum-based composter with a user-interface known as the control panel
including:
* On/Off Switch:  Main power switch, mostly used only for an emergency
* Button-1:  Rotate drum clockwise
* Button-2:  Rotate drum counter-clockwise
* Button-3:  Press to schedule the composter to run automatically
* Button-3:  Hold to cancel an existing schedule
* Speaker:   Provides audible feedback for the control panel operation
* LED-1:     Indicates the composter is scheduled to run automatically
* LED-2:     Indicates the system battery is discharged
* LED-3:     Indicates the system battery is overcharged
The composting system is designed to operate from a small storage battery
charged by a small solar panel, enabling it to be independently located
far from an electrical outlet.  The control panel is designed to be very
simple to use (no commands, menus, etc).

In addition to the buttons, LEDs and speaker noted above, the arduino
processor has the following peripherals:
* Motor Controller:  PWM controller for a 12VDC motor driving the drum
* Motor Relay:  Enable/Disable 12VDC power to the motor controller system
* ADC:  Monitors the system battery's voltage

# Potential Improvements
A better controller might monitor more parameters (e.g. moisture content
or temperature) of the mixture during composting.

# Copyright and License
(C) 2017 James R Conrad, Boise, Idaho (USA).  This is open-source software,
licensed with the MIT License (see "License" file in project folder).

# References
* Compost:          https://en.wikipedia.org/wiki/Compost 
* Arduino:          https://www.sparkfun.com/products/12640 
* Sketch vs C/C++   https://github.com/arduino/Arduino/wiki/Build-Process 
* Arduino Pro Micro https://www.sparkfun.com/products/12640
* Pro Micro Pins    https://cdn.sparkfun.com/datasheets/Dev/Arduino/Boards/ProMicro16MHzv1.pdf
* Sleep             https://github.com/rocketscream/Low-Power
* ATmega32u4 Sleep  https://arduino.stackexchange.com/questions/10408/starting-usb-on-pro-micro-after-deep-sleep-atmega32u4
* RTC               https://www.sparkfun.com/products/12708
* Motor Controller  http://www.robotshop.com/en/cytron-30a-5-30v-single-brushed-dc-motor-driver.html
* Relay Controller  https://www.sparkfun.com/products/13815
* Gear Motor:       http://www.surpluscenter.com/Electric-Motors/DC-Gearmotors/DC-Gearmotors/100-RPM-12-Volt-DC-Gearmotor-5-1649.axd
* Speaker:          https://www.sparkfun.com/products/7950 

# Manifest
Battery.*           Monitors the charge-level of the storage battery
Composter.*         An Arduino "sketch" implementing the main composter controller
LED.*               Controls a single LED on a specified Arduino pin
MotorController.*   Implements the slow-start/stop features of the motor control
PButton.*           Physical button debouncer
PDebug.*            Debuggin definitions for software developers
pinAssignments.h    Defines electrical connections to the Arduino 
PSleep.*            Processor sleep features (for power conservation)
PTimer.*            Yet another timer implementation
Schedule.*          Schedules the autorun at some specified time-of-day
SoundMaker.*        Clicks and beeps



