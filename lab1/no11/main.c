#include "/root/Desktop/AOS/lab1/no1.h"

int main(int argc, char* argv[])
{
    int *sizes = NULL, max = 0, max_i = 0;
    struct stat buf;
    sizes = (int *)malloc(sizeof(int)*(argc-1));

    printf("No.\t| Filename\t|\tSize\n---------------------------------------\n");
    for(int i=argc-1; i>0; i--){
        if(stat(argv[i],&buf) == -1){
            print_error(errno);
            return 1;
        }
        sizes[i] = buf.st_size;
        if (sizes[i] > max){
            max = sizes[i];
            max_i = i;
        }
        printf("%d.\t| %s\t|\t%d\n", i, argv[i], sizes[i]);
    }

    printf("Max size %d has file \"%s\"\n", max, argv[max_i]);

    free(sizes);
    return 0;
}
