#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>
#include <signal.h>

void disp(int sig){

}

int receive(int fd_r, const char msg[]){
    int cnt = 0, l = 255;
    char buf[256] = "\0";
    cnt = read(fd_r, buf, l);

    if (cnt == -1){
        print_error(errno);
        return -1;
    }
    printf("PID: %d - %s%s\n",getpid(), msg, buf);

    return 0;
}

int send(int fd_w, const char msg[]){
    int cnt = 0, l = 255;
    char buf[256] = "\0";
    //printf("%s", msg);
    //cnt = scanf("%s", buf);
    write(0,msg,strlen(msg));
    cnt = read(1, buf, l);

    if (cnt == -1){
        print_error(errno);
        return -1;
    }
    if (!cnt)
        return -1;
    write(fd_w,buf,l);

    return 0;
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
    signal(SIGUSR1, &disp);
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

        while (send(w,"Parent -> ") != -1)
            if (receive(r, "C: ") == -1)
                return -1;
        int fl = fcntl(p[0],F_GETFL);
        fl = fcntl(p[0], F_SETFL, fl | O_NONBLOCK);
        //kill(child,SIGUSR1);

    }
    else{
        r = p[0];
        w = c[1];
        //cnt = scanf("%s", buf);
        if (receive(r, "P: ") == -1)
            return -1;
        while (send(w,"Child -> ") != -1)
            if (receive(r, "P: ") == -1)
                return -1;
        int fl = fcntl(c[0],F_GETFL);
        fl = fcntl(c[0], F_SETFL, fl | O_NONBLOCK);
        kill(getppid(),SIGUSR1);
    }

    return 0;
}
