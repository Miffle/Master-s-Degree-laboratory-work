#include "/root/Desktop/AOS/lab1/no1.h"
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

void print_qstat(struct msqid_ds *qstat){
    struct msqid_ds qstatus = *qstat;
    printf("Real user id of the queue creator: %d\n",qstatus.msg_perm.cuid);
    printf("Real group id of the queue creator: %d\n",qstatus.msg_perm.cgid);

    printf("Effective user id of the queue creator: %d\n",qstatus.msg_perm.uid);
    printf("Effective group id of the queue creator: %d\n",qstatus.msg_perm.gid);
    printf("Permissions: 0%o\n",qstatus.msg_perm.mode);

    printf("%d message(s) on queue\n",qstatus.msg_qnum);
    printf("Last change time %s", ctime(&(qstatus.msg_ctime)));
    printf("Last message sent by process :%3d at %s", qstatus.msg_lspid, ctime(&(qstatus.msg_stime)));

    printf("Last message received by process :%3d at %s",qstatus.msg_lrpid,ctime(&(qstatus.msg_rtime)));
    printf("Current number of bytes on queue %d\n",qstatus.msg_cbytes);
    printf("Maximum number of bytes allowed on the queue %d\n",qstatus.msg_qbytes);
}

int main(int argc, char *argv[])
{
    int msgd, perm = 0666;
    key_t msg_key = ftok(argv[0], argc);
    if (msg_key == -1){
        perror("Key generation error");
        return -1;
    }
    msgd = msgget(msg_key, perm | IPC_CREAT);
    if (msgd == -1){
        perror("Create msg error");
        return -1;
    }
    printf("Message queue with id = %d created\n", msgd);
    struct msqid_ds qstatus;

    if(msgctl(msgd,IPC_STAT,&qstatus) == -1){
        perror("Queue status error");
        return -1;
    }
    print_qstat(&qstatus);

    struct mymsg1{
        long mtype;
        char mtext[128];
    } mbuf1;
    mbuf1.mtype = 1;
    strcpy(mbuf1.mtext, "First message\0");

   struct mymsg2{
       long mtype;
       struct {
           unsigned int len;
           char mtext[128];
       } info;
   } mbuf2;
    mbuf2.mtype = 2;
    strcpy(mbuf2.info.mtext, "Second message\0");
    mbuf2.info.len = strlen(mbuf2.info.mtext);

    msgsnd(msgd,&mbuf1, strlen(mbuf1.mtext)+1, 0);
    msgsnd(msgd,&mbuf2, mbuf2.info.len+sizeof(unsigned int)+1, 0);
    puts("\nAfter sending:\n");
    if(msgctl(msgd,IPC_STAT,&qstatus)<0){
        perror("Queue status error");
        return -1;
    }
    print_qstat(&qstatus);
    if (argc < 2 || strcmp(argv[1], "-n"))
        if(msgctl(msgd,IPC_RMID,NULL) == -1){
            perror("Queue remove error");
            return -1;
        }

    return 0;
}
