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
        printf("There is no argument. USAGE: %s fname_for_exec [-n]\nKeys:\n\t-n\tno fork\n", argv[0]);
        return 0;
    }
    if (argc > 2 && !strcmp(argv[2],"-n")){
        printf("Child process:\nPID:\t%d\nPPID:\t%d\nPGID:\t%d\n",getpid(),getppid(),getpgrp());
        printf("Child argv: ");
        print_argv(argc, argv);
        puts("Environment:");
        print_env();
        return 0;
    }
    pid_t child = fork();
    char *arg[] = {argv[1],argv[1],"-n",0};

    switch (child) {
    case -1:
        print_error(errno);
        return 1;
    case 0:
        //printf("Child process:\nPID:\t%d\nPPID:\t%d\nPGID:\t%d\n",getpid(),getppid(),getpgrp());
        execvp(arg[0],arg);
        puts("Never printed");
    default:
        printf("Parent process:\nPID:\t%d\nPPID:\t%d\nPGID:\t%d\n",getpid(),getppid(),getpgrp());
        printf("Parent argv: ");
        print_argv(argc, argv);
        puts("Environment:");
        print_env();
        //wait();
        break;
    }
    return 0;
}
