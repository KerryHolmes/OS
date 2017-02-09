# OS
Repository for running 3460:426 Operating Systems Project

# Summary
This is a small 16 bit operating system based on the work of M. Black

# Use
To run this program you will need to build it from these source files and run it. 
To build the OS image you can run the compile script compileOS.sh from a terminal.
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