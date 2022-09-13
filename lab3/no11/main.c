#include "/root/Desktop/AOS/lab1/no1.h"
#include <signal.h>

int sync_sig = SIGUSR1;
sigset_t sync_mask, emp_mask;

void disp(int sig){

}

void dream(const char msg[], pid_t s_pid){    
    printf("%s suspends..\n", msg);
    kill(s_pid,sync_sig);
    sigsuspend(&emp_mask);

}

void work(const char msg[], pid_t s_pid){
    printf("%s works..\n", msg);
    for (int i = 0; i < 1000000000; i++);
    printf("%s waits..\n", msg);
    kill(s_pid,sync_sig);
    sigsuspend(&emp_mask);
}


int main()
{
    sigset_t disp_mask;
    struct sigaction act;
    sigaction(sync_sig, NULL, &act);
    act.sa_handler = disp;
    sigaction(sync_sig, &act, NULL);

    sigfillset(&emp_mask);
    sigdelset(&emp_mask,sync_sig);
    sigemptyset(&sync_mask);
    sigaddset(&sync_mask, sync_sig);
    sigprocmask(SIG_BLOCK, &sync_mask, NULL);
    pid_t child = fork();
    if (child < 0){
        print_error(errno);
        return 1;
    }
    if (child){
        dream("Parent", child);
        puts("Parent receive the signal");
        work("Parent", child);
        puts("Parent is done");
    }
    else{
        work("Child", getppid());
        puts("Child receive the signal");
        dream("Child", getppid());
        puts("Child is done");
    }
    sigprocmask(SIG_UNBLOCK, &sync_mask, NULL);
    return 0;
}
