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
/* Signed:____Kerry Holmes_____________________ Date:___02/04/2017___     */
/*                                                                        */
/*                                                                        */
/* 3460:4/526 BlackDOS kernel, Version 1.03, Spring 2017.                 */

void handleInterrupt21(int,int,int,int);

void main()
{
    char line[80];
    int x;
    
    makeInterrupt21();
    interrupt(33,12,8,10,0);
    
    interrupt(33,0,"___.   .__                 __       .___           \r\n\0",0,0);
    interrupt(33,0,"\\_ |__ |  | _____    ____ |  | __ __| _/___  ______\r\n\0",0,0);
    interrupt(33,0," | __ \\|  | \\__  \\ _/ ___\\|  |/ // __ |/ _ \\/  ___/\r\n\0",0,0);
    interrupt(33,0," | \\_\\ \\  |__/ /\\ \\\\  \\___|    </ /_/ ( <_> )___ \\ \r\n\0",0,0);
    interrupt(33,0," |___  /____(____  /\\___  >__|_ \\____ |\\___/____  >\r\n\0",0,0);
    interrupt(33,0,"     \\/          \\/     \\/     \\/    \\/         \\/ \r\n\0",0,0);
    interrupt(33,0," V. 1.03, C. 2017. Based on a project by M. Black. \r\n\0",0,0);
    interrupt(33,0," Author(s): Kerry Holmes\r\n\r\n\0",0,0);
    
    interrupt(33,0,"Hola mondo.\r\n\0",0,0);
    interrupt(33,0,"Enter a line: \0",0,0);
    interrupt(33,1,line,0,0);
    interrupt(33,0,"\r\nYou typed: \0",0,0);
    interrupt(33,0,line,0,0);
    interrupt(33,0,"\r\n\0",0,0);
    interrupt(33,0,"Enter a number: \0",0,0);
    interrupt(33,14,&x,0,0);
    interrupt(33,0,"\r\n\0",0,0);
    interrupt(33,0,"You entered \0",0,0);
    interrupt(33,13,x,0,0);
    interrupt(33,0,"\r\n\0",0,0);
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
    while(1)
    {
        char raw = interrupt(22,0,0,0,0);
        if(raw == 13)
        {
            *(c+i) = '\0';
            return;
        }
        
        else if(raw == 8)
        {
            if(i > 0)
                i--;
        }
        
        else
        {
            *(c+i) = raw;
            i++;
        }
        interrupt(16, 14*256+raw,0,0,0);
    }
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
        
    case 12:
        clearScreen(bx,cx);
        break;
        
    case 13:
        writeInt(bx);
        break;
        
    case 14:
        readInt(bx);
        break;
        
    default:
        printString(" Error you did not enter a correct value for AX");
    }
}
