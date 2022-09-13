#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>
#include <signal.h>

int main(int argc, char * argv[])
{
    if (argc < 2){
        fprintf(stderr, "Erron arguments");
        return 1;
    }
    int fd, cnt = 0, wrcnt = 0, len = 255;
    char buf[256] = "\0";
    pid_t child = fork();
    if (child == -1){
        perror("Fork error");
        return -1;
    }
    if (child){
        fd = open(argv[1], O_RDONLY);
        if (fd == -1){
            perror("Open file (read) error");
            return -1;
        }
        do{
            cnt = read(fd, buf, 1);
            if (cnt == -1){
                perror("Read from file error");
                return -1;
            }
            if(write(1, buf, cnt) == -1){
                perror("Write to terminal error");
                return -1;
            }
        } while(cnt != 0);
    }
    else {
        fd = open(argv[1], O_WRONLY);
        if (fd == -1){
            perror("Open file (write) error");
            return -1;
        }
        /*do{
            cnt = read(0, buf, len);
            if (cnt == -1){
                perror("Read from terminal error");
                return -1;
            }
            buf[0] = '0';
            buf[1] = '\0';
            wrcnt = write(fd, buf, 1);
            if (wrcnt == -1){
                perror("Write to file error");
                return -1;
            }
        }while (1);*/
        char b[] = "Hello, i'm writer, how are you?\0";
        for (int i = 0; i < strlen(b); i++){
            wrcnt = write(fd, &(b[i]), 1);
            if (wrcnt == -1){
                perror("Write to file error");
                return -1;
            }
        }

    }
    return 0;
}
