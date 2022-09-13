#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <stdlib.h>

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
    if (argc < 3){
        printf("Usage %s qmsgid msgtype [len]\n", argv[0]);
        return 1;
    }
    int msgd = atoi(argv[1]), perm = 0444, len = 127;
    if (argc > 3)
        len = atoi(argv[3]);
    long msgtype = atol(argv[2]);

    struct msqid_ds qstatus;
    if(msgctl(msgd,IPC_STAT,&qstatus) == -1){
        perror("Queue status error");
        return -1;
    }
    //print_qstat(&qstatus);

    struct mymsg1{
        long mtype;
        char mtext[128];
    } mbuf1;

    struct mymsg2{
        long mtype;
        struct {
            unsigned int len;
            char mtext[128];
        } info;
    } mbuf2;
    int clen = 0;
    if (msgtype == 1){
        while (clen != -1){
            clen = msgrcv(msgd,&mbuf1,len,msgtype,IPC_NOWAIT | MSG_NOERROR);
            if(clen == -1){
                perror("Message receive error");
            }
            else
                printf("Message with type %d contains:\n%s\n",mbuf1.mtype,mbuf1.mtext);
        }
    }
    if (msgtype == 2){
        while(clen != -1){
            clen = msgrcv(msgd,&mbuf2,len,msgtype,IPC_NOWAIT | MSG_NOERROR);
            if(clen == -1){
                perror("Message receive error");
            }
            else
                printf("Message with type %d has length %d and contains:\n%s\n",mbuf2.mtype,mbuf2.info.len, mbuf2.info.mtext);
        }
    }
    //print_qstat(&qstatus);


    return 0;
}
