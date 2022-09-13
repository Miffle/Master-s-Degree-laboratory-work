#include "no1.h"

int main(int argc, char* argv[])
{
    char *buff = NULL;
    char buf[256] = "\0";
    int fd = 0;
    printf("Bello!\n");
    if (argc == 1){
        printf("There is no argument. Usage:\n\t");
        printf("%s %s",argv[0],"filename\n");
        printf("%s", "Enter name of file you want to open -> ");
        if(scanf("%s",buf) == -1)
            return 0;
        buff = buf;
    }
    else {
        buff = argv[1];
    }
    printf("%s %s\n","Opening file:",buff);
    fd = open(buff,O_RDWR);
    if (fd == -1)
        print_error(errno);
    for (int i =0; i< sys_nerr;i++){
        printf("%d %s\n",i,sys_errlist[i]);
    }
    return 0;
}


