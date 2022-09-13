#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>
#include <signal.h>

int lock(struct flock *flck){
    struct flock lck = *flck;
    lck.l_type = F_RDLCK | F_WRLCK;
    lck.l_whence = SEEK_SET;
    lck.l_start = 0;
    lck.l_len = 0;
    if (fcntl(0, F_SETLKW, &lck) == -1){
        perror("Setting ReadLock error");
        return -1;
    }
    if (fcntl(1, F_SETLKW, &lck) == -1){
        perror("Setting ReadLock error");
        return -1;
    }
    *flck = lck;
    return 0;
}

int unlock(struct flock *flck){
    struct flock lck = *flck;
    lck.l_type = F_UNLCK;
    if (fcntl(0, F_SETLK, &lck) == -1){
        perror("Unlock error");
        return -1;
    }
    if (fcntl(1, F_SETLK, &lck) == -1){
        perror("Unlock error");
        return -1;
    }
    sleep(1);
    *flck = lck;
    return 0;
}

void dialog(char c){
    struct flock lck;
    int cnt = 0;
    char buf[128] = "\0";
    do{
        if (lock(&lck) == -1)
            return;
        printf("%c -> ", c);
        cnt = scanf("%s", buf);
        printf("%c: %s\n", c, buf);
        buf[0] = '\0';
        if (unlock(&lck) == -1)
            return ;
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
