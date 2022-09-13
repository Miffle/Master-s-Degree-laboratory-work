#ifndef LAB6_H
#define LAB6_H

#include "/root/Desktop/AOS/lab1/no1.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#define MSG_REQ 1
#define MSG_REP 2
#define MSG_CRRING_SIZE sizeof(unsigned long)

extern int sem_id;
extern int shm_id;
extern long *shmad;

struct cr_ringmsg {
    long mtype;
    unsigned long m;
};

int get_sem(void);
int get_shm(void);

int killsons(pid_t son[], unsigned int count){
    if (count){
        while (count){
            count--;
            kill(son[count], SIGTERM);
        }
        kill(son[0],SIGTERM);
    }
    free(son);
    return 0;
}

int srv_socket(const char sock_name[], int sock_type){
    unlink(sock_name);
    int srv_sock = socket(AF_UNIX, sock_type, 0), cl_sock = 0;
    struct sockaddr_un srv_addr, cl_addr;
    unsigned int srv_len, cl_len;
    srv_addr.sun_family = AF_UNIX;
    strcpy(srv_addr.sun_path, sock_name);
    srv_len = sizeof (srv_addr);
    bind(srv_sock, (struct sockaddr *)&srv_addr, srv_len);
    long cnt = 0;
    int m = 0;
    switch (sock_type) {
    case SOCK_STREAM:
        listen(srv_sock, 2);
        cl_len = sizeof (cl_addr);
        cl_sock = accept(srv_sock, (struct sockaddr *)&cl_addr, &cl_len);
        cnt = read(cl_sock, &m, 4);

        break;
    case SOCK_DGRAM:
        cnt = recvfrom(srv_sock, &m, sizeof(int), 0, NULL, NULL);
        break;
    default:
        break;
    }
    if (cnt == -1){
        perror("ReadSocket error");
        return -1;
    }


    close(cl_sock);
    return m;
}

int cl_socket(const char sock_name[], int sock_type, long q){
    sleep(1);
    int cl_sock = socket(AF_UNIX, sock_type, 0);
    struct sockaddr_un cl_addr;
    unsigned int cl_len;
    cl_addr.sun_family = AF_UNIX;
    strcpy(cl_addr.sun_path, sock_name);
    cl_len = sizeof (cl_addr);
    long cnt = 0;
    switch (sock_type) {
    case SOCK_STREAM:
        if (connect(cl_sock, (struct sockaddr *)&cl_addr, cl_len) == -1){
            perror("Connect error");
            return -1;
        }
        cnt = write(cl_sock, &q, sizeof(long));
        break;
    case SOCK_DGRAM:
        cnt = sendto(cl_sock, &q, sizeof (q),0,(struct sockaddr *)&cl_addr, sizeof(cl_addr));
        break;
    }

    if (cnt == -1){
        perror("WriteSocket error");
        return -1;
    }
    close(cl_sock);
    return 0;
}

void seckill(){
    if(shmdt(shmad) == -1)
        perror("Detach shared memory error");
    if(semctl(sem_id,3,IPC_RMID,0) == -1 && errno != EINVAL){
        perror("Remove semaphore error");
    }
    if(shmctl(shm_id,IPC_RMID,NULL) == -1 && errno != EINVAL){
        perror("Remove shared memory error");
    }
    exit(0);
}

int create_sons(unsigned int sons_cnt, unsigned int lim, pid_t *sons[], unsigned char isbeginner){
    pid_t *son = *sons;
    char args[3][21];
    sprintf(args[1], "%u", lim);
    sprintf(args[2], "%u", sons_cnt);

    for (unsigned int i = 0; i < sons_cnt; i++){
        son[i] = fork();
        switch (son[i]) {
        case -1:
            perror("Creating son (fork) of main ring error");
            killsons(son, i);
            return -1;
        case 0:
            if (isbeginner){
                sprintf(args[0], "%u", i);
                execl("child", args[0], args[1], args[2], NULL);
                perror("Creating son (exec) of main ring error");
                killsons(son, i);
                return -1;
            }
            else{
                signal(SIGTERM, &seckill);
                unsigned int points = 0;
                unsigned int m;
                char parsockname[25] = "\0";
                sprintf(parsockname, "%d", getppid());
                strcat(parsockname,"ring\0");
                long st;
                unsigned char fl = 0;
                sem_id = get_sem();
                if (sem_id == -1)
                    return -1;
                struct sembuf sop[2] = {{(unsigned short)i,-1,0},{(unsigned short)i+1,1,0}};
                if (i == 2)
                    sop[1].sem_num = 0;
                shm_id = get_shm();
                shmad = (long *)shmat(shm_id, NULL, 0);
                while (points < lim) {
                    //printf("\tsecond ring %d\n", i);
                    if (!i && !fl){
                        st = srv_socket(parsockname, SOCK_DGRAM);
                        fl = 1;
                    }
                    else{
                        semop(sem_id, &(sop[0]), 1);
                        //work with shared memory
                        st = *shmad;                        
                    }
                    if (st == -1)
                        return -1;
                    m = (unsigned int)st;
                    printf("\tsecond ring %u receive m = %u\n",i,m);
                    long q = (long)m - 1;
                    if (q > 0){                        
                        //work with shared memory
                        *shmad = q;
                        semop(sem_id,&(sop[1]), 1);
                    }
                    else{
                        points++;
                        if (points < lim){
                            *shmad = m;
                            semop(sem_id,&(sop[1]), 1);
                        }
                    }

                }
                printf("\tfrom second ring send winner = %u\n", i);

                st = cl_socket(parsockname, SOCK_DGRAM, i);
                if (st == -1)
                    return -1;
                exit(0);

            }
        default:
            break;
        }
    }
    return 0;
}

unsigned int str_to_ui(char string[]){
    int num = atoi(string);
    if (num > 0)
        return (unsigned int)num;
    else
        return 0;
}

key_t gen_key(int par){
    char tmpfname[] = ".lab6.tmp";
    key_t key = 0;

    key = ftok(tmpfname, par);
    if (key == -1){
        perror("Key generation error");
        return -1;
    }
    return key;
}

int get_shm(){
    int perm = 0666;
    int shm_id = 0;
    key_t key = gen_key(getppid());

    if (key == -1)
        return -1;
    shm_id = shmget(key, sizeof (long), perm | IPC_CREAT);
    if (shm_id== -1) {
        perror("Create/get shared memory error");
        return -1;
    }
    return shm_id;
}

int get_sem(){
    int perm = 0666;
    int sem_id = 0;
    key_t key = gen_key(getppid());

    if (key == -1)
        return -1;
    sem_id = semget(key, 3, perm | IPC_CREAT);
    if (sem_id== -1) {
        perror("Create/get semaphore error");
        return -1;
    }
    return sem_id;
}

int get_msgq(){
    int perm = 0666;
    key_t key = 0;
    int msg_id = 0;

    key = gen_key(1);

    if (key == -1)
        return -1;
    msg_id = msgget(key, perm | IPC_CREAT);
    if (msg_id== -1) {
        perror("Create/get msg queue error");
        return -1;
    }
    return msg_id;
}

int create_msgq(){
    char tmpfname[] = ".lab6.tmp";

    int tmpfd = creat(tmpfname, 0644);
    if (tmpfd == -1){
        perror("Create tmp file error");
        return -1;
    }
    close(tmpfd);

    return get_msgq();
}

#endif // LAB6_H
