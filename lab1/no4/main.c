#include <stdio.h>
#include "/root/Desktop/AOS/lab1/no2/no2.h"

int seek(int fd, int size){
    char buf[256] = "\0";

    printf("Reading from begining of file %d bytes:\n",size/2);

    lseek(fd,0,SEEK_SET);
    if (read(fd,buf,size/2) == -1)
        print_error(errno);
    printf("%s\n", buf);

    printf("Writing to end of file:\n");
    lseek(fd,0,SEEK_END);
    int cnt = write_to_file(fd);
    if(cnt == -1)
        return -1;
    size += cnt;
    int offset = -size*2/3, count = size/3;
    printf("Reading from end of file with offset %d %d bytes:\n",offset,count);
    lseek(fd,offset,SEEK_END);
    if (read(fd,buf,count) == -1)
        print_error(errno);
    printf("%s\n", buf);


    return 0;
}

int alm_emp(int fd, int size){
    int offset = size*1000*8;
    size = lseek(fd,offset,SEEK_END);
    printf("Writing to end of file with offset %d:\n",offset);
    int cnt = write_to_file(fd);
    if(cnt == -1)
        return -1;
    size += cnt;
    return cnt;
}

int alm_emp1(int fd, int size){
    int offset = 1000*8;
    size = lseek(fd,offset,SEEK_END);
    printf("Writing to end of file with offset %d:\n",offset);
    int cnt = write_to_file(fd);
    if(cnt == -1)
        return -1;
    size = lseek(fd,0,SEEK_END);
    lseek(fd, -size/2, SEEK_END);
    char buf[256] = "\0";
    cnt = read(fd,buf,5);

    if (cnt == -1){
        print_error(errno);
    }
    else{
        printf("Read read %d bytes -> %s\n", cnt, buf);
    }

    lseek(fd,25,SEEK_END);
    cnt = read(fd,buf,5);
    if (cnt == -1) {

        print_error(errno);
    }
    else{

        printf("Read read %d bytes -> %s\n", cnt, buf);
    }

    return cnt;
}

int main(int argc, char* argv[])
{
    char *fname = NULL;

    unsigned int mode;
    int fd = 0, size = 0;
    get_fname(argc,argv,&fname,&mode);
    printf("%s %s %s %o\n","Creating file:",fname,"Rights:",mode);
    fd = creat(fname,mode);
    if (fd == -1){
        print_error(errno);
        return 1;
    }
    if (write_to_file(fd) == -1)
        return 0;

    size = lseek(fd,0,SEEK_END);
    close(fd);
    fd = open(fname,O_RDWR);

    if (argc > 3) {
        if (!strcmp(argv[3],"-ae")){
            if(alm_emp(fd,size)== -1){
                return 0;
            }
        }
        if (!strcmp(argv[3],"-ae1")){
            if(alm_emp1(fd,size)== -1){
                return 0;
            }
        }
    }
    else
        if(seek(fd,size) == -1)
            return 0;

    close(fd);
    return 0;
}
