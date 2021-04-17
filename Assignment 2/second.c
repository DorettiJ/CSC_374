#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<wait.h>

const int	TEXT_LEN	= 256;

int		turn;
//**********2 Sig Handlers(3a)**********//
void		sigUsr1Handler	(int	sigNum){
  turn = 1;
}

void		sigUsr2Handler	(int	sigNum){
  turn = 2;
}


//3B start

int		main		(int argc, char *argv[]){
//**********Local Variables**********//
int status;  //for child
pid_t childID; //for child
char text[TEXT_LEN];
//*********3B.1*********//
    if(argv[1] == 0){
        fprintf(stderr,"Missing command line argument for second\n");
        exit(EXIT_FAILURE);
    }else{
        turn = atoi(argv[1]); 
    }
    int	oldTurn	= turn;
//*********Install Sig Handlers (3B.2)*********//
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

//*********Current val of turn (3B.3)*********//
    printf("second: turn = %d\n",turn);

//*********Making one child(3B.4)*********//
 
    snprintf(text,TEXT_LEN,"%d",turn);
    childID = fork();
    
    if  (childID == 0){
        execl("./third", "third", text);
    }
//*********Waiting(3B.5)*********//   
    while  (turn == oldTurn)
    {
      sleep(1);
    }
//*********Current val of turn (3B.6)*********//
    printf("second: turn = %d\n",turn);
//*********Checks if turn == 1 (3B.7)*********// 
    if (turn == 1){ 
        kill(getppid(), SIGUSR1);
        printf("second: sending %d to parent\n", SIGUSR1);
    }else{
        kill(getppid(), SIGUSR2);
        printf("second: sending %d to parent\n", SIGUSR2);
    }
    printf("second: recieved child %d\n", childID);
//*********Waiting for child to finish(3B.8)*********//
    wait(&status);
//**********Finish(3B.9)**********//
    return(EXIT_SUCCESS);
}