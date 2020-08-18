#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

void collatz (int i){
    pid_t child_pid, wpid;
    int status = 0;

    if ((child_pid = fork()) == 0){
        while (i != 1){
            printf ("%d   ", i);

            if (i % 2 == 0){
                i = i/2;
            } else{
                i = i*3 + 1;
            }
        }

        printf ("1\n");
        exit (1);
    }
    
    while ((wpid = wait(&status)) > 0){
        printf ("Processo encerrado.\n");
    }
}

int main (){
    int n;
    char num[10];

    while (1){
        printf ("N= ");
        scanf ("%s", num);

        if (strcmp (num, "q") == 0) exit (0);

        n = atoi (num);

        if (n > 2){
            collatz (n);
        } else{
            printf ("Forneca um número valido.\n");
        }
    }

    return 0;

}