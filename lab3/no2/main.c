#include "/root/Desktop/AOS/lab1/no1.h"
#include <signal.h>

struct sigaction new, old;
sigset_t mask;

void disp(int sig){
    puts("Good. You're in handler");
    printf("Signal %d\n",sig);
    new.sa_handler = SIG_DFL;
    if (sigaction(SIGINT,&new,NULL) == -1){
        print_error(errno);
        return;
    }

    puts("Exit from handler");
}

int main()
{

    sigemptyset(&mask);
    new.sa_handler = disp;
    new.sa_mask = mask;
    new.sa_flags = 0;

    if (sigaction(SIGINT,&new,&old) == -1){
        print_error(errno);
        return 1;
    }
    puts("Press Ctrl+C");
    pause();
    pause();
    return 0;
}
