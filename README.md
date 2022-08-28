# Dedicated-Lock-System
Embedded system with STM32F103

Our project aims to create an electronic lock system for study room usage in a university library. The system can lock the door of a study room and unlock it for a reserved student. The main objective is to reduce disputes caused by the problem of the late arrival since the study room should be public to all the students if a booker does not show up.

## Main Features
* Read data form SDcard to get booker’s information.
* Display the current time on LCD.
* Display the coming time-slots with booker’s name on LCD.
* Lock the door when current time is in a reserved timeslot.
* Unlock for a booker with his/her student ID card.
* Unlock for other groupmates manully by input booker’s student ID.
* Change the room status to PUBLIC if booker is late for 10 mins.
* An unconditional unlock button for people inside the room to get out.

## Components
* STM32F103:
  * Main board used for all processes.
  * K1: Speed up the system time for demo.
  * K2: Trigger the keypad input function.

* LCD Display:
  * Show current time (system time)
  * Show room status
  * Show information of coming time slot 
  * Show entered number by keypad
  
* Electromagnetic lock:	
  * Unlock when received 12V power.
  * It can not be energized for more than 10 seconds.
  * We use a transistor connected with GPIO PA4 to control 
  * Set PA4 to HIGH to unlock
  * Delay 2 second, set it back to LOW
  
* Power:
  * Power Adaptor (input: AC 220V, output: DC 12V).
  * By testing, the voltage of 12V battery is not stable.
  
* RFID card reader module: M3650A-HA
  * It has 4 pins: 3.3VIN, GND, RX, TX
  * It uses UART TTL, with 13.56MHz

* SDCard
  * Necessary premise: #include “fatfs.h”
  * f_mount : establish the connect and the status of SD card, waiting for next command.
  * f_open : open the specific file(“booker.txt”) in the root directory of SD card
  * f_gets : read the text row by row sotred in “booker.txt”(the function ends when “\n” occurs or at the end of file and set the file pointer to next row.) - the combination off_read and f_lseek
 
## System Flow
<img width="453" alt="image" src="https://user-images.githubusercontent.com/89694131/187093102-6cbbbb88-486b-423f-b9ed-0b9e145716ed.png">


  
