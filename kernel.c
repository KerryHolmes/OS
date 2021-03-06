/* ACADEMIC INTEGRITY PLEDGE                                              */
/*                                                                        */
/* - I have not used source code obtained from another student nor        */
/*   any other unauthorized source, either modified or unmodified.        */
/*                                                                        */
/* - All source code and documentation used in my program is either       */
/*   my original work or was derived by me from the source code           */
/*   published in the textbook for this course or presented in            */
/*   class.                                                               */
/*                                                                        */
/* - I have not discussed coding details about this project with          */
/*   anyone other than my instructor. I understand that I may discuss     */
/*   the concepts of this program with other students and that another    */
/*   student may help me debug my program so long as neither of us        */
/*   writes anything during the discussion or modifies any computer       */
/*   file during the discussion.                                          */
/*                                                                        */
/* - I have violated neither the spirit nor letter of these restrictions. */
/*                                                                        */
/*                                                                        */
/*                                                                        */
/* Signed:____Kerry Holmes & Samuel Goodrick___ Date:___02/04/2017___     */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS kernel, Version 1.03, Spring 2017.                 */

void handleInterrupt21(int,int,int,int);

void main()
{
    char file[6];
    char buffer[13312];
    int size;
    makeInterrupt21();
    interrupt(33,12,8,10,0);
    
    interrupt(33,0,"___.   .__                 __       .___           \r\n\0",0,0);
    interrupt(33,0,"\\_ |__ |  | _____    ____ |  | __ __| _/___  ______\r\n\0",0,0);
    interrupt(33,0," | __ \\|  | \\__  \\ _/ ___\\|  |/ // __ |/ _ \\/  ___/\r\n\0",0,0);
    interrupt(33,0," | \\_\\ \\  |__/ /\\ \\\\  \\___|    </ /_/ ( <_> )___ \\ \r\n\0",0,0);
    interrupt(33,0," |___  /____(____  /\\___  >__|_ \\____ |\\___/____  >\r\n\0",0,0);
    interrupt(33,0,"     \\/          \\/     \\/     \\/    \\/         \\/ \r\n\0",0,0);
    interrupt(33,0," V. 1.03, C. 2017. Based on a project by M. Black. \r\n\0",0,0);
    interrupt(33,0," Author(s): Kerry Holmes & Sam Goodrick\r\n\r\n\0",0,0);
    
    /*Uncomment the two lines bekow this to see the contents of msg
      printed onto the screen.*/    
/*    interrupt(33,3,"msg\0",buffer,&size);
    interrupt(33,0,buffer,0,0);  
*/
    /*Get test program from the user*/
    /*interrupt(33,0,"Enter a file name (Max 6 characters): \0",0,0);
    interrupt(33,1, file, 0, 0);
    interrupt(33,0,"\r\n\0",0,0);
    */
    /* Run the program. */
   /* interrupt(33,4,file,2,0);
    interrupt(33,0,"Error if this executes\r\n\0",0,0);
   
    while(1);*/

    interrupt(33, 4, "Shell\0",2,0);
    interrupt(33,0,"Bad or missing command interpreter.\r\n\0",0,0);

  while(1);
}

/*This function will output a string onto the screen. 
The string is passed in as a character array and printed
to the screen using interrupt 16 and function 14*/
void printString(char* c)
{
    int i;
    for(i = 0; *(c+i) != '\0'; ++i)
        interrupt(16, 14*256+*(c+i),0,0,0);
    return;
}

/*readString takes a character array with at least 80 elements but nothing in them. 
It calls interrupt 22 repeatedly and save the results in successive elements of the 
character array until the ENTER key is pressed (ASCII 13 or 0xD). It then adds 0x0 
(end of string) as the last character in the array and returns. Every character is
printed to the screen after being typed. If the BACKSPACE key is pressed, it is not
stored, but the current position in the character array is moved backwards.*/
void readString(char* c)
{
    int i = 0;
    char current;
    
    do
    {
	/* interrupt 22 accepts a single keystroke and stores it in current */
	current = interrupt( 22, 0, 0, 0, 0 );
	/* print the inputted character with interrupt 16 */
	interrupt( 16, 14*256+current, 0, 0, 0 );
	*(c + i) = current;

	/* if BACKSPACE is pressed, decrement unless at beginning of string */
	if( current == 0x8 )
	{
	    if( i > 0 )
		--i;
	}
	else
	    ++i;
    } while( current != 0xD ); /*continue until ENTER is pressed */
    *(c + (i - 1)) = '\0';
    return;
}
/*This function will issue 24 carriage return/newline 
combinations; issue the command interrupt(16,512,0,0,0);
Finally if both bx and cx are bigger than zero execute
interrupt(16, 1536, 4096 * (bx – 1) + 256 * (cx – 1), 0, 6223)
which will change the terminal color based on bx and cx values*/
void clearScreen(int bx, int cx)
{
    int i;
    for(i=0; i < 24; ++i)
        printString("\r\n\0");
    interrupt(16,512,0,0,0);
    if( bx > 0 && bx < 9 && cx > 0 && cx < 16)
        interrupt(16, 1536, 4096 * (bx - 1) + 256 * (cx - 1), 0, 6223);
    return;
}

/*These three functions were provided by Doctor Oneil and
serve to implement modulus, division, and writing characters
to the console screen.*/
int mod(int a, int b)
{
    int x = a;
    while (x >= b) x = x - b;
    return x;
}

int div(int a, int b)
{
    int q = 0;
    while (q * b <= a) q++;
    return (q - 1);
}

/*Writes an integer to the screen*/
void writeInt(int x)
{
    char number[6], *d;
    int q = x, r;
    if (x < 1)
    {
        d = number; *d = 0x30; d++; *d = 0x0; d--;
    }
    else
    {
        d = number + 5;
        *d = 0x0; d--;
        while (q > 0)
        {
            r = mod(q,10); q = div(q,10);
            *d = r + 48; d--;
        }
        d++;
    }
    printString(d);
}

/* This function reads a number as a character string, from
left to right and converts the ASCII characters for each individual
digit to the corresponding numeric value, correctly weights
each value and add them into a running sum. Then stores the sum at
the address provided as an argument. */
void readInt(int* number)
{
    char* input;
    int i = 0,sum = 0;
    readString(input);
    for(; i < 5 && *(input + i) != '\0'; ++i)
    {
        sum *= 10;
        sum  += *(input + i) - 48;
        
    }
    *number = sum;
    return;
}

/*This is the error function that was provided by Dr. Oneil*/
void error(int bx)
{
   char errMsg0[18], errMsg1[17], errMsg2[13], errMsg3[17];

   errMsg0[0] = 70; errMsg0[1] = 105; errMsg0[2] = 108; errMsg0[3] = 101;
   errMsg0[4] = 32; errMsg0[5] = 110; errMsg0[6] = 111; errMsg0[7] = 116;
   errMsg0[8] = 32; errMsg0[9] = 102; errMsg0[10] = 111; errMsg0[11] = 117;
   errMsg0[12] = 110; errMsg0[13] = 100; errMsg0[14] = 46; errMsg0[15] = 13;
   errMsg0[16] = 10; errMsg0[17] = 0;
   errMsg1[0] = 66; errMsg1[1] = 97; errMsg1[2] = 100; errMsg1[3] = 32;
   errMsg1[4] = 102; errMsg1[5] = 105; errMsg1[6] = 108; errMsg1[7] = 101;
   errMsg1[8] = 32; errMsg1[9] = 110; errMsg1[10] = 97; errMsg1[11] = 109;
   errMsg1[12] = 101; errMsg1[13] = 46; errMsg1[14] = 13; errMsg1[15] = 10;
   errMsg1[16] = 0;
   errMsg2[0] = 68; errMsg2[1] = 105; errMsg2[2] = 115; errMsg2[3] = 107;
   errMsg2[4] = 32; errMsg2[5] = 102; errMsg2[6] = 117; errMsg2[7] = 108;
   errMsg2[8] = 108; errMsg2[9] = 46; errMsg2[10] = 13; errMsg2[11] = 10;
   errMsg2[12] = 0;
   errMsg3[0] = 71; errMsg3[1] = 101; errMsg3[2] = 110; errMsg3[3] = 101;
   errMsg3[4] = 114; errMsg3[5] = 97; errMsg3[6] = 108; errMsg3[7] = 32;
   errMsg3[8] = 101; errMsg3[9] = 114; errMsg3[10] = 114; errMsg3[11] = 111;
   errMsg3[12] = 114; errMsg3[13] = 46; errMsg3[14] = 13; errMsg3[15] = 10;
   errMsg3[16] = 0;


   switch(bx) {
   case 0: printString(errMsg0); break;
   case 1: printString(errMsg1); break;
   case 2: printString(errMsg2); break;
   default: printString(errMsg3);
   }
}

/*This function reads in a specified sector from memory into a provided 
buffer. Modulus and division are used to translate the sector to its actual location.
Interrupt 19 is called to transfer the data.*/
void readSector(char* buffer, int sector)
{
    int relSecNo = mod(sector, 18) + 1;
    int headNo = mod(div(sector, 18), 2);
    int trackNo = div(sector, 36);

    interrupt(19, 513, buffer, (trackNo * 256) + relSecNo, headNo * 256); 
}

/*This helper function for read file compares two strings,
passed as char arrays. The strings can be a maximum of 6 characters
due to limitations on our file system. The function returns 1 if the
strings are the same and 0 if they are not.*/
int lex_compare(char* original, char* compare)
{
    int i = 0;
    for(i = 0; i < 6; ++i)
    {
        if(*(original + i) == 0 && *(compare + i) == 0)
            return 1;
	if(*(original + i) != *(compare + i))
	    return 0;
    }
    
    return 1;
}
/*Searches the directory passed in for a file name and returns the
index of the file.*/
int locate_file(char* directory, char* name)
{
    int i;
    for(i = 0; i < 16; ++i)
    {
       if(*(directory + i * 32) != 0x0)
       {
           if(lex_compare(name, (directory + i * 32)))
           {
               return (directory + i * 32);
           }
       }
    }
    return 0;
}

/*Returns the directory position of a file (0 - 15)*/
int file_index(char* directory, char* name)
{
    int i;
    for(i = 0; i < 16; ++i)
    {
       if(*(directory + i * 32) != 0x0)
       {
           if(lex_compare(name, (directory + i * 32)))
           {
	       return i;
           }
       }
    }
    return 0;    
}

/*This function reads a file into a buffer. The function requires 
a filename, a buffer and a size. The file is found in the directory
using the lex compare function above. After it is found, the file
is loaded from the specified sectors into the buffer, by repeatedly
calling the read Sector function. If the file is not found an 
error is returned.*/
void readFile(char* fname, char* buffer, int* size)
{
    char directory[512];
    int j;
    char* position = buffer;
    char* file;
    readSector(directory, 2);
    file = locate_file(directory,fname);
    if(file)
    {
        *size = 0;
        for(j = 6; j < 31; ++j)
        {
            if(*(file+j) == 0)
                return;
		
	    readSector(position,*(file + j));
		
            *size += 1;
            position += 512;
        }
        return;    
     }    
    error(1);
}

/*This function runs a program with the name specified as an argument.
It uses the readFile program above to load the program, and then uses
putInMemory and launchProgram to initiate it. If readFile does not find
the program the function exits before writing to memory, and does nothing.*/
void runProgram(char* name, int segment)
{
    char buffer[13312];
    int size, real_segment, i;
    readFile(name, buffer, &size);
    if(*buffer == 0)
      return;
    real_segment = segment * 4096;
    for(i = 0; i < size*512; i++)
    {
        putInMemory(real_segment, i, *(buffer + i));
    }
    launchProgram(real_segment);
}

/*This function was provided by Dr. Oneil.*/
void stop() { launchProgram(8192); }

/*Writes the contents of buffer into the specified
  sector.*/
void writeSector(char* buffer, int sector)
{
    int relSecNo = mod(sector, 18) + 1;
    int headNo = mod(div(sector, 18), 2);
    int trackNo = div(sector, 36);
    
    interrupt(19, 769, buffer, (trackNo * 256) + relSecNo, headNo * 256);
}

/*Sets the first bit of the file name to 0 and then
  iterates through the map marking every sector for this file
  as free.*/
void deleteFile(char* name)
{
    char map[512];
    char directory[512];
    char* file;
    int i, j, index, cnt;

    readSector( map, 1 );
    readSector( directory, 2 );

    cnt = 0;
    for( i = 0; i < 512; ++i )
    {
	if( map[i] != 0x00 )
	    ++cnt;
    }

    file = locate_file( directory, name );
    index = file_index( directory, name );
    if( !file )
    {
	interrupt( 33, 0, "\r\nError: File Not Found\n\0", 0, 0 );	
    }
    else
    {
	/* set the first bit of the filename to 0 */
	*file = 0;
	writeSector(directory, 2);

	/* beginning at the end of the filename (byte 6), loop through any */
	/* sectors that may belong to the file */
        for(j = 6; j < 31; ++j)
        {
	    /* remove the corresponding sectors from disk */
	    map[directory[index*32 + j]] = 0;
	    /* if we have reached an empty or deleted sector, return */
            if(*(file+j) == 0)
	    {
		cnt = 0;
		for( i = 0; i < 512; ++i )
		{
		    if( map[i] != 0x00 )
			++cnt;
		}

		writeSector(map, 1);
                return;	  
	    }
        }
	writeSector(map, 1);
        return;    
    }    
}

/*Searches for a free space in the directory, to put a new file*/
int findEntry(char* directory, char* name)
{
    int i = 0;
    int freeSpace = -1;
    
    for(; i < 16; ++i)
    {
        if( *(directory + i * 32) == '\0')
		 freeSpace = i;
		
        if(lex_compare(name, (directory + i * 32)))
		return 0; 
    }

    return freeSpace;
}

/*Writes a file from a buffer onto the disk. It updates the
  directory and map to indicate the file is present.*/
void writeFile(char* name, char* buffer, int numberOfSectors)
{
   char temp_buffer[512];
   char map[512];
   char directory[512];
   int file;
   int i,j,l,index;
	
   readSector( map, 1);
   readSector( directory, 2);
   
   file = findEntry( directory, name);
   if(!file)
   { 
       interrupt(33, 15, 1, 0,0);
       return;
   }

   if(file == -1)
   {
	interrupt(33, 15, 2, 0, 0);
	return;
   }

   for(i = 0; i < 6; ++i)
   {
          if(*(name + i) == '\0')
               break;

          *(directory + i + 32 * file) = *(name + i);
   }

   for(; i < 6; ++i)
      *(directory + i + 32 * file) = '\0';

   index = 0;
   j = 6;
   for(i = 0; i < numberOfSectors; ++i)
   {
       for(; index < 512; ++index)
           if(*(map + index) == '\0')
	       break;
       if(index == 511 && *(map + index) != '\0')
       {
           interrupt(33,15,2,0,0);
           return;
       }

       *(map + index) = 255;
       *(directory + (j++) + 32 * file) = index;
       for(l = 0; l < 512; l++)
	   temp_buffer[l] = buffer[l + 512 * i];

       writeSector(temp_buffer, index);
   }

   for(; i < 26; ++i, ++j)
       *(directory + j + 32 * file) = '\0';

   writeSector(directory, 2);
   writeSector(map,1);
}

/*This handler takes in arguments for ax, bx, cx, and dx
then it switches on the function defined by ax and executes 
the corresponding function. 0 prints a string, 1 reads a 
string in, 12 clears the screen, 13 writes and integer 
and 14 reads an integer. This is accomplished by calling the 
functions defined above.*/
void handleInterrupt21(int ax, int bx, int cx, int dx)
{
    switch(ax)
    {
    case 0:
        printString(bx);
        break;
        
    case 1:
        readString(bx);
        break;
    
    case 2:
	readSector(bx,cx);
	break;    
    
    case 3:
	readFile(bx,cx,dx);
	break;

    case 4:
	runProgram(bx,cx);
	break;

    case 5:
	stop();
	break;
    
    case 6:
        writeSector(bx,cx);
        break;

    case 7:
        deleteFile(bx);
        break;

    case 8:
        writeFile(bx,cx,dx);
        break;

    case 11:
        interrupt(25,0,0,0,0);
        break;

    case 12:
        clearScreen(bx,cx);
        break;
        
    case 13:
        writeInt(bx);
        break;
        
    case 14:
        readInt(bx);
        break;
    
    case 15:
	error(bx);
	break;
        
    default:
        printString("\rError: Interrupt 33: invalid argument for AX\n\0");
    }
}
