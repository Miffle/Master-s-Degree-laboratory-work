#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>

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
    }
    else{
        r = p[0];
        w = c[1];
        strcat(msg,ch);
        strcat(msg, "Hi, parent!");
    }
    cnt = write(w, msg, l);

    if (cnt == -1){
        print_error(errno);
        return -1;
    }
    cnt = read(r, buf, cnt);
    if (cnt == -1){
        print_error(errno);
        return -1;
    }

    printf("PID: %d - %s\n",getpid(),buf);

    return 0;
}
