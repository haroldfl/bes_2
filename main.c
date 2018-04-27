#include "popen.h"






int main() {


    FILE* file;
    char path[PATH_MAX];

    mypopen("ls /home/flo","r");
    //execl("/bin/sh","sh","-c","ls /home/flo/c_prog -la",NULL);
    //


    //file=popen("nano bla.txt","w");
/*
    if(file==NULL){
        exit(EXIT_FAILURE);
    }
    while(fgets(path,PATH_MAX,file)!=NULL){
        printf("%s",path);
    }

*/

    return 0;

 }