#include "/root/Desktop/AOS/lab1/no1.h"
#include <signal.h>

struct sigaction new, old;
sigset_t mask;

int main()
{
    sigemptyset(&mask);
    new.sa_handler = SIG_IGN;
    new.sa_mask = mask;
    new.sa_flags = 0;

    if (sigaction(SIGCHLD,&new,&old) == -1){
        print_error(errno);
        return 1;
    }

    pid_t child = fork();
    if (child == -1){
        print_error(errno);
        return 1;
    }
    if (!child){
        puts("Child process. Sleep 2 seconds and exit.");
        sleep(2);
        exit(0);
    }
    puts("Parent process. Sleep 10 seconds and awaiting signal.");
    sleep(10);
    pause();
    return 0;
}
