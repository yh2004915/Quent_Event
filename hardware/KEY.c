#include "KEY.h"


volatile uint32_t sys_tick_ms;

void SysTick_Handler(void)
{
    sys_tick_ms++;
}
uint32_t get_system_tick()
{
    return sys_tick_ms;
}
//框架的初始化
task_state_em task_Init(task_manager_t *manager)
{
    if (manager == NULL) return TASK_FALSE;
    
    for (int i = 0; i < MAX_TASKS; i++) {
        manager->pool[i].is_used = 0;
        manager->pool[i].state   = TASK_STATE_IDLE;
    }
    manager->task_num = 0;
    return TASK_TRUE;
}

//设备的注册
task_bool_em task_Register(task_manager_t *manager, uint32_t id, task_func func, uint32_t period)
{
    if (manager == NULL || func == NULL || period == 0)
        return TASK_FALSE;
        for (int i = 0; i < MAX_TASKS; i++) {
            if (manager->pool[i].is_used == 0) {
                manager->pool[i].task_id    = id;
                manager->pool[i].func       = func;
                manager->pool[i].period     = period;
                manager->pool[i].last_time  = 0;
                manager->pool[i].task_count = 0;
                manager->pool[i].state      = TASK_STATE_READY;
                manager->pool[i].is_used    = 1;
                manager->task_num++;
                return TASK_TRUE;
            }
        }
        return TASK_FALSE;  // 池满了   
}

/* 根据ID启动/挂起任务 */
task_bool_em task_SetState(task_manager_t *manager, uint32_t id, task_state_em state) 
{
     if(manager == NULL)
         return TASK_FALSE;
        for (int i = 0; i < MAX_TASKS; i++)
        {
            if (manager->pool[i].is_used && manager->pool[i].task_id == id)
            {
            manager->pool[i].state = state;
            return TASK_TRUE;
            }
        }
        return TASK_FALSE;
}
//根据ID删除任务
task_bool_em task_Delete(task_manager_t *manager, uint32_t id)
{

     if(manager == NULL)
        return TASK_FALSE;
    for (int i = 0; i < MAX_TASKS; i++)
    {
        if (manager->pool[i].is_used && manager->pool[i].task_id == id)
        {
            manager->pool[i].is_used = 0;
            manager->task_num--;
            return TASK_TRUE;
        }
    }
    return TASK_FALSE;
}
/* 根据ID获取任务状态 */
task_state_em task_GetState(task_manager_t *manager, uint32_t id)
{
    if(manager == NULL)
        return TASK_STATE_IDLE ;
    for (int i = 0; i < MAX_TASKS; i++)
    {
        if (manager->pool[i].is_used && manager->pool[i].task_id == id)
        {
            return manager->pool[i].state;
        }
    }
    return TASK_STATE_IDLE;
}
/* 根据ID设置任务周期 */
task_bool_em task_SetPeriod(task_manager_t *manager, uint32_t id, uint32_t period)
{
    if(manager == NULL)
        return TASK_FALSE;
    for (int i = 0; i < MAX_TASKS; i++)
    {
        if (manager->pool[i].is_used && manager->pool[i].task_id == id)
        {
            manager->pool[i].period = period;
            return TASK_TRUE;
        }
    }
    return TASK_FALSE;
}
/* 根据ID强制运行任务 */
task_bool_em task_ForceRun(task_manager_t *manager, uint32_t id)
{
    if(manager == NULL)
        return TASK_FALSE;
    for (int i = 0; i < MAX_TASKS; i++)
    {
        if (manager->pool[i].is_used && manager->pool[i].task_id == id)
        {
            manager->pool[i].state = TASK_STATE_RUNNING;
            if (manager->pool[i].func) manager->pool[i].func();
            manager->pool[i].state = TASK_STATE_READY;
            return TASK_TRUE;
        }
    }
    return TASK_FALSE;
}
/* 启动调度器 */
void task_StartScheduler(task_manager_t *manager)
{
    if(manager == NULL) return;
    manager->scheduler_enable = 1;
}
/* 停止调度器 */
void task_StopScheduler(task_manager_t *manager)
{
    if(manager == NULL) return;
    manager->scheduler_enable = 0;
}


/* 调度运行 */
void task_Run(task_manager_t *manager) 
{
    if(manager == NULL ||  manager->scheduler_enable == 0)
         return;
        uint32_t now = get_system_tick();  // 需自行实现
        for (int i = 0; i < MAX_TASKS; i++) 
        { 
                task_node_t *t = &manager->pool[i];

            if (!t->is_used || t->state != TASK_STATE_READY)
            continue;

                if ((now - t->last_time) >= t->period) 
                {
                    t->last_time = now;
                    t->state = TASK_STATE_RUNNING;
                    if (t->func) t->func();
                    t->task_count++;
                    t->state = TASK_STATE_READY;
                }
        }
}
