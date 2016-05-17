#ifndef _TYPE_DEFINES_H_
#define _TYPE_DEFINES_H_
typedef struct _Pcb{
    int pid;			/* 进程ID */
    char name[20];	/* 进程名 */

    int state;			/* 进程状态 */
    int priority;		/* 进程优先级 */

    int tArriving;		/* 到达时间 */
    int tOverall;		/* 运行总时间 */
    int tRunnng;		/* 已运行时间 */
} Pcb;

typedef struct _PcbQueue{
    Pcb *block;		        /* Pcb数据域 */
    struct _PcbQueue *next;		/* 指针域 */
} PcbQueue;

typedef struct _PPQueue{
    PcbQueue *head;
    PcbQueue *tail;
} PPQueue;

typedef struct _GQueues{
    PPQueue qFree;		/* 空余队列 */
    PPQueue qReady;		/* 就绪队列 */
} GQueues;

/* 分配的最大空余PCB块个数 */
#define MAX_PCB_NUM 10
extern Pcb pcbBlock[MAX_PCB_NUM];


#endif