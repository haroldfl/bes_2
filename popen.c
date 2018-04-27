//
// Created by flo on 24.04.18.
//
#include "popen.h"


FILE* mypopen(char* command, char* type){
    int fd[4];
    pid_t pid;
    char *befehl=malloc(1*sizeof(char*));
    int i=0;






    if(pipe(fd)<0){
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    if((pid=fork())<0){
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if(pid==0){
        if((strcmp(type,"r"))==0) {
            close(fd[0]);
           // dup2(,STDOUT_FILENO);
        }else if((strcmp(type,"w"))==0){
            close(fd[1]);

        }
    }

}
