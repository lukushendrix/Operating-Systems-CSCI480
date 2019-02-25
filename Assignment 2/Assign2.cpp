/*
Assign 2
CSCI 480
Name: Lukus Hendrix
ZID: Z1761354
SECTION: 2
Due Date: 9/18/2018
T.A: Jeremy Crumpton

   Program:   This program is written in C/C++ and uses 2 forks to branch the
	parent process into a child and grandchild process. Once forked, the processes
	will close and open the appropriate pipes for each process. The parent function,
	child funtion, and grandchild function will be called with a read and write parameter.
	The functions will then use arithmetic on an int to and turn it back to a string.
	The output will be the parent writing to pipe A and reading pipe C, the child writing
	to pipe B and reading from pipe A, and the grandchild writing to C and reaing pipe B.

*/


#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

using namespace std;


void pWork(int, int);
void gWork(int, int);
void cWork(int, int);


/****************************************************************

   FUNCTION:   Main Function

   ARGUMENTS:  int argc, char** argv

   RETURNS:    0

   Notes:     This function closes and opens the appropriate pipes
	for the parent, child, and grandchild. Then it calls the
	appropriate functions either pWork, cWork, or gWork, and
	waits for the child processes. After the functions are
	opened the appropriate read and write pipes are closed.

****************************************************************/

int main(int argc, char** argv )
{
setbuf(stdout, NULL); /* Try to prevent buffering of stdout. */

int pipeA[2]; //creating pipe subscripts
int pipeB[2];
int pipeC[2];

 if (pipe(pipeA) == -1) {  //check for errors if -1
        perror("pipe #1 error");
        exit(-5);
	}
 if (pipe(pipeB) == -1) {
        perror("pipe #2 error");
        exit(-5);
        }
 if (pipe(pipeC) == -1) {
        perror("pipe #3 error");
        exit(-5);
        }

pid_t fork1 = fork();  // 1st fork Child process

	if(fork1 < 0)
        {
        perror("The first fork failed. ");
        exit(-1);
        }

        else if(fork1 == 0) 
        {
	        pid_t fork2= fork(); //2nd fork Grandchild Process

             	if(fork2 < 0) //GrandChild
	        	{
        		perror("The first fork failed. ");
        		exit(-1);
        		}
   		     else if (fork2 == 0) //Grandchild
        		{
      			close(pipeB[1]);
        		close(pipeC[0]);
        		gWork(pipeB[0],pipeC[1]); //Grandchild Function
			wait(0);                /* Wait for child */
  		        close(pipeC[1]);
		        close(pipeB[0]);
		        exit(0);
        		}
				else // Child Process
				{
			  	close(pipeA[1]);
      			  	close(pipeB[0]);
   			  	cWork(pipeA[0], pipeB[1]); //Child Function
      			  	wait(0);                /* Wait for child */
       			 	close(pipeA[0]);
      			  	close(pipeB[1]);
				}
		}
	 else // Parent Process
	{
	 close(pipeC[1]);
        close(pipeA[0]);
        pWork(pipeC[0], pipeA[1]);  // Parent function
        wait(0);                /* Wait for child */
        close(pipeC[0]);
        close(pipeA[1]);

        exit(0);
        }
        return 0;
 }


/****************************************************************

   FUNCTION:   void pWork(int pRead, int pWrite)

   ARGUMENTS: int pRead, int pWrite

   RETURNS:    Nothing

   Notes:     This function creates a char string buffer of 10,
	a char string value of 10 intitiated to' 1'. This function
	starts by printing and writing the parent value of 1 to
	pipe A. It then loops while m is less then 999999999, then loop
	while pRead is reading pipe C one byte at a time greater
	than zero and not null, iterate one index. Store the buff
	value into the buffer array. Clear integer i back to zero.
	This uses atoi to get the characters in buffer and converts
	each into an integer. After the arithmitic the integers get
	converted back into a string. It gets printed on the screen
	and writes to pipe A.

*******************************************************************/

void pWork(int pRead, int pWrite)
{
char buffer[10];
char value[10] = {'1'};
char buff;
int  m = 1;
int i = 0;

cerr<< "\nThis is the parent, now writing "<< value << " to pipe A" <<endl;
write(pWrite, value , strlen(value)+1);


while(m <= 99999999)
{

	while(read(pRead, &buff, 1) > 0 && buff != '\0')
	{
	buffer[i] = buff; //iterate to the next index
	i++;
	}

	buffer[i] = buff; //store next index in buffer
	i = 0;		// clear int back to zero

	m= atoi(buffer); // char to int
	 m = 4 * m + 3;
	sprintf(value,"%d", m);//int to char inside value[10]
	 cerr<< "Parent:        Value = "<< value << endl;
	write(pWrite, value, strlen(value)+1); //write the string lenth of value to pipe A.
	}

}


/****************************************************************

   FUNCTION:   void cWork(int cRead, int cWrite)

   ARGUMENTS: int cRead, int cWrite

   RETURNS:    Nothing

   Notes:     This function creates a char string buffer of 10,
        a char string value of 10 intitiated to' 1'. This function
        starts by looping while m is less then 999999999, then loop
        while cRead is reading pipe A one byte at a time greater
        than zero and not null, iterate one index. Store the buff
        value into the buffer array. Clear integer i back to zero.
        This uses atoi to get the characters in buffer and converts
        each into an integer. After the arithmitic the integers get
        converted back into a string. It gets printed on the screen
        and writes to pipe B.

*******************************************************************/

void cWork(int cRead, int cWrite)
{
char buffer[10];
char value[10] = {'1'};
char buff;
int  m = 1;
int i = 0;

while(m <= 99999999)
{

        while(read(cRead, &buff, 1) > 0 && buff != '\0')
        {
        buffer[i] = buff;
        i++;
        }

        buffer[i] = buff;
        i = 0;

        m= atoi(buffer); // char to int
        m = 4 * m + 3;
	sprintf(value,"%d", m);//int to char inside value[10]
	cerr<< "Child:         Value = "<< value << endl;
        write(cWrite, value, strlen(value)+1); //write the string lenth of value to pipe B.
	}

}


/****************************************************************

   FUNCTION:   void gWork(int gRead, int gWrite)

   ARGUMENTS: int gRead, int gWrite

   RETURNS:    Nothing

   Notes:     This function creates a char string buffer of 10,
        a char string value of 10 intitiated to' 1'. This function
        starts by looping while m is less then 999999999, then loop
        while gRead is reading pipe B one byte at a time greater
        than zero and not null, iterate one index. Store the buff
        value into the buffer array. Clear integer i back to zero.
        This uses atoi to get the characters in buffer and convert
        them into integers. After the arithmitic the integers get
        converted back into a string. It gets printed on the screen
        and writes to pipe C.

*******************************************************************/



void gWork(int gRead, int gWrite)
{
char buffer[10];
char value[10] = {'1'};
char buff;
int  m = 1;
int i = 0;

while(m <= 99999999)
{

        while(read(gRead, &buff, 1) > 0 && buff != '\0')
        {
        buffer[i] = buff;
        i++;
        }

        buffer[i] = buff;
        i = 0;

        m= atoi(buffer); // char to int
	m = 4 * m + 3;
        sprintf(value,"%d", m);  //int to char inside value[10]
	cerr<< "GrandChild:    Value = "<< value <<endl;
        write(gWrite, value, strlen(value)+1); //write the string lenth of value to pipe C.
	}

}





