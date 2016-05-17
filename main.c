#include "QueueHandler.h"
#include <stdio.h>
#include <stdlib.h>

#define RR 0
#define SPF 1

int main(int argc, char ** argv) {
    int nSchedule = SPF;
    int RRflag = 0;
    int pid;
//    if (argc == 1) {
//        nSchedule = atoi(argv[1]);
//    }
//    else {
//        printf("too few arguments\n");
//        return -1;
//    }
    GQueues *gq = (GQueues *) malloc(sizeof(GQueues));
    InitGQueues(gq);

/* 根据录入进程数据，分配PCB */
    allocPcb(gq, 0, 3, 0);
    allocPcb(gq, 1, 6, 2);
    allocPcb(gq, 2, 4, 4);
    allocPcb(gq, 3, 5, 6);
    allocPcb(gq, 4, 2, 8);

/* 输入进程调度方式　*/

    do {
        /* 调度就绪队列 */
        switch (nSchedule)
        {
            case RR:
                invokeRR(&(gq->qReady), RRflag);
                break;
            case SPF:
                invokeSpf(&(gq->qReady));
                break;
            default:
                break;
        }

        /* 就绪队列的队头是否运行完毕？*/
        RRflag = finishReadyHead(gq);
        /* 是否全部运行完毕？*/
        if (gq->qReady.head == NULL)
            break;

        /* 运行 */
        pid = running(gq->qReady.head);
        printf("now running %d\n", pid);
    } while (1);

    /* 结束实验，清理内存 */
    DestroyGQueues(gq);
}