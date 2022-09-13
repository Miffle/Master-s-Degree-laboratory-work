#include <lab6.h>
int shm_id;
int sem_id;
long *shmad;
int msg_ring(unsigned int m){
    int msg_id = create_msgq();
    //printf("Create msgq = %d\n", msg_id);
    struct cr_ringmsg msg;
    msg.mtype = MSG_REQ;
    msg.m = m;

    if (msgsnd(msg_id, &msg, MSG_CRRING_SIZE, 0) == -1){
        perror("Sending \"m\" to 1st son error");
        return -1;
    }
    //printf("Send message: queue = %d m = %u\n", msg_id, m);

    if (msgrcv(msg_id, &msg, MSG_CRRING_SIZE, MSG_REP, 0) == -1){
        perror("Receiving number of winner error");
        return -1;
    }
    msgctl(msg_id,IPC_RMID,NULL);
    return (int)msg.m;
}

int main(int argc, char *argv[])
{
    if (argc < 4){
        fprintf(stderr,"Usage: %s N M L\n", argv[0]);
        return 1;
    }
    unsigned int n, m, l;
    n = str_to_ui(argv[1]);
    m = str_to_ui(argv[2]);
    l = str_to_ui(argv[3]);
    if (!n || !m || !l){
        fprintf(stderr, "%s: One of parameters isn't positive\n", argv[0]);
        return 1;
    }

    printf("Bello!\nPID %d is a creator of the main ring\n", getpid());
    printf("Parameters: N = %u M = %u L = %u\n", n, m, l);

    pid_t *son = (pid_t *)malloc(n * sizeof (pid_t));

    if (create_sons(n, l, &son, 1) == -1)
        return 1;
    int winner = msg_ring(m);
    if (winner == -1){
        killsons(son, n);
        return 1;
    }

    printf("The winner is son №%d (PID %d)\n", winner, son[winner]);
    killsons(son, n);
    for (unsigned int i = 0; i < n; i++){
        char name[5] = "\0";
        sprintf(name, "%d", i);
        unlink(name);
    }
    return 0;
}
