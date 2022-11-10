#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
//La fork sarebbe la funzione che crea un processo
//può avere valori che partono da 0 fino a inf
//dipende da quanti processi vengono creati.
//Se il valore è < 0 c'è stato un problema con la creazione
//Il valore 0 rappresenta il processo in esecuzione al momento
void orphan(void){
    pid_t p;
    
    p = fork();
    if(p == 0){
        printf("\nIo sono il figlio.\n\nPID:%d\nPPID:%d\nFORK_ID:%d\n",getpid(),getppid(),p);
        printf("\nSto aspettando che mio padre muoia.\n");
        sleep(60);
        printf("\nOra sono orfano.\n");
        printf("\nPID:%d\nPPID:%d\nFORK_ID:%d\n",getpid(),getppid(),p);
    }else if(p < 0){
        fprintf(stderr,"Qualcosa e' andato storto.\n");

    }else{
        printf("\nIo sono il padre.\n\nPID:%d\nPPID:%d\nFORK_ID:%d\n",getpid(),getppid(),p);
        exit(0);
    }
}
void zombie(void){
    pid_t p;
    p = fork();
    if(p==0){
        printf("\nIo sono il figlio.\n\nPID:%d\nPPID:%d\nFORK_ID:%d\n",getpid(),getppid(),p);
        exit(0);
    }else if(p < 0){
        printf("\nQUalcosa e' andato storto.\n");
        return;
    }else{
        printf("\nIo sono il padre.\n\nPID:%d\nPPID:%d\nFORK_ID:%d\n",getpid(),getppid(),p);
        sleep(60);
    }
}
int main(void){
    char c;
    while(c != 'q'){
        printf("\no)Orphan process.\nz)Zombie process.\nq)Quit.\nType choise: ");
        scanf(" %c",&c);
        switch(c){
            case 'o':
                orphan();
                break;
            case 'z':
                zombie();
                break;
            case 'q':
                printf("\nQuitting.\n");
                return 0;
            default:
                printf("\nScelta non valida, riprova.");
                break;
        }
    }
}