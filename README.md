# SmartKeurig

Software to intelligently control a modified Keurig K40 coffee maker in my dorm's lobby.  Powered by Arduino, a Raspberry Pi, Arch Linux, and a lot of Caffeine.


I took apart a Keurig K40 coffee maker to take a look at its internal embedded systems and gain a better understanding of the logic behind the coffee brewing process.  Upon disassembly, I was able to gain access to a PCB that had push buttons, LEDs, various capacitors and resistors, and a pressure sensor. 

There were a few screw mounts holding this board in place, and a 16-pin cable attached, presumably to connect this interface board to an internal microcontroller.  Unfortunately, I was unable to figure out how to disassemble the machine further to gain access to the microcontroller without damaging the Keurig permanently.

Various inputs are given to the Keurig during the brewing process - the pressure of the water left in the tank to determine if enough is present to make coffee, a power button and brew size buttons to select coffee size, and a NC limit switch on the open/close lever used to detect when a new K-cup has been placed in the brewing slot.  Five LEDs are the devices output, giving information to the user on the current state of the brewing process.  A temperature sensor must be present because the device knows when the water is properly heated, but is not visible.

The picture on this page illustrates how I have soldered six wires to various points on the back of the control board to interface with this machine.  Because I was unable to gain access to the internal microcontroller of this Keurig, I connected these nine wires to my own ATMega328P-UP microcontroller’s digital input / output pins and wrote a program that simulates button presses needed to brew the coffee and interpret the status LED states to gain further insight and automate the brewing process.

After taking inputs from the temperature sensor, pressure sensor, and user interface buttons, the machine outputs various status LED states and connects power to a pump’s motor in order to brew the coffee.  The ATMega takes serial commands for the brew size and time to start the brew, and this in turn is connected to a Raspberry Pi running Arch Linux for ARM machines.  Thus, I am able to set a crontab for daily brewing automation, and can also SSH into the Raspberry Pi to issue commands to brew on demand from anywhere in the world with internet access.
