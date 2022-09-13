#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>
#include <signal.h>
#include <stdlib.h>

int main()
{
    int p[2];

    if (pipe(p) == -1){
        print_error(errno);
        return -1;
    }


    pid_t child = fork();
    if (child == -1){
        print_error(errno);
        return 1;
    }
    if (child){
        if (dup2(p[1], 1) == -1){
            perror("Dup error");
            return 1;
        }
        close(p[0]);
        close(p[1]);
        char *args[] = {"who", NULL};
        if (execvp(args[0], args) == -1){
            perror("Exec error");
            return 1;
        }
    }
    else{
        if (dup2(p[0], 0) == -1){
            perror("Dup error");
            return 1;
        }
        close(p[0]);
        close(p[1]);
        char *args[] = {"wc", "-l" ,NULL};
        if (execvp(args[0], args) == -1){
            perror("Exec error");
            return 1;
        }
    }
    return 0;
}
