//
// Created by 冉惟之 on 16/5/17.
//

#include <stdlib.h>
#include <stdio.h>
#include "QueueHandler.h"

#define RUNNING 0
#define FINISHED 1
/* 初始化PCB块和空余队列 */
int InitGQueues(GQueues *gq) {
    gq->qFree.head = NULL;
    gq->qFree.tail = gq->qFree.head;
    gq->qReady.head = NULL;
    gq->qReady.tail = gq->qReady.head;

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
    if (gq->qFree.head == NULL) {
        gq->qFree.head = node;
        gq->qFree.tail = gq->qFree.head;
        return 0;
    }
    gq->qFree.tail->next = node;
    gq->qFree.tail = node;
    return 0;
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
        if (node->next != NULL) {
            printf("task %d finished, next task %d scheduled\n", node->block->pid, ready->head->block->pid);
        }
        else {
            printf("task %d finished, no next task comming\n", node->block->pid);
        }
        collectPcb(node);
        return;
    }
    if (ready->head->next == NULL) {
        printf("task %d does not finish and next task %d scheduled\n", node->block->pid, node->block->pid);
        return;
    }
    printf("task %d does not finish and next task %d scheduled\n", node->block->pid, ready->head->next->block->pid);
    ready->head = ready->head->next;
    node->next = NULL;
    ready->tail->next = node;
    ready->tail = node;
}
/* SPF算法 */
void invokeSpf(PPQueue *ready) {
    PcbQueue *node = ready->head;
    if (node == NULL) {
        return;
    }
    if (node->block->state == RUNNING) {
        return;
    }
    ready->head = ready->head->next;
    if (ready->head == NULL) {
        printf("task %d finished, no next task comming\n", node->block->pid);
        return;
    }
    int mini = ready->head->block->tOverall;
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
                next->next = ready->head;
                ready->head = next;
                break;
            }
        }
    }
    printf("task %d finished, next task %d scheduled\n", node->block->pid, ready->head->block->pid);
    collectPcb(node);
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
    static int time = -1;
    time++;
    printf("now time: %d\n", time);
    if (gq->qFree.head != NULL && gq->qFree.head->block->tArriving == time) {
        PcbQueue *node = gq->qFree.head;
        gq->qFree.head = gq->qFree.head->next;
        node->next = NULL;
        if (gq->qReady.head == NULL) {
            gq->qReady.head = node;
            gq->qReady.tail = gq->qReady.head;
            return 0;
        }
        else {
            gq->qReady.tail->next = node;
            gq->qReady.tail = node;
        }
    }
    if (gq->qFree.head != NULL) {
        for (PcbQueue *temp = gq->qFree.head; temp->next != NULL; temp = temp->next) {
            if (temp->next->block->tArriving == time) {
                PcbQueue *node = temp;
                temp->next = temp->next->next;
                node->next = NULL;
                gq->qReady.tail->next = node;
                gq->qReady.tail = node;
            }
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