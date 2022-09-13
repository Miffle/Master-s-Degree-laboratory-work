#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>
#include <signal.h>

int open_fifo(const char *fname, int mode){
    int fd;
    fd = open(fname, mode);
    if (fd == -1){
        if (errno == 2){
            fd = mknod(fname,S_IFIFO | 0666 ,0);
            if (fd == -1){
                perror("Mkfifo error");
                return -1;
            }
            fd = open(fname, mode);
            if (fd == -1){
                perror("Open fifo error");
                return -1;
            }
        }
        else{
            perror("Open fifo error");
            return -1;
        }
    }
    return fd;
}

int main(int argc, char *argv[])
{
    int r = 0, w = 1;
    const char *rd = NULL, *wr = NULL;
    if (argc > 2){
        rd = argv[1];
        wr = argv[2];
    }
    else{
        fprintf(stderr,"Error arguments");
        return 1;
    }

    int cnt = 0, len = 255;
    char c = 0;
    char buf[256] = "\0", beg[] = "Other:";
    pid_t child = fork();
    if (child == -1){
        perror("Fork error");
        return -1;
    }
    if (child){
        w = open_fifo(wr, O_WRONLY);
        if (w == -1)
            return -1;
        do{
            c = cnt = read(0, buf, len);
            //printf("\b\rYou: %s\n", buf);
            if (cnt == -1){
                perror("Read error");
                return -1;
            }
            if (cnt && (write(w, &c, 1) == -1)){
                perror("Write error");
                return -1;
            }
            if (cnt && (write(w, buf, c) == -1)){
                perror("Write error");
                return -1;
            }

        } while(cnt != 0);
    }
    else{
        r = open_fifo(rd, O_RDONLY | O_NDELAY);
        if (r == -1)
            return -1;
        do{
            cnt = read(r, &c, 1);
            if (cnt == -1){
                perror("Read error");
                return -1;
            }
            cnt = read(r, buf, c);
            if (cnt == -1){
                perror("Read error");
                return -1;
            }
            buf[cnt] = '\0';
            if (cnt)
                printf("%s %s", beg, buf);
        } while(cnt != 0);
    }





    return 0;
}
