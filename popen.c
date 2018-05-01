//
// Created by flo on 24.04.18.
//

#include "popen.h"

static pid_t child_pid = -1;
static int limit = 0;
static FILE* openfile = NULL;


FILE* mypopen(char* command, char* type){
    int fd[2];
    FILE* rfile = NULL;
    pid_t *pid = NULL;
    int i=0;
    char puffer[PIPE_BUF]; //PIPE_BUF max größe des buffers

    /* pipe:
     * fd[0] reading end of pipe
     * fd[1] writing end of pipe
     * Error -1 and errno is set
    */
    if(pipe(fd)<0){
        return NULL;
    }
    /* fork:
    * parent: >0
    * child == 0
    * Error -1 and errno is set
    */
    if((pid=fork())<0){
        close(fd[0]);
        close(fd[1]);
        return NULL;
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
            /*if(read(fd[0],puffer,PIPE_BUF)==-1){
                perror("read");
                exit(EXIT_FAILURE);
            }
            printf("%s",puffer);*/
            //############ --------------------------------------------- ##############

            rfile = fdopen(fd[0],type);
            if(rfile == NULL){
                close(fd[0]);
                return NULL;
            }

            //exit(0);
        }
        else if((strcmp(type,"w"))==0){
            //read --> schreibe seite kann geschlossen werden
            close(fd[0]);
            //parent warten auf child
            //wait(pid);

            //############ --------------------------------------------- ##############
            //############ zum Testzweck, Ausgabe der eingelesenen Datei ##############
            //dupliziere pipe read der pipe auf stdin
            /*if(read(fd[1],puffer,PIPE_BUF)==-1){
                perror("read");
                exit(EXIT_FAILURE);
            }
            printf("%s",puffer);*/
            //############ --------------------------------------------- ##############

            rfile = fdopen(fd[1],type);
            if(rfile == NULL){
                close(fd[1]);
                return NULL;
            }
            //exit(0);
        }

        openfile = rfile;
        return rfile;

    }
        //CHILD
    else if(pid==0){
        child_pid = getpid();
        if((strcmp(type,"r"))==0) {
            close(fd[0]);
            //dupliziere pipe write in stdout, sprich alles was auf stdout geht, geht in die pipe
            if(dup2(fd[1],STDOUT_FILENO)==-1){
                close(fd[1]);
                exit(EXIT_FAILURE);
            }
        }
        else if((strcmp(type,"w"))==0) {
            close(fd[1]);
            //dupliziere pipe write in stdout, sprich alles was auf stdout geht, geht in die pipe
            if(dup2(fd[0],STDIN_FILENO)==-1){
                close(fd[0]);
                exit(EXIT_FAILURE);
            }
        }
        if(execl("/bin/sh","sh","-c",command,(char *)NULL)==-1){
            printf("#execl");
            return NULL;
        }
        if((strcmp(type,"r"))==0){
            close(STDOUT_FILENO);
        }
        else{
            close(STDIN_FILENO);
        }
        //exit(0);
    }
    return NULL;
}

int mypclose(FILE *stream){
    if(stream == NULL){
        errno = EINVAL;
        return -1;
    }
    //es dürfen nur files geschlossen werden die auch mit popen geöffnet wurden
    if(openfile != stream){
        errno = EINVAL;
        return - 1;
    }
    fclose(stream);
    if(waitpid(child_pid, NULL,0)==-1){
        return -1;
    }
    exit(EXIT_SUCCESS);
}