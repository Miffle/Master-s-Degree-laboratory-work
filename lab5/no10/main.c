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
unsigned char server = 0;
int msg_req;
void killhand(int sig){
    puts("Exiting..");
    if (server){
        if (msgctl(msg_req,IPC_RMID,NULL) == -1){
            perror("Cannot remove request queue");
            exit(1);
        }
    }
    exit(0);
}

struct mymsg{
    long mtype;
    long senderpid;
    char mtext[128];
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

    char tmpfname[] = ".srv.tmp";
    int perm = 0666;
    key_t req_key = 0;
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
    }
    req_key = ftok(tmpfname, MSG_REQUEST);
    if (req_key == -1){
        perror("Key generation error");
        return -1;
    }
    msg_req = msgget(req_key, perm | IPC_CREAT);
    if (msg_req == -1) {
        perror("Create msg queue error");
        return -1;
    }
    printf("PID %d: Request - %d\n", getpid(), msg_req);
    int clen = 0;
    int maxlen = 128+sizeof (long);
    struct mymsg mbuf;
    if (server){
        while(1){
            clen = msgrcv(msg_req, &mbuf, maxlen, MSG_REQUEST, 0);
            if(clen == -1){
                perror("Message receive error");
            }
            else{
                mbuf.mtype = mbuf.senderpid;
                sprintf(mbuf.mtext,"Server receive message from %d", mbuf.mtype);
                if(msgsnd(msg_req, &mbuf, strlen(mbuf.mtext)+sizeof(long)+1, 0) == -1){
                    perror("Server send message error");
                    return -1;
                }
            }
        }
    }
    else{
        int cnt = 0, rec = 0, rcnt = 0;
        do{
            mbuf.mtype = MSG_REQUEST;
            mbuf.senderpid = getpid();
            printf("PID %d: Enter message:\n", getpid());
            cnt = scanf("%s", mbuf.mtext);
            if (msgsnd(msg_req, &mbuf,strlen(mbuf.mtext)+sizeof(long)+1, 0) == -1){
                perror("Client send message error");
                return -1;
            }

            while (rcnt != -1){
                rcnt = msgrcv(msg_req,&mbuf,maxlen,getpid(),IPC_NOWAIT);
                if (rcnt != -1)
                    printf("PID %d: %s\n", getpid(), mbuf.mtext);
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
