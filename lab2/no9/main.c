#include "/root/Desktop/AOS/lab1/no1.h"
#include <stdlib.h>


int copyfile(int fd_src, int fd_dst){
    char buf[256] = "\0";
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
    if (argc < 2){
        printf("There is no filename. USAGE:\n\t%s filename\n",argv[0]);
        return 0;
    }

    char *filename = argv[1];


    pid_t child = fork();
    if (child == -1){
        fprintf(stderr,"Creating child process error\n");
        print_error(errno);
        return 1;
    }
    int fd_in = open(filename,O_RDONLY);
    if (fd_in == -1){
        fprintf(stderr,"Open input file error\n");
        print_error(errno);
        return 1;
    }
    int cnt = 0;
    const unsigned int buflen = 255;
    const char buf[buflen + 1];
    if (argc > 2 && !strcmp(argv[2],"-o")){
        do{
            cnt = read(fd_in,buf,buflen);
            if (cnt == -1){
                fprintf(stderr,"Read error\n");
                print_error(errno);
                return 1;
            }
            write(0,buf,cnt);
        } while(cnt != 0);
        puts("");
        return 0;
    }

    char ofname[256] = "\0";
    sprintf(ofname,"%d_%s",child,filename);
    int fd_out =  open(ofname,O_CREAT|O_RDWR|O_TRUNC,0666);

    if (fd_out == -1){
        fprintf(stderr,"Open output file error\n");
        print_error(errno);
        return 1;
    }

    if (copyfile(fd_in,fd_out) == -1){
        fprintf(stderr,"Copy error\n");
        print_error(errno);
        return 1;
    }
    lseek(fd_out,0,SEEK_SET);

    if (!child){
        //printf("Child process:\nPID:\t%d\nPPID:\t%d\nPGID:\t%d\n",getpid(),getppid(),getpgrp());
        printf("File %s contains:\n",ofname);
        do{
            cnt = read(fd_out,buf,buflen);
            if (cnt == -1){
                fprintf(stderr,"Read error\n");
                print_error(errno);
                return 1;
            }
            write(0,buf,cnt);
        }while (cnt != 0);
        puts("");
        exit(0);
    }
    else{
        //printf("Parent process:\nPID:\t%d\nPPID:\t%d\nPGID:\t%d\n",getpid(),getppid(),getpgrp());
        wait();
        printf("\n\n\n\n\n\nFile %s contains:\n",ofname);
        do{
            cnt = read(fd_out,buf,buflen);
            if (cnt == -1){
                fprintf(stderr,"Read error\n");
                print_error(errno);
                return 1;
            }
            write(0,buf,cnt);
        }while (cnt != 0);
        puts("");
    }

    return 0;
}
