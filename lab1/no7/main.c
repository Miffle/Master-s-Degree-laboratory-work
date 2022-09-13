#include "/root/Desktop/AOS/lab1/no1.h"
#include <time.h>

int main(int argc, char *argv[])
{
    printf("Bello!\nSystem call \"stat\"\n");
    if (argc < 2){
        printf("There is no argument. Usage:\n\t");
        printf("%s %s",argv[0],"filename\n");
        return 0;
    }
    char *fname = argv[1];
    struct stat buf;
    printf("%s\t\t\t%s\n","Filename:",fname);
    if (stat(fname,&buf) == -1)
        print_error(errno);
    else{
        printf("%s\t\t\t%d\n","ID of device containing file",buf.st_dev);
        printf("%s\t\t\t%d\n","Inode number",buf.st_ino);
        if (buf.st_rdev)
            printf("%s\t\t\t%d\n","Device ID",buf.st_rdev);
        printf("%s\t\t\t%d\n","Size in bytes",buf.st_size);
        printf("%s\t\t\t%d\n","Size in blocks",buf.st_blocks/8);
        printf("%s\t\t\t%d\n","Size of block",buf.st_blksize);
        printf("%s\t\t\t\t%o\n","Access",buf.st_mode);
        printf("%s\t\t\t%d\n","Number of links",buf.st_nlink);
        printf("%s\t\t\t%d\n","Owner's uid",buf.st_uid);
        printf("%s\t\t%d\n","Owner's group gid",buf.st_gid);
        struct tm *tmp_time = localtime(&buf.st_atime);
        char time[256];
        strftime(time, 256, "%d.%m.%Y %H:%M:%S, %A", tmp_time);
        printf("%s\t\t%s\n","Time of last access",time);
        tmp_time = localtime(&buf.st_mtime);
        strftime(time, 256, "%d.%m.%Y %H:%M:%S, %A", tmp_time);
        printf("%s\t%s\n","Time of last modification",time);
        tmp_time = localtime(&buf.st_ctime);
        strftime(time, 256, "%d.%m.%Y %H:%M:%S, %A", tmp_time);
        printf("%s\t\t%s\n","Time of last cnange",time);

    }
    return 0;
}
