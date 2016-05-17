//
// Created by 冉惟之 on 16/5/17.
//

#include <stdlib.h>
#include "QueueHandler.h"

#define RUNNING 0
#define FINISHED 1
/* 初始化PCB块和空余队列 */
int InitGQueues(GQueues *gq) {
    gq = (GQueues *)malloc(sizeof(GQueues));
    gq->qFree.head = NULL;
    gq->qFree.tail = NULL;
    gq->qReady.head = NULL;
    gq->qReady.tail = NULL;

}

/* 释放内存 */
void DestroyGQueues(GQueues *gq) {
    free(gq);
}

/* 分配PCB内存块 */
int allocPcb(GQueues *gq, int pid, int tServing, int tArriving) {
    Pcb * pcb = (Pcb *)malloc(sizeof(Pcb));
    pcb->pid = pid;
    pcb->tOverall = tServing;
    pcb->tArriving = tArriving;
    pcb->tRunnng = 0;
    pcb->state = RUNNING;
    PcbQueue * node = malloc(sizeof(PcbQueue));
    node->next = NULL;
    node->block = pcb;
    gq->qFree.tail->next = node;
    gq->qFree.tail = node;
}
/* 回收PCB块 */
void collectPcb(PcbQueue *node) {
    free(node->block);
    node->next = NULL;
    free(node);
}

/* RR算法 */
void invokeRR(PPQueue *ready, int flag) {
    PcbQueue *node = ready->head;
    if (ready->head == NULL) {
        return;
    }
    if (flag) {
        ready->head = ready->head->next;
        collectPcb(node);
        return;
    }
    ready->head = ready->head->next;
    ready->tail->next = node;
    ready->tail = node;
}
/* SPF算法 */
void invokeSpf(PPQueue *ready) {
    PcbQueue *node = ready->head;
    if (node->block->state == RUNNING) {
        return;
    }
    ready->head = ready->head->next;
    collectPcb(node);
    int mini = ready->head->block->pid;
    PcbQueue *next = ready->head;
    for (PcbQueue *temp = ready->head; temp->next != NULL; temp = temp->next) {
        if (temp->next->block->tOverall < mini) {
            mini = temp->next->block->tOverall;
            next = temp->next;
        }
    }
    if (next != ready->head) {
        for (PcbQueue *temp = ready->head; temp->next != NULL; temp = temp->next) {
            if (temp->next->block->pid == next->block->pid) {
                temp->next = temp->next->next;
                next->next = ready->head->next;
                ready->head = next;
            }
        }
    }
}

/* 返回执行进程的pid, -1表示无就绪进程 */
int running(PcbQueue *head) {
    if (head == NULL) {
        return -1;
    }
    return head->block->pid;
}

/* 队头进程是否结束，1为运行完毕 */
int finishReadyHead(GQueues *gq) {
    static int time = 0;
    time++;
    if (gq->qFree.head->block->tArriving == time) {
        PcbQueue *node = gq->qFree.head;
        node->next = NULL;
        gq->qFree.head = gq->qFree.head->next;
        gq->qReady.tail->next = node;
        gq->qReady.tail = node;
    }
    for (PcbQueue *temp = gq->qFree.head; temp->next != NULL; temp = temp->next) {
        if (temp->next->block->tArriving == time) {
            PcbQueue *node = temp;
            node->next = NULL;
            temp->next = temp->next->next;
            gq->qReady.tail->next = node;
            gq->qReady.tail = node;
        }
    }
    PcbQueue *node = gq->qReady.head;
    if (gq->qReady.head == NULL) {
        return 0;
    }
    node->block->tRunnng++;
    if (node->block->tRunnng == node->block->tOverall) {
        node->block->state = FINISHED;
        return 1;
    }
    return 0;

}