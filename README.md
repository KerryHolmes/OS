# OS
Repository for running 3460:426 Operating Systems Project

#Contributors
Kerry Holmes &
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
You can verify this functionality by running the commands below. You will then be prompted
to enter the name of a test program to run. The programs loaded are test1, test2, cal, 
and fib. Each of the prgrams hangs the OS after execution so you will need to quit and
run again to enter another program. You will receive errors if you do not correctly enter
one of the file names above. *Note that msg is also on the disk but is not a program. Its
contents can be written to the screen by uncommenting the noted lines in the main function.*
The test programs are provided curtesy of Dr. Oneil.

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
After you have the done this at least onece, you can continue to use
this command to launch the OS or you can run the command below.
```
bochs -f osxterm.txt
```

The OS will boot into a shell prompt. For a list of avaiable commands, you can type `help`
into the shell.
