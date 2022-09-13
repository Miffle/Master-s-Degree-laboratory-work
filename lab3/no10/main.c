#include "/root/Desktop/AOS/lab1/no1.h"
#include <signal.h>

struct sigaction new, old;
sigset_t mask;
int h_sig = SIGINT;

void disp(int sig){
    sigset_t new_mask;
    sigemptyset(&new_mask);
    sigaddset(&new_mask,sig);
    //sigprocmask(SIG_BLOCK,&new_mask, NULL);
    printf("Signal %d handler\n",sig);
    for (int i = 0; i < 1000000000; i++);
    //sleep(10);
    puts("Exit from handler");
    //sigprocmask(SIG_UNBLOCK,&new_mask, NULL);
}

int main()
{
    if (sigaction(h_sig, NULL, &old) == -1){
        print_error(errno);
        return 1;
    }
    new.sa_handler = disp;
    mask = old.sa_mask;
    sigaddset(&mask,h_sig);
    sigaddset(&mask, SIGSTOP);
    new.sa_mask = mask;
    new.sa_flags = 0;

    if (sigaction(SIGUSR1, &new, NULL) == -1){
        print_error(errno);
        return 1;
    }
    if (sigaction(SIGINT, &new, NULL) == -1){
        print_error(errno);
        return 1;
    }
    puts("Press Ctrl+C");
    kill(getpid(),SIGUSR1);
    puts("Pause");
    pause();
    return 0;
}
