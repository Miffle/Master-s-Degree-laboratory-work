#include <no2.h>

int main(int argc, char* argv[])
{
    char *buff = NULL;
    int fd = 0;
    unsigned int mode = 0700;
    printf("Bello!\n");
    get_fname(argc,argv,&buff,&mode);
    printf("%s %s %s %o\n","Creating file:",buff,"Rights:",mode);
    fd = open(buff,O_WRONLY|O_CREAT|O_EXCL,mode);
    //fd = creat(buff,mode);
    if (fd == -1)
        print_error(errno);

    int cnt = write_to_file(fd);
    if (cnt == -1) return 1;

    close(fd);

    fd = open(buff,O_RDONLY);
    if (fd == -1){

        return 1;
    }
    char u_string[256] = "\0";
    read(fd,u_string,cnt);
    printf("Info from file: %s\n",u_string);
    close(fd);

    fd = open(buff,O_RDWR);
    if (fd == -1)
        print_error(errno);
    else
        close(fd);
    return 0;
}


