#ifndef __KEY_H_
#define __KEY_H_

#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"
#include "usart.h"

/* 编译一个关于时间片轮询的调度机制用链表进行编写 */

#define MAX_TASKS    16    // 最大任务数（不用256，省内存）
#define TASK_ID_MAX  32    // 任务ID最大长度

typedef void (*task_func)(void);

typedef enum {
    TASK_STATE_IDLE = 0,
    TASK_STATE_READY,
    TASK_STATE_RUNNING,
    TASK_STATE_SUSPENDED
} task_state_em;

typedef enum {
    TASK_FALSE = 0,
    TASK_TRUE  = 1
} task_bool_em;

/* 单个任务节点 */
typedef struct {
    uint32_t task_id;          // 唯一标识
    uint32_t period;           // 执行周期
    uint32_t last_time;        // 上次执行时间
    uint32_t task_count;       // 执行次数
    task_state_em state;       // 当前状态
    task_func func;            // 任务函数
    uint8_t  is_used;          // 槽位是否被占用
} task_node_t;

/* 管理器 */
typedef struct {
    task_node_t pool[MAX_TASKS];  // 静态任务池
    uint8_t     task_num;         // 当前任务数
    uint8_t scheduler_enable;
} task_manager_t;


task_state_em task_Init(task_manager_t *manager);
task_bool_em task_Register(task_manager_t *manager, uint32_t id, task_func func, uint32_t period);
task_bool_em task_SetState(task_manager_t *manager, uint32_t id, task_state_em state); 
void task_Run(task_manager_t *manager); 
void task_StartScheduler(task_manager_t *manager);
void task_StopScheduler(task_manager_t *manager);
task_bool_em task_ForceRun(task_manager_t *manager, uint32_t id);
task_bool_em task_SetPeriod(task_manager_t *manager, uint32_t id, uint32_t period);
task_state_em task_GetState(task_manager_t *manager, uint32_t id);
task_bool_em task_Delete(task_manager_t *manager, uint32_t id);



#endif

