#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<wait.h>

int     turn;

int		main		(int argc, char *argv[]){
    
//*********4B*********//
    if(argv[1] == 0){
        fprintf(stderr,"Missing command line argument for second\n");
        exit(EXIT_FAILURE);
    }else{
        turn = atoi(argv[1]); 
    }
//**********4C**********//
    printf("third: turn = %s\n", argv[1]);
//*********Checks if turn == 0 (4D)*********//    
    if (turn == 0){ 
        kill(getppid(), SIGUSR1);
        printf("third: sending %d to parent\n", SIGUSR1);
    }else{
        kill(getppid(), SIGUSR2);
        printf("third: sending %d to parent\n", SIGUSR2);
    }
//**********4E**********//
    return(EXIT_SUCCESS);
}