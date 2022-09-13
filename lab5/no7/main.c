#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>

#define MSG_REQUEST 2
#define MSG_REPLY 1
#define MSG_EXIT 3
int msgd;
void killhand(int sig){
    puts("Exiting..");
    if (msgctl(msgd,IPC_RMID,NULL) == -1){
        perror("Cannot remove message queue");
        exit(1);
    }
    exit(0);
}

struct info{
    long senderid;
    long receiverid;
    char mtext[128];
};
struct mymsg{
    long mtype;
    struct info info;
};

void print_qstat(struct msqid_ds *qstat){
    struct msqid_ds qstatus = *qstat;
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

int main(int argc, char *argv[])
{
    unsigned char server = 0;
    char tmpfname[] = ".srv.tmp";
    int srvid, perm = 0666;
    key_t msg_key, srv_key = 0;
    signal(SIGINT, &killhand);
    if (argc > 1 && !strcmp(argv[1],"-s")){
        puts("Server running...");
        server = 1;
        int tmpfd = creat(tmpfname, 0644);
        if (tmpfd == -1){
            perror("Create tmp file by server error");
            return -1;
        }
        close(tmpfd);
        msg_key = ftok(tmpfname, 1);
    }
    else{
        msg_key = ftok(argv[0], getpid());
        srv_key = ftok(tmpfname, 1);
    }
    if (msg_key == -1 || srv_key == -1){
        perror("Key generation error");
        return -1;
    }
    msgd = msgget(msg_key, perm | IPC_CREAT);
    if (msgd == -1){
        perror("Create msg queue error");
        return -1;
    }
    printf("PID %d: Message queue id is %d\n", getpid(), msgd);
    int clen = 0;
    int maxlen = sizeof (struct info);
    struct mymsg mbuf;
    if (server){
        while(1){
            clen = msgrcv(msgd,&mbuf,maxlen, 0, 0);
            if(clen == -1){
                perror("Message receive error");
            }
            else{
                int clid = mbuf.info.receiverid;
                mbuf.mtype = MSG_REPLY;
                if(msgsnd(clid, &mbuf, strlen(mbuf.info.mtext)+2*sizeof(long)+1, 0) == -1){
                    perror("Server send message error");
                    return -1;
                }
            }
        }
    }
    else{
        srvid = msgget(srv_key, perm);
        if (srvid == -1){
            perror("Get server msg queue error");
            return -1;
        }
        int cnt = 0, rec = 0, rcnt = 0;
        printf("PID %d: Enter receiver message queue -> ", getpid());
        cnt = scanf("%d", &(mbuf.info.receiverid));
        do{
            mbuf.mtype = MSG_REQUEST;
            mbuf.info.senderid = msgd;
            printf("PID %d: Enter message:\n", getpid());
            cnt = scanf("%s", mbuf.info.mtext);
            if (msgsnd(srvid, &mbuf,strlen(mbuf.info.mtext)+2*sizeof(long)+1, 0) == -1){
                perror("Client send message error");
                return -1;
            }

            while (rcnt != -1){
                rcnt = msgrcv(msgd,&mbuf,maxlen,0,IPC_NOWAIT);
                if (rcnt != -1)
                    printf("PID %d: %s\n", getpid(), mbuf.info.mtext);
            }
            rcnt = 0;
        }while(cnt != EOF);
    }


    if(server && unlink(tmpfname) == -1){
        perror("Remove tmp file by server error");
        return -1;
    }
    return 0;
}
