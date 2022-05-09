
******************************************************************************************************
Name - Roshan Raj Kanakaiprath
ASU ID - 1222478062

Description : 

An application program in Zephyr 2.6.0, which takes the measurement from an ultrasonic sensor(HC-SR04)
at a specific interval, and provides the lastest measured value when requested using the Shell CLI 
feature of Zephyr. PUTTY is used as the command line interface
*******************************************************************************************************************

***********************Steps to import the HC-SR04 driver to Zephyr source tree************************************

The driver and steps to add it to zephyr source tree can be found at https://github.com/inductivekickback/hc-sr04.
Do not have to import the driver for HC_SR04_NRFX variant.
Modifications are done to source file to report the distance in inches.


*****************Connection Steps for mimxrt1050_evk***************************************************************

2. Connect the Vcc and GND pins of HCSR-04 sensor to 5V and GND on the board respectively.
3. Connect trigger pin and echo pin respectively to PORT J22(pins 4 and 7)

*******************************************************************************************************************


***********************Steps to compile and execute the codse********************************************************


1. Download the project from the repository

2. Make the necessary changes to zephyr source tree.(port the distance sensor driver to zephyr source tree)

2. To build, run west build -b mimxrt1050_evk on the project.

3. Run west flash to flash the code into the board.
	
4. Open PUTTY and select port /dev/ttyACM0 and enter baud rate 115200 and start a new serial session

5. Press the reset button on the board.

6. On the terminal enter command "get_value" and press "ENTER" to get the latest measurement.

7. When done enter command "exit" and press ENTER to terminate the program.




	



