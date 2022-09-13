#include "/root/Desktop/AOS/lab1/no1.h"
#include <signal.h>

void disp(int sig){
    puts("Good. You're in handler");
    printf("Signal %d\n",sig);
    if (signal(sig,SIG_DFL) < 0){
        print_error(errno);
        return;
    }
    puts("Exit from handler");
}

int main()
{
    //printf("Hello World!\n");
    signal(SIGINT,&disp);
    puts("Press Ctrl+C");
    pause();
    pause();
    return 0;
}
