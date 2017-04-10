#include "blackdos.h"

int lex(char*, char*);
int str_find(char*, char);

int main()
{   
    int position;
    int i, j;
    char input[300];
    char command[6];

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
	{
	    CLEAR;
	    interrupt(33,11,0,0,0);
	}
	    
	if(lex(command, "cls\0"))
	{
	    CLEAR;
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
	    {
		PRINTS("Error file 1 missing\r\n\0");
	        continue;
	    }
	    file2 = str_find(input, ' ', file1+1);
	    if(file2  == -1)
	    {
		file2 = str_find(input, '\0', file1+1);
		if(file2 == -1)
	        {
                    PRINTS("Error file 2 missing\r\n\0");
		    continue;
		}
		if(*(input + file1 + 1) < 'a')
                {
                PRINTS("duplicate or invalid filename\r\n\0");
                continue;
                }
	    }

	    for(j = 0, i = position + 1; i < file1 && j < 6; ++i, ++j)
                *(file1n + j) = *(input + i);
	    for(; j < 6; ++j)
		*(file1n + j) = '\0';

	    for(j = 0, i = file1 + 1; i < file2 && j < 6; ++i, ++j)
                *(file2n + j) = *(input + i);
	    for(; j < 6; ++j)
                *(file2n + j) = '\0';

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
	    {
                PRINTS("Error file missing\r\n\0");
		continue;
	    }

	    if(*(input + position + 1) < 'a')
            {
                PRINTS("duplicate or invalid filename\r\n\0");
                continue;
            }

	    for(i = 0; i < file; ++i)
                *(filename + i) = *(input + position + i + 1);
	    interrupt(33,7,filename,0,0);
	    continue;
	}
	if(lex(command, "dir\0"))
	{
	    char directory[512], name[7];
	    int i, j, size, total = 0;
	    interrupt(33, 2, directory, 2, 0);
	    for(i = 0; i < 6; ++i)
		*(name + i) = '\0';

	    for(i = 0; i < 16; ++i)
	    {
		size = 0;
		if(*(directory + i * 32) == '\0')
		    continue;
		for(j = 6; j < 32; ++j)
                {
                    if(*(directory + j + i * 32) == '\0')
                        break;
                    size++;
                }
		
		total += size;

		if(*(directory + i * 32) > 'a')
		{
		    for(j = 0; j < 6; ++j)
			*(name + j) = *(directory + j + i * 32);
		    
		    PRINTS(name);
		    PRINTS("  Size: \0");
		    PRINTN(size);
		    PRINTS("\r\n\0");
		}
	    }
	
	    PRINTS("Total space used: \0");
	    PRINTN(total);
	    PRINTS("\r\n\0");

	    PRINTS("Total space available: \0");
	    PRINTN(256 - total);
	    PRINTS("\r\n\0");

            continue;	
	}
	if(lex(command, "echo\0"))
	{
	    /* check if everything after "echo " is empty */
	    for( i = 5; i < sizeof( input ); ++i )
	    {
		/* if something is not whitespace, the string is not empty */
		if( *( input + i ) != ' ' 
		    || *( input + i ) != '\0'
		    || *( input + i ) != '\n'
		    || *( input + i ) != '\t' )
		    break;
		/* otherwise, print a new line */
		else
		    PRINTS( "\r\n\0" );
	    }
	  
		/* print everything after "echo " */
		PRINTS( input + 5 );
		PRINTS( "\r\n\0" );
	    
            continue;
	}
	if(lex(command, "help\0"))
	{
	    PRINTS( "boot - reboot the system\r\n\0" );
	    PRINTS( "cls - clear screen\r\n\0" );
	    PRINTS( "copy <file1> <file2> - copy file1 into file 2\r\n\0" );
	    PRINTS( "del <filename> - delete a file\r\n\0" );
	    PRINTS( "dir - list disk directory contents\r\n\0" );
	    PRINTS( "echo <comment> display a comment\r\n\0" );
	    PRINTS( "help - display this prompt\r\n\0" );
	    PRINTS( "run <filename> - run executable file\r\n\0" );
	    PRINTS( "tweet <filename> create a text file\r\n\0" );
	    PRINTS( "type <filename> print out the contents of a file\r\n\0" );
            continue;
	}
	if(lex(command, "run\0"))
	{
	    int file;
	    char filename[6];
	    for( i = 0; i < 6; ++i )
		*(filename + i) = '\0';
	    file = str_find( input, '\0', position + 1 );
	    if( file == -1 )
		PRINTS( "Error: file not found\r\n\0" );
               
	    else
	    {
		for( i = 0; i < file; ++i )
		    *(filename + i) = *(input + position + i + 1);
		RUN( filename, 4 );
	    }
            continue;
	}
	if(lex(command, "tweet\0"))
	{
	    char buffer[140], filename[6];
	    int file;

            for(i = 0; i < 6; ++i)
                *(filename + i) = '\0';

            file = str_find(input, '\0', position+1);
            if(file  == -1)
	    {
                PRINTS("Error file missing\r\n\0");
		continue;
	    }

	    if(*(input + position + 1) < 'a')
	    {
		PRINTS("duplicate or invalid filename\r\n\0");
		continue;
	    }

            for(i = 0; i < file; ++i)
                *(filename + i) = *(input + position + i + 1);

	    PRINTS("Input a string less than 140 characters long\r\n\0");
	    SCANS(buffer);
	    interrupt(33, 8, filename, buffer, 1);
	    PRINTS("\r\n\0");

            continue;
	}
	if(lex(command, "type\0"))
	{
	    char buffer[512], filename[6];
	    int file, filesize;
	    
            for(i = 0; i < 6; ++i)
                *(filename + i) = '\0';
	    for( i = 0; i < 512; ++i )
		*( buffer + i ) = '\0';

            file = str_find(input, '\0', position+1);
            if(file  == -1)
	    {
                PRINTS("Error file missing\r\n\0");
		continue;
	    }
	    else
	    {
		for( i = 0; i < file; ++i )
		    *(filename + i) = *(input + position + i + 1);
		interrupt( 33, 3, filename, buffer, filesize );
		interrupt( 33, 0, "\r\n\0", 0, 0 );
		interrupt( 33, 0, buffer, 0, 0 );
		interrupt( 33, 0, "\r\n\0", 0, 0 );
	    }
	    

            continue;
	}

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
