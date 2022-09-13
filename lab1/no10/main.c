#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>

int main(int argc, char* argv[]) {
    printf("Bello!\n");
    int fd = 0, len = 0, size =0, cnt = 0;
    char fname[256] = "\0";
    char *buf = NULL;
    printf("Source file -> ");
    if (argc < 2){
    len = scanf("%s", fname);
        if (len == -1)
            return 0;
    }
    else {
        strcpy(fname,argv[1]);
        printf("%s\n",fname);
    }
    fd = open(fname,O_RDONLY);
    if (fd == -1){
        print_error(errno);
        return 1;
    }
    size = lseek(fd,0,SEEK_END);
    if (size == -1){
        print_error(errno);
        return 1;
    }
    buf = (char *)malloc(sizeof(char)*(size+1));
    buf[size] = '\0';
    lseek(fd,0,SEEK_SET);
    cnt = read(fd,buf,size);
    if (cnt == -1){
        print_error(errno);
        return 1;
    }
    printf("File contains:\n%s\n", buf);

    int cur = 0;

    lseek(fd,-1,SEEK_END);
    for (;cur != size;cur++){
        char c;
        read(fd,&c,1);
        buf[cur]=c;
        lseek(fd,-2,SEEK_CUR);
    }
    buf[cur]='\0';
    printf("Reverse:\n%s\n",buf);
    free(buf);
    return 0;
}
