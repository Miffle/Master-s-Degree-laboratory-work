#include "/root/Desktop/AOS/lab1/no1.h"
#include <signal.h>
#include <wait.h>

int it_max = 50000, i = 0;

void disp(int sig){
    i = it_max;
}

int main()
{
    pid_t child = fork();
    if (child == -1){
        print_error(errno);
        return 1;
    }
    if (!child){
        if (signal(SIGUSR1, &disp) < 0){
            print_error(errno);
            return 1;
        }
        for (; i < it_max; i++){
            printf("Iteration %d\n", i);
            for (int j = 0; j < it_max; j++);
        }
        return 0;
    }
    sleep(1);
    kill(child,SIGUSR1);
    int status = 0;
    wait(&status);

    if (WIFEXITED(status))
        printf("Child exited normally. Exited status -> %d\n", WEXITSTATUS(status));
    if (WIFSIGNALED(status))
        printf("Child exited by signal. Signal -> %d\n", WTERMSIG(status));

    return 0;
}
