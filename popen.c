//
// Created by flo on 24.04.18.
//
#include "popen.h"

pid_t child_pid;

FILE* mypopen(char* command, char* type){
    int fd[4];
    FILE* rfile = NULL;
    pid_t *pid = NULL;
    char *befehl=malloc(1*sizeof(char*));
    int i=0;
    char puffer[PIPE_BUF]; //PIPE_BUF max größe des buffers

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

            //############ --------------------------------------------- ##############
            //############ zum Testzweck, Ausgabe der eingelesenen Datei ##############
            //dupliziere pipe read der pipe auf stdin
            if(read(fd[0],puffer,PIPE_BUF)==-1){
                perror("read");
                exit(EXIT_FAILURE);
            }
            printf("%s",puffer);
            //############ --------------------------------------------- ##############

            rfile = fdopen(fd[0],"r");
            if(rfile == NULL){
                perror("fdopen");
                exit(EXIT_FAILURE);
            }
            return rfile;

            exit(0);
        }
        else if((strcmp(type,"w"))==0){
            //read --> schreibe seite kann geschlossen werden
            close(fd[0]);
            //parent warten auf child
            wait(pid);

            //############ --------------------------------------------- ##############
            //############ zum Testzweck, Ausgabe der eingelesenen Datei ##############
            //dupliziere pipe read der pipe auf stdin
            if(read(fd[1],puffer,PIPE_BUF)==-1){
                perror("read");
                exit(EXIT_FAILURE);
            }
            printf("%s",puffer);
            //############ --------------------------------------------- ##############

            rfile = fdopen(fd[1],"w");
            if(rfile == NULL){
                perror("fdopen");
                exit(EXIT_FAILURE);
            }
            return rfile;

            exit(0);
        }

    }
        //CHILD
    else if(pid==0){
        child_pid = getpid();
        if((strcmp(type,"r"))==0) {
            close(fd[0]);
            //dupliziere pipe write in stdout, sprich alles was auf stdout geht, geht in die pipe
            if(dup2(fd[1],STDOUT_FILENO)==-1){
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            //ausführen des Kommandos
            if(execl("/bin/ls",command, NULL)==-1){
                perror("\nError EXECL");
                exit(EXIT_FAILURE);
            }
            printf("TEST");
            exit(0);
        }else if((strcmp(type,"w"))==0) {
            close(fd[1]);
            //dupliziere pipe write in stdout, sprich alles was auf stdout geht, geht in die pipe
            if(dup2(fd[0],STDIN_FILENO)==-1){
                perror("dup2");
                exit(EXIT_FAILURE);
            }
            //ausführen des Kommandos
            if(execl("/bin/sh","sh","-c",command,NULL==-1)){
                perror("\nError EXECL");
                exit(EXIT_FAILURE);
            }
            printf("TEST");
            exit(0);
        }
    }

}

int mypclose(FILE *stream){
    if(stream == NULL){
        //errno = EINVAL;
        return -1;
    }
    if(waitpid(child_pid, NULL,0)==-1){
        return -1;
    }
    fclose(stream);
    exit(EXIT_SUCCESS);
}