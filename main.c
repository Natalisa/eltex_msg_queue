#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

struct message{
    long mtype;
    char mesg[20];
};

int main(){
    key_t token = ftok("./main.c",'A');
    switch (fork()) {
        case 0: {
            int ds = msgget(token, IPC_CREAT | 0666);
            struct message buf;
            buf.mtype = 1;
            strcpy(buf.mesg, "Hello");
            if(msgsnd(ds, &buf, sizeof(buf), 0) < 0) {
                perror("P2: Error msgsnd()");
            }
            sleep(1);
            msgrcv(ds, &buf, sizeof(buf), 1, 0);
            printf("P2 = %s\n", buf.mesg);
            msgctl(ds, IPC_RMID, 0);
        } break;
        default: {
            sleep(1);
            int ds = msgget(token, 0666);
            struct message buf;
            msgrcv(ds, &buf, sizeof(buf), 1, 0);
            printf("P1 = %s\n", buf.mesg);
            strcpy(buf.mesg, "Hi");
            if(msgsnd(ds, &buf, sizeof(buf), 0) < 0) {
                perror("P1: Error msgsnd()");
            }
        } break;
    }
    return 0;
}
