/*
Assign 3
CSCI 480
Name: Lukus Hendrix
ZID: Z1761354
SECTION: 2
Due Date: 9/29/2018
T.A: Jeremy Crumpton

   Program:   This program  implements a microshell and will
	execute commands. It can pipe two commands together
	with the "||" argument.This program is written in C/C++.
	This is on a countinuous loop until exited with quit or q.
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


/****************************************************************

   FUNCTION:   Main Function

   ARGUMENTS:  void

   RETURNS:    0

     Notes:     This function is the main function. It is in a continous
	while loop until exited with "quit" or 'q'. It starts by
	reading the buffer which will get the user input. Then
	it uses strtok to find the delimiter "||" and points to each side
	of the command with an array. Then that command array gets
	seperated with two additional pointer arrays named spaceL and
	spaceR. Which then seperates the spaces out of the commands.
	After that, pipeA gets opened and checks for errors. If there
	are no errors, the first fork gets called and a child process is
	born. This child process then closes std output and uses dup
	to duplicate pipeA's write, then closes the read and write pipe
	of pipeA. It then uses execvp to call the left hand commands.
	It then checks if there is a right hand command and if there is
	it calls a second fork and the second child is born. The second
	child is used for the right side of the command.  This then
	proceeds to close std read input and uses dup to duplicate pipeA's
	read pipe. It then closes pipeA's read and write pipes.
	Once the pipes are closed it uses execvp to command the
	right side of the delimiter. It then proceeds to close
	the read and write of pipeA again and waits for the child
	processes to finish.

****************************************************************/

int main(void)
{
setbuf(stdout, NULL); /* Try to prevent buffering of stdout. */

int pipeA[2];

char buf[1024];

pid_t fork2;
char *command[1024];
char *token;
char *spaceL[35] = {NULL};
char *spaceR[35] = {NULL};
int status;
string exiting;



printf("480shell> ");


while(fgets(buf,1024,stdin) != NULL)// while loop that gets user input
 {

	exiting = buf;

	if(buf[0] == 'q' ) // ways to quit the program
	exit(0);
	else if(exiting  == "quit")
	exit(0);

	buf[strlen(buf)-1] = 0; // starts from 0 and -1 of 1024 bytes

	int num = 0;

        command[0] = strtok(buf,"||"); // pointer to first side of argument

	command[1] = strtok(NULL, "||"); // pointer to secind side of argument

	// These next lines separates each side of commands  by spaces

	num = 0;
	spaceL[0] = strtok(command[0], " "); // uses a space delimiter
	while((token = strtok(NULL, " ")) != NULL) // Loop through array if not NULL
           {
	    num++;
            spaceL[num] = token; // iterates to next token
           }
		spaceL[num+1] = NULL; // end of token gets replaced by NULL


	if(command[1] != NULL) // if right side is not null
	{
	num = 0;
        spaceR[0] = strtok(command[1], " "); // uses a space delimiter
        while((token = strtok(NULL, " ")) != NULL) // Loop through array if not NULL
           {
            num++;
            spaceR[num] = token; // iterates to next token
           }
		spaceR[num+1] = NULL;// end of token gets replaced by NULL
	}

	if(spaceR != NULL) // if right side of command is not NULL
	{
	 if (pipe(pipeA) == -1) {  //check pipe for errors if -1

	 perror("pipe #1 error");
         exit(-5);
	 }
        }

	pid_t fork1  = fork(); // First child process created

	if(fork1 < 0)// checks child process for errors
	{
	perror("Fork Error");
	exit(-5);
	}

	else if (fork1  == 0) // if there is a child process
		{
			if(spaceR[0] != NULL) // checks for right side command
			{
			close(1);//closes the std output
			dup(pipeA[1]); // duplicates output from pipe output.
			close(pipeA[1]); // closes pipe output
			close(pipeA[0]); //closes pipe input
			}

		execvp(spaceL[0],spaceL); // execute left side command
		}

		else
		{
			if(spaceR[0] != NULL) // checks right side of command
			{
			fork2 = fork(); // forks second child process

			 if(fork2 < 0){ //checks for errors
                         perror("2nd fork failed");
                         exit(-1);
                         }

			 if(fork2 == 0) // if there is a second  child process
			 {
			 close(0); //closes the std input
			 dup(pipeA[0]); // duplicates input from pipe input.
			 close(pipeA[0]); // closes pipe output
			 close(pipeA[1]); // closes pipe input

			 if( execvp(spaceR[0], spaceR) < 0) // checks for command errors
			 {
			  perror("exevp failed");
			 }

			 execvp(spaceR[0], spaceR); // execute right side of command

			  }
			}

			 if(spaceR != NULL) // if pointer is on NULL, its at the end
			 {
			 close(pipeA[1]); // close pipeA output
			 close(pipeA[0]); // close pipeA input
			 }
		}

		if(spaceR[0] != NULL)
		{
		wait(&fork2); // waits for second process
		waitpid(fork1, &status,0); // waits for first process
		}

		printf("480shell> ");
	}

return 0; // returns 0
}

