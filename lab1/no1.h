#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#ifndef NO1_H
#define NO1_H

void print_error(int err){
    printf("errno: %d\n",err);
    perror("perror result:");
    printf("%s%d%s","sys_errlist[",err,"]: ");
    printf("%s\n",strerror(err));
    puts("syserrlist:");

}

#endif // NO1_H
