#include "/root/Desktop/AOS/lab6/creator/lab6.h"

int shm_id;
int sem_id;
long *shmad;

void selfkill(){
    char name[5] = "\0";
    sprintf(name, "%d", getpid());
    strcat(name,"ring");
    unlink(name);
    exit(0);
}

int msg_begin(){
    int msg_id = get_msgq();
    //printf("Get msgq = %d\n", msg_id);
    struct cr_ringmsg msg;

    if (msgrcv(msg_id, &msg, MSG_CRRING_SIZE, MSG_REQ, 0) == -1){
        perror("Receiving msg from beginner error");
        return -1;
    }
    return (int)msg.m;
}

int main(int argc, char *argv[])
{
    if (argc < 3){
        fprintf(stderr,"Usage: %s L cnt\n", argv[0]);
        return 1;
    }
    signal(SIGTERM, &selfkill);
    unsigned int son_num, sons_cnt, n, m, l, p, points = 0;
    son_num = str_to_ui(argv[0]);
    sons_cnt = str_to_ui(argv[2]) - 1;
    n = 3;
    l = str_to_ui(argv[1]);

    printf("process %s: starts\n", argv[0]);
    int st;
    unsigned char fl = 0;
    while(points < l){
        if (!son_num && !fl){
            st = msg_begin();
            fl = 1;
        }
        else{
            st = srv_socket(argv[0], SOCK_STREAM);
        }
        if (st == -1)
            return 1;
        m = (unsigned int)st;
        printf("process %s: receive m = %u\n", argv[0], m);


        if (!n || !m || !l){
            fprintf(stderr, "%s: One of parameters isn't positive\n", argv[0]);
            return 1;
        }

        printf("Parameters: N = %u M = %u L = %u\n", n, m, l);

        pid_t *son = (pid_t *)malloc(n * sizeof (pid_t));

        if (create_sons(n, l, &son, 0) == -1)
            return 1;

        char sockname[25] = "\0";
        sprintf(sockname, "%d", getpid());
        strcat(sockname,"ring\0");
        printf("write to socket %s\n", sockname);
        st = cl_socket(sockname, SOCK_DGRAM, m);
        if (st == -1){
            killsons(son, n);
            return 1;
        }
        printf("process %s send to second ring m = %u\n", argv[0], m);

        st = srv_socket(sockname, SOCK_DGRAM);

        if (st == -1)
            return 1;
        p = (unsigned int)st;        
        printf("The winner from second ring id №%d (PID %d)\n", p, son[p]);
        killsons(son, n);
        long q = (long)m - p - 1;
        if (q > 0){
            //need to fill
            char sockname[5] = "0";
            if (son_num == sons_cnt)
                st = cl_socket(sockname, SOCK_STREAM, q);
            else{
                sprintf(sockname, "%u", son_num+1);
                st = cl_socket(sockname, SOCK_STREAM, q);
            }
            printf("Send q = %ld to process №%s\n", q, sockname);
            if (st == -1){
                killsons(son, n);
                return 1;
            }
        }
        else{
            points++;
            if (points < l){
                char sockname[5] = "0";
                if (son_num == sons_cnt)
                    st = cl_socket(sockname, SOCK_STREAM, m);
                else{
                    sprintf(sockname, "%u", son_num+1);
                    st = cl_socket(sockname, SOCK_STREAM, m);
                }
                printf("Send m = %u to process №%s\n", m, sockname);
                if (st == -1){
                    killsons(son, n);
                    return 1;
                }
            }
        }
        printf("Process %s has %d points\n", argv[0], points);

    }
    int msg_id = get_msgq();
    if (msg_id == -1){
        return 1;
    }
    struct cr_ringmsg msg;
    msg.mtype = MSG_REP;
    msg.m = son_num;
    if (msgsnd(msg_id, &msg, MSG_CRRING_SIZE, 0) == 1){
        perror("Sendind winner msg error");
        return 1;
    }
    selfkill();
    return 0;
}
