#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>
#include <signal.h>

void disp(int sig){

}

int main()
{
    int p[2], c[2];
    int r = 0, w = 1, cnt = 0, l = 255;
    char msg[256] = "\0", buf[256] = "\0";
    const char ch[] = "C: ", par[] = "P: ";
    if (pipe(p) == -1){
        print_error(errno);
        return -1;
    }
    if (pipe(c) == -1){
        print_error(errno);
        return -1;
    }
    pid_t child = fork();
    if (child == -1){
        print_error(errno);
        return -1;
    }
    if (child){
        r = c[0];
        w = p[1];
        strcat(msg,par);
        strcat(msg, "Hello, child!");

        signal(SIGUSR1, &disp);

        cnt = read(r, buf, l);
        while (cnt != 0){
            if (cnt == -1){
                print_error(errno);
                return -1;
            }
            printf("PID: %d - %s\n",getpid(),buf);
            cnt = read(r, buf, l);
        }

    }
    else{
        r = p[0];
        w = c[1];
        //cnt = scanf("%s", buf);
        cnt = read(1, buf, l);
        while (cnt != 0){
            if (cnt == -1){
                print_error(errno);
                return -1;
            }
            write(w,buf,l);
            //cnt = scanf("%s", buf);
            cnt = read(1, buf, l);
        }
        int fl = fcntl(c[0],F_GETFL);
        fl = fcntl(c[0], F_SETFL, fl | O_NONBLOCK);
        kill(getppid(),SIGUSR1);
    }
    close(r);
    close(w);

    return 0;
}
