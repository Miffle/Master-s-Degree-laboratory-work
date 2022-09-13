#include "/root/Desktop/AOS/lab1/no1.h"

int main()
{
    //printf("Hello World!\n");
    pid_t child = fork();

    switch (child) {
    case -1:
        print_error(errno);
        return 1;
    case 0:
        printf("Child process:\nPID:\t%d\nPPID:\t%d\nPGID:\t%d\n",getpid(),getppid(),getpgrp());
        exit(0);
    default:
        printf("Parent process:\nPID:\t%d\nPPID:\t%d\nPGID:\t%d\n",getpid(),getppid(),getpgrp());
        wait();
        break;
    }
    return 0;
}
