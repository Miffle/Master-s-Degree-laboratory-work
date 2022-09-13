#include "/root/Desktop/AOS/lab1/no1.h"
#include <signal.h>
#include <wait.h>

int main()
{
    pid_t child = fork();
    if (child == -1){
        print_error(errno);
        return 1;
    }
    if (!child){
        int it_max = 20000;
        for (int i = 0; i < it_max; i++){
            printf("Iteration %d\n", i);
            for (int j = 0; j < it_max; j++);
        }
        return 0;
    }
    int status = 0;
    wait(&status);
    if (WIFEXITED(status))
        printf("Child exited normally. Exited status -> %d\n", WEXITSTATUS(status));
    if (WIFSIGNALED(status))
        printf("Child exited by signal. Signal -> %d\n", WTERMSIG(status));
    return 0;
}
