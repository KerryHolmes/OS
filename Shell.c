#include "blackdos.h"

int lex(char*, char*);
int str_find(char*, char);

int main()
{   
    int position;
    int i, j;
    char input[300];
    char command[6];

    CLEAR;

    while(1)
    {
	PRINTS("blackdos~(_^>\0");
        SCANS(input);
	PRINTS("\r\n\0");	
	for(i = 0; i < 6; ++i)
	    *(command + i) = '\0';

        position = str_find(input, ' ', 0);
        if(position == -1)
	{
	    position = str_find(input, '\0', 0);
	}

	for(i = 0; i < position; ++i)
	    *(command + i) = *(input + i);

        if(lex(command, "boot\0"))
	    interrupt(33,11,0,0,0);

	if(lex(command, "cls\0"))
	{
	    interrupt(33,12,8,10,0);
	    continue;
	}

	if(lex(command, "copy\0"))
	{
	    int file1;
	    int file2;
	    char file1n[6];
	    char file2n[6];
	    char buffer[13312];
	    int size;

	    file1 = str_find(input, ' ', position+1);
	    if(file1  == -1)
		PRINTS("Error file 1 missing\r\n\0");
	    
	    file2 = str_find(input, ' ', file1+1);
	    if(file2  == -1)
	    {
		file2 = str_find(input, '\0', file1+1);
		if(file2 == -1)
                    PRINTS("Error file 2 missing\r\n\0");
	    }

	    for(i = 0; i < 6; ++i)
                *(file2n + i) = '\0';

	    for(i = 0; i < 6; ++i)
                *(file1n + i) = '\0';

	    for(j = 0, i = position + 1; i < file1; ++i, ++j)
                *(file1n + j) = *(input + i);

	    for(j = 0, i = file1 + 1; i < file2; ++i, ++j)
                *(file2n + j) = *(input + i);

	    interrupt(33,3,file1n,buffer,&size);
	    interrupt(33,8,file2n,buffer,size);
 
            continue;
	}
	if(lex(command, "del\0"))
        {
            int file;
	    char filename[6];
	    for(i = 0; i < 6; ++i)
	        *(filename + i) = '\0';
            file = str_find(input, '\0', position+1);
            if(file  == -1)
                PRINTS("Error file missing\r\n\0");
	    for(i = 0; i < file; ++i)
                *(filename + i) = *(input + position + i + 1);
	    PRINTS(filename);
	    interrupt(33,7,filename,0,0);
	    continue;
	}
	if(lex(command, "dir\0"))
            continue;	
	if(lex(command, "echo\0"))
            continue;
	if(lex(command, "help\0"))
            continue;
	if(lex(command, "run\0"))
            continue;
	if(lex(command, "tweet\0"))
            continue;
	if(lex(command, "type\0"))
            continue;

	PRINTS("Command not recognized. Type help to see commands\r\n\0");
    }
    END;
}

int lex(char* orig, char* comp)
{
    int i = 0;
    for(i = 0; i < 6; ++i)
    {
        if(*(orig + i) == 0 && *(comp + i) == 0)
            return 1;
        if(*(orig + i) != *(comp + i))
            return 0;
    }
    return 1;
}

int str_find(char* string, char target, int spot)
{
    int i = spot;
    while(*(string + i) != '\0')
    {
        if(*(string + i) == target)
            return i;
	i++;
    }
    if(*(string + i) == target)
        return i;
    return -1;
}