//
// Created by 冉惟之 on 16/5/17.
//

#include <stdlib.h>
#include "QueueHandler.h"

/* 初始化PCB块和空余队列 */
int InitGQueues(GQueues *gq) {
    gq = (GQueues *)malloc(sizeof(GQueues));
    gq->qFree.head = NULL;
    gq->qFree.tail = NULL;
    gq->qReady.head = NULL;
    gq->qReady.tail = NULL;

}

/* 释放内存 */
void DestroyGQueues(GQueues *gq) ;

/* 分配PCB内存块 */
int allocPcb(GQueues *gq, int pid, int tServing, int tArriving) {
    Pcb * pcb = (Pcb *)malloc(sizeof(Pcb));
    pcb->pid = pid;
    pcb->tOverall = tServing;
    pcb->tArriving = tArriving;
    PcbQueue * node = malloc(sizeof(PcbQueue));
    node->next = NULL;
    node->block = pcb;
    gq->qReady.tail->next = node;
    gq->qReady.tail = node;
}
/* 回收PCB块 */
void collectPcb(GQueues *gq);

/* RR算法 */
void invokeRR(PPQueue *ready, int flag);
/* SPF算法 */
void invokeSpf(PPQueue *ready);

/* 返回执行进程的pid, -1表示无就绪进程 */
int running(PcbQueue *head);

/* 队头进程是否结束，1为运行完毕 */
int finishReadyHead(GQueues *gq);