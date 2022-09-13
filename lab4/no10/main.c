#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>
#include <signal.h>

void dialog(char c){
    int cnt = 0;
    unsigned int len = 127;
    char buf[128] = "\0";
    do{
        printf("%c -> ", c);
        cnt = scanf("%s", buf);
        printf("%c: %s\n", c, buf);
        buf[0] = '\0';
    } while (cnt != EOF);
}

void dialog1(char c){
    int cnt = 0;
    unsigned int len = 127;
    char buf[128] = "\0";
    do{
        //printf("%c -> ", c);
        //cnt = scanf("%s", buf);
        printf("%c: %s\n", c, "0");
        buf[0] = '\0';
    } while (1);
}

int main()
{
    char c = '\0';
    pid_t child = fork();
    if (child == -1){
        perror("Fork error");
        return -1;
    }
    if (child){
        c = 'P';
        dialog(c);
    }
    else{
        c = 'C';
        dialog1(c);
    }
    return 0;
}
