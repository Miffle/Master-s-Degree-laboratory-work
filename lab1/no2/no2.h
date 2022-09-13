#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>

#ifndef NO2_H
#define NO2_H

int get_fname(int argc, char *argv[], char **fname, unsigned int *mode){
    char buf[256] = "\0";
    char *end = NULL;
    int len = 0;
    if (argc < 2){
        printf("There is no argument. Usage:\n\t");
        printf("%s %s",argv[0],"filename\n");
        printf("%s", "Enter name of file you want to create -> ");
        len = scanf("%s",buf) ;
        if (len == -1)
            return 0;
        *fname = (char *)malloc(len+1);
        strcpy(*fname, buf);
        printf("%s", "Enter rights -> ");
        if (scanf("%o", mode) == -1) {
            puts("Invalid number");
            return 0;
        }
        while (getchar() != '\n');
    }
    else {
        *fname = argv[1];
        if (argc > 2)
            *mode = strtol(argv[2],&end, 8);
        else{
            if (scanf("%o", mode) == -1) {
                puts("Invalid number");
                return 0;
            }
            while (getchar() != '\n');
        }
    }
    return 0;
}

int write_to_file(int fd){
    char u_string[256] = "\0";
    puts("Enter information which writes to file:");
    if (gets(u_string) == -1)
        return -1;
    int cnt = write(fd,u_string,strlen(u_string));
    if (cnt == -1)
        print_error(errno);
    return cnt;
}

#endif // NO2_H
