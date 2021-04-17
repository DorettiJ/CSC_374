#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<wait.h>

const int	TEXT_LEN	= 256;

int		turn		= 0;

void		sigUsr1Handler	(int	sigNum
				)
{
  turn = 1;
}


void		sigUsr2Handler	(int	sigNum
				)
{
  turn = 2;
}


int		main		()
{
//**********Local Variables**********//
    int status;  //for child
    int childID; //for child
    int pid; // for child
    char text[TEXT_LEN];
//**********INSTALLS HANDLERS (2C.1)*****************//    
    struct sigaction	act;
    //  Installs memset data struct
    memset(&act, '\0', sizeof(act));
    // Sets flags
    act.sa_flags= SA_NOCLDSTOP | SA_RESTART;
    //  Install sigUsr1Handler() and handler for SIGUSR1
    act.sa_handler = sigUsr1Handler;
    sigaction(SIGUSR1,&act, NULL);
    //  Install sigUsr2Handler() and handler for SIGUSR2
    act.sa_handler = sigUsr2Handler;
    sigaction(SIGUSR2,&act, NULL);

//***********2C.2***********//
  printf("first: turn = %d\n",turn);

  while  (turn < 2)
  { 
    int	oldTurn	= turn;
//*********Makes child (2C.3)*********//
    //  YOUR CODE HERE
    //  Make a child process.
    snprintf(text,TEXT_LEN,"%d",turn);
    childID = fork();
    //  Have the child run "./second" and
    //  pass it the value of turn (as a string) on the command line
    if(childID == 0){
        
        execl("./second", "second", text);
    }
    while  (turn == oldTurn)
    {
      sleep(1);
    }
    printf("first: turn = %d\n",turn);
  }

//*********Waiting (2C.4)*********//
  //  YOUR CODE HERE
  //  Wait for both children to end
    childID = wait(&status);
    if  (WIFEXITED(status)){
        if (WEXITSTATUS(status) ==  EXIT_SUCCESS){ 
            printf("first: recieved child %d\n", childID);
        }
    }
    childID = wait(&status);
    if  (WIFEXITED(status)){
        if (WEXITSTATUS(status) ==  EXIT_SUCCESS){ 
            printf("first: recieved child %d\n", childID);
        }
    }

  return(EXIT_SUCCESS);
}