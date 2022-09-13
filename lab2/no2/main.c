#include "/root/Desktop/AOS/lab1/no1.h"
#include <math.h>

int main()
{
    //printf("Hello World!\n");
    int fd = 0;
    const char temp_fname[] = "temp.log";


    pid_t p_test = fork();
    if (p_test == -1){
        print_error(errno);
        return -1;
    }
    if (p_test){
        fd = creat(temp_fname, 0744);
        close(fd);
        wait();
        fd = open(temp_fname,O_RDONLY);
        char c;
        int par = 0, ch = 0;
        while (read(fd,&c,1) != 0){
            if (c == '0')
                par++;
            if (c == '1')
                ch++;
            read(fd,&c,1);
        }
        close(fd);
        unlink(temp_fname);
        printf("Parent:\t%d\nChild:\t%d\n",par,ch);
    }
    else{
        //printf("Child process\n");
        for (size_t cnt = 1000; cnt != 0; cnt--){
            pid_t child = fork();
            fd = open(temp_fname,O_WRONLY|O_CREAT|O_APPEND,0744);

            for (unsigned int i = 5000000; i != 0; i--);
            if (child == -1){
                print_error(errno);
                close(fd);
                return -1;
            }
            if (child){
                //printf("Parent process\n");
                write(fd,"0",1);
                close(fd);
                wait();
            }
            else{
                //printf("Child process\n");
                write(fd,"1",1);
                close(fd);
                exit(0);
            }
        }
    }

    return 0;
}
