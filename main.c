#include "QueueHandler.h"
#include <stdio.h>
#include <stdlib.h>

#define RR 0
#define SPF 1
int main(int argc, char ** argv) {
    static GQueues * gq;
    int i;
    int tServing, tArriving;
    int nSchedule = 0;
    int RRflag = 0;

    if (argc == 1) {
        nSchedule = atoi(argv[1]);
    }
    InitGQueues(gq);

/* 根据录入进程数据，分配PCB */
    allocPcb(gq, i, tServing, tArriving);

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

        /* 运行 */
        pid = running(gq->qReady.head);

        /* 就绪队列的队头是否运行完毕？*/
        RRflag = finishReadyHead(gq);

        /* 是否全部运行完毕？*/
        if (gq->qReady.head == NULL)
            break;
    } while (1);

    /* 结束实验，清理内存 */
    DestroyGQueues(gq);
}