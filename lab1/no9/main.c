#include "/root/Desktop/AOS/lab1/no1.h"
#include <math.h>

int copyfile(){
    char buf[256] = "\0";
    int fd_src = 0, fd_dst = 1;
    const int buf_len = 255;
    int cnt = 0;

    cnt = read(fd_src,buf,buf_len);
    while(cnt != 0){
        if (cnt == -1){
            print_error(errno);
            return -1;
        }
        if(write(fd_dst,buf,cnt) == -1){
            print_error(errno);
            return -1;
        }
        cnt = read(fd_src,buf,buf_len);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    char src[256] = "\0", dst[256] = "\0";
    int src_len = 0, dst_len = 0;
    int fd_src = 0, fd_dst = 1;

    if (argc > 1){
        strcpy(src,argv[1]);
        src_len = strlen(src);
        printf("Source file -> %s\n",src);

        if (argc > 2){
            strcpy(dst,argv[2]);
            dst_len = strlen(dst);
            printf("Destination file -> %s\n",dst);
            close(fd_dst);
            fd_dst = open(dst,O_RDWR|O_CREAT,0766);
        }
        //close(fd_src);
        fd_src = open(src,O_RDONLY);
        fd_src = dup2(fd_src,0);


    }

    if (fd_src == -1 || fd_dst == -1){
        print_error(errno);
        return 1;
    }

   if (copyfile() == -1)
        return 1;
    close(fd_src);
    close(fd_dst);
    return 0;
}
