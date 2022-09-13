#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>

extern char **environ;

void print_env(){
    if (environ)
        for (size_t i = 0;environ[i] != NULL;i++){
            puts(environ[i]);
        }
}

int main(int argc, char *argv[])
{
    printf("Hello World!\n");
    char *name = NULL, *value = NULL;
    char buf[256] = "\0";
    int cnt = 1, mod_fl = 0;

    puts("Current environment:");
    print_env();
    for(; cnt < argc; cnt++){
        value = strchr(argv[cnt],'=');
        if (value){
            *value = '\0';
            name = argv[cnt];
            value++;
            if (setenv(name, value, 0)){
                print_error(errno);
                return 1;
            }
            mod_fl = 1;
        }
    }
    if (mod_fl){
        puts("\nModificated environment:");
        print_env();
    }
    return 0;
}
