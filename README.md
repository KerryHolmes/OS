# OS
Repository for running 3460:426 Operating Systems Project

#Contributors
Kerry Holmes 
Samuel Goodrick 

# Summary
This is a small 16 bit operating system based on the work of M. Black
Following the procedure in the lab, we have created basic kernel functionality.
In addition to that, the system is now able to load files and run programs.
There a limitations imposed on the size of files based on the limitations of the 
floppy disk that is used to run the operating system, and store its data.
The kernel changes the terminal display colors to a grey background with blue text.
The system is capable of correctly reading and writing both strings  up to 80 characters
in length and integers ( up to MAX_INT which in this case is 32767) using system 
interrupts. Descriptions of each function are commented above them in kernel.c. 
You can verify this functionality by running the commands below. This will launch 
one of the provided test programs. You can enter the name of another program in
the kernel.c file to test other programs. You will need to use the compile script
again after changing the program that will run.

# Use
To run this program you will need to build it from these source files and run it. 
To build the OS image you can run the compile script compileOS.sh from a terminal.
If you are unable to run the script you will need to run the following command to 
make it executable.
```
chmod +x compileOS.sh
```
*Note this requires you to have Bruce Evan's C Compiler (bcc).*
This will not only place all necessary data onto the floppy disk,
but also run the operating system using the bochs emulator.
EX:
```
./compileOS.sh
```
