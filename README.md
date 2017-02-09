# OS
Repository for running 3460:426 Operating Systems Project

# Summary
This is a small 16 bit operating system based on the work of M. Black
Following the procedure in the lab, I have created basic kernel functionality.
The kernel changes the terminal display colors to a grey background with blue text.
The system is capable of correctly reading and writing both strings  up to 80 characters
in length and integers ( up to MAX_INT which in this case is 32767) using system 
interrupts. Descriptions of each function are commented above them in kernel.c. 
You can verify this functionality by running the commands below, and then enter 
a string that is at most 80 characters and entering a number under MAX_INT 
when prompted.

# Use
To run this program you will need to build it from these source files and run it. 
To build the OS image you can run the compile script compileOS.sh from a terminal.
If you are unable to run the script you will need to run the following command to 
make it executable.
```
chmod +x compileOS.sh
```
*Note this requires you to have Bruce Evan's C Compiler (bcc).*

EX:
```
./compileOS.sh
```
Then you can run the image from the bochs emulator

EX:
```
bochs -f osxterm.txt
```
