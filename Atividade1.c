#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int cont = 0;
int t;

void handlerAlarm(int signum){

    printf("Demorou muito!");
    exit(1);    
    
}

void handlerINT(int signum){

    signal(SIGALRM,handlerAlarm);

    cont ++;    

    if (cont == 3){
        printf("Voce realmete deseja sair? 1/0 \n");
        alarm(5);
        scanf("%i",&t);

        if(t == 1){
            exit(1);

        }else if(t == 0){
            cont = 0;
            t=0;
            alarm(0);
            return;
        }

    }
    
}

void handlerSTP(int signum){
    
    printf("Voce apertou Control+c %i vezes!\n",cont);


}

void main(){

    signal(SIGINT, handlerINT);     
    signal(SIGTSTP, handlerSTP);

    while(1){   
        pause();
    }

}
