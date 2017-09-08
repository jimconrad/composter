# composter
Arduino-based controller for an automated composter

# Author(s)
Jim Conrad, Boise, Idaho (USA)

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
* LED-1:     Indicates when the composter is scheduled to run automatically
* LED-2:     Indicates that the system battery is discharged
* LED-3:     Indicates that the system battery is overcharged
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
* Compost:  https://en.wikipedia.org/wiki/Compost 
* Arduino:  https://www.sparkfun.com/products/12640 
* Motor Controller:  http://www.robotshop.com/en/cytron-30a-5-30v-single-brushed-dc-motor-driver.html 


