//
// Created by flo on 24.04.18.
//
#include "popen.h"


FILE* mypopen(char* command, char* type){
    int fd[4];
    pid_t *pid = NULL;
    char *befehl=malloc(1*sizeof(char*));
    int i=0;
    char puffer[PIPE_BUF];




    /* pipe:
     * fd[0] reading end of pipe
     * fd[1] writing end of pipe
     * Error -1 and errno is set
    */
    if(pipe(fd)<0){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    /* fork:
    * parent: >0
    * child == 0
    * Error -1 and errno is set
    */
    if((pid=fork())<0){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    //PARENT
    if(pid>0) {
        if((strcmp(type,"r"))==0){
            //read --> schreibe seite kann geschlossen werden
            close(fd[1]);
            //parent warten auf child
            wait(pid);

            //dupliziere pipe read der pipe auf stdin
            if(read(fd[0],puffer,PIPE_BUF)==-1){
                perror("read");
                exit(EXIT_FAILURE);
            }
            printf("%s",puffer);

            exit(0);

        }

    }
        //CHILD
    else if(pid==0){
        if((strcmp(type,"r"))==0) {
            close(fd[0]);
            //dupliziere pipe write in stdout, sprich alles was auf stdout geht, geht in die pipe
            if(dup2(fd[1],STDOUT_FILENO)==-1){
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            //ausführen des Kommandos
            if(execl("/bin/ls","ls","/home/ernstl/FH", NULL)==-1){
                perror("\nError EXECL");
                exit(EXIT_FAILURE);
            }
            printf("TEST");
            exit(0);
        }else if((strcmp(type,"w"))==0){
            printf("write");
            exit(0);
        }
    }

}
