#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

#define MSG_DR 1
#define MSG_WR 2

struct mymsg{
    long mtype;
    char mtext[128];
};

int msg_d;

void print_qstat(int msg_id){
    struct msqid_ds qstatus;
    msgctl(msg_id,IPC_STAT,&qstatus);
    printf("Real user id of the queue creator: %d\n",qstatus.msg_perm.cuid);
    printf("Real group id of the queue creator: %d\n",qstatus.msg_perm.cgid);

    printf("Effective user id of the queue creator: %d\n",qstatus.msg_perm.uid);
    printf("Effective group id of the queue creator: %d\n",qstatus.msg_perm.gid);
    printf("Permissions: %d\n",qstatus.msg_perm.mode);

    printf("%d message(s) on queue\n",qstatus.msg_qnum);
    printf("Last message sent by process :%3d at %s", qstatus.msg_lspid, ctime(&(qstatus.msg_stime)));

    printf("Last message received by process :%3d at %s",qstatus.msg_lrpid,ctime(&(qstatus.msg_rtime)));
    printf("Current number of bytes on queue %d\n",qstatus.msg_cbytes);
    printf("Maximum number of bytes allowed on the queue %d\n",qstatus.msg_qbytes);
}

void killhand(int sig){
    puts("Exiting..");

    if (msgctl(msg_d,IPC_RMID,NULL) == -1){
        perror("Cannot remove request queue");
        exit(1);
    }

    exit(0);
}

int create_msgq(){
    char tmpfname[] = ".srv.tmp";
    int perm = 0666;
    key_t key = 0;
    int msg_id = 0;
    signal(SIGINT, &killhand);

    int tmpfd = creat(tmpfname, 0644);
    if (tmpfd == -1){
        perror("Create tmp file error");
        return -1;
    }
    close(tmpfd);

    key = ftok(tmpfname, getpid());
    if (key == -1){
        perror("Key generation error");
        return -1;
    }
    msg_id = msgget(key, perm | IPC_CREAT);
    if (msg_id== -1) {
        perror("Create msg queue error");
        return -1;
    }
    return msg_id;
}

void dream(const char msg[], pid_t s_pid){
    printf("%s suspends..\n", msg);
    struct mymsg mbuf;
    mbuf.mtype = MSG_DR;
    if (msgsnd(msg_d,&mbuf,0,0) == -1){
        perror("Send message error");
        exit(1);
    }
    if (msgrcv(msg_d,&mbuf,0,MSG_WR,0) == -1){
        perror("Receive message error");
        exit(1);
    }
}

void work(const char msg[], pid_t s_pid){
    printf("%s works..\n", msg);
    for (int i = 0; i < 1000000000; i++);
    printf("%s waits..\n", msg);
    struct mymsg mbuf;
    mbuf.mtype = MSG_WR;
    if (msgsnd(msg_d,&mbuf,0,0) == -1){
        perror("Send message error");
        exit(1);
    }
    if (msgrcv(msg_d,&mbuf,0,MSG_DR,0) == -1){
        perror("Receive message error");
        exit(1);
    }
}


int main()
{
    msg_d = create_msgq();
    if (msg_d == -1)
        return -1;

    pid_t child = fork();
    if (child < 0){
        perror("Fork error");
        return 1;
    }
    if (child){
        dream("Parent", child);
        puts("Parent receive the message");
        work("Parent", child);
        puts("Parent is done");
        killhand(0);
    }
    else{
        work("Child", getppid());
        puts("Child receive the message");
        dream("Child", getppid());
        puts("Child is done");
    }

    return 0;
}
