#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>

extern char **environ;

void print_env(){
    if (environ)
        for (size_t i = 0;environ[i] != NULL;i++){
            puts(environ[i]);
        }
}

void print_argv(int argc, char *argv[]){
    if (argv){
        for (int i = 0; i < argc; i++)
            printf("%s ", argv[i]);
        puts("");
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2){
        printf("There is no argument. USAGE: %s fname_for_exec\n", argv[0]);
        return 0;
    }
    char buf[256] = "\0";
    pid_t child = fork();
    char **arg = (char **)malloc(sizeof(char *)*argc);

    switch (child) {
    case -1:
        print_error(errno);
        return 1;
    case 0:
        //printf("Child process:\nPID:\t%d\nPPID:\t%d\nPGID:\t%d\n",getpid(),getppid(),getpgrp());
        for (int i = 0; i < argc; i++){
            arg[i] = argv[i + 1];
        }
        puts("Fork-exec");
        execvp(arg[0],arg);
        puts("Never printed");
    default:
        for (int i = 1; i < argc; i++){
            strcat(buf," ");
            strcat(buf,argv[i]);
        }
        sleep(1);
        puts("System");
        system(buf);
        //wait();
        break;
    }
    return 0;
}
