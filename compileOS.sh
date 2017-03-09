dd if=/dev/zero of=floppya.img bs=512 count=2880
#These statements add the precompiled data to different sectors
#on the disk
dd if=map.img of=floppya.img bs=512 count=1 seek=1 conv=notrunc
dd if=dir.img of=floppya.img bs=512 count=1 seek=2 conv=notrunc
./loadFile msg
./loadFile test1
./loadFile test2
#This will compile the bootloader and write the
#compiled bootloader into the floppy disk image
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc

#This will compile the kernel code using bruce's c compiler
bcc -ansi -c -o kernel.o kernel.c 

#Here the asembly code is compiled 
as86 kernel.asm -o kernel_asm.o 

#Finally, the asembly and c .o files are linked
ld86 -o kernel -d kernel.o kernel_asm.o 

#The final kernel is written out to sector 3 of the floppy
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3 

#Run the OS
bochs -f osxterm.txt
