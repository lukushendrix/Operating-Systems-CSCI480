/*
Assign 1
CSCI 480
Name: Lukus Hendrix
ZID: Z1761354
SECTION: 2
Due Date: 9/7/2018
T.A: Jeremy Crumpton

   Program:   This program is written in C/C++ and uses forks to branch the
	processes into the child grandchild and the parents of those children.
 	The PIDs of the processes are called to show each child and parant.

   ARGUMENTS:  Nothing

   RETURNS:    0
*/


#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


pid_t getpid(void);
pid_t getppid(void);
pid_t fork(void);
pid_t getOriginal();
pid_t getGrandchild();
pid_t getChild();

/****************************************************************

   FUNCTION:   Main Function - This calls the getChild and
	getOriginal functions.

   ARGUMENTS:  int argc, char** argv

   RETURNS:    0

****************************************************************/

int main(int argc, char** argv )
{
setbuf(stdout, NULL); /* Try to prevent buffering of stdout. */

 getOriginal();

printf("Now we have the first fork\n");

 getChild();

return 0;
}


/****************************************************************

   FUNCTION:   pid_t getChild()

   ARGUMENTS:  Nothing

   RETURNS:    0

   NOTES:      This function forks the child process off of the
	parent process and prints the parent and child PIDs. Then
	the child calls the getGrandchild function and waits until
	the Grandchild exits. Then then the parent sleeps for 2
	seconds and waits for the child to exit and uses a system
	call to call "ps" and then exits.

****************************************************************/

 pid_t getChild()
{
pid_t forkPID = fork();


	 if(forkPID < 0)
        {
        perror("The first fork failed. ");
        exit(-1);
        }

        if(forkPID == 0)
        {
         printf("Im the child, my PID is %d, and my parent is %d\n",
         getpid(), getppid());
	 printf("Now we have the second fork\n");
	 getGrandchild();
	 wait(0);
	 printf("Im the child, about to exit\n");

	 exit(0);
        }

        else
	{
         printf("Im the parent, my PID is %d, and my parent is %d\n",
         getpid(), getppid());
	 printf("I am the parent, about to call ps.\n");
	 sleep(2);
	 system("ps");
	 wait(0);
	 printf("I am the parent, about to exit.\n");
	}
	return 0;
}

/****************************************************************

   FUNCTION:   pid_t getOriginal()

   ARGUMENTS:  Nothing

   RETURNS:    0

   NOTES:      This function just calls the original getpid and
	getppid functions.

****************************************************************/
pid_t getOriginal()
{
	  printf("Im the original process, my PID is %d, and my parent is %d\n",
          getpid(), getppid());
return 0;
}


/****************************************************************

   FUNCTION:   pid_t getGrandchild()

   ARGUMENTS:  Nothing

   RETURNS:    0

   NOTES:     This funtion is the Grandchild function. It is called inside
	the child function. It uses a second fork to continue branching
	processes. It prints it's pid and it's parents pid and exits.

****************************************************************/

pid_t getGrandchild()
{
pid_t forkPID = fork();

 if(forkPID < 0)
        {
        perror("The second fork failed.");
        exit(-1);
        }

 if(forkPID == 0)
        {
         printf("Im the grandchild, my PID is %d, and my parent is %d\n",
         getpid(), getppid());

	 printf("I am the grandchild, im about to exit.\n");

	exit(0);
	}
	return 0;

}
