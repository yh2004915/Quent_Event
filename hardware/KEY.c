#include "KEY.h"
#include "Delay.h"

/* 全局按键事件队列 */
quent_st key_queue;

/* 按键去抖用：记录上次按键触发时的系统 tick */
static volatile uint32_t last_press_tick = 0;

/* ======== 事件队列函数 ======== */

quent_em quent_Init(quent_st *q)
{
    q->head = 0;
    q->write = 0;
    q->quent_state = quent_empty;
    q->count = 0;
    return quent_OK;
}

quent_em quent_push(quent_st *q, event_st *event)
{
    if(q == NULL || event == NULL)
    {
        return quent_error;
    }
    if(quent_is_full(q) == quent_full)
    {
        return quent_full;
    }
    q->quent[q->write] = *event;
    q->write = (q->write + 1) % quent_size;
    q->count++;
    q->quent_state = (q->count == quent_size) ? quent_full :  quent_normal ;
    return quent_OK;

}
quent_em quent_pop(quent_st *q, event_st *event)
{
    if(q == NULL || event == NULL)
    {
        return quent_error;
    }
    if(quent_is_empty(q) == quent_empty)
    {
        return quent_empty;
    }
    *event = q->quent[q->head];
    q->head = (q->head + 1) % quent_size;
    q->count--;
    q->quent_state = (q->count == 0) ? quent_empty : quent_normal ;
    return quent_OK;
}

quent_em quent_is_empty(quent_st *q)
{
    if(q->count == 0)
    {
        return quent_empty;
    }
    else
    {
        return quent_OK;
    }
}

quent_em quent_is_full(quent_st *q)
{
    if(q->count == quent_size)
    {
        return quent_full;
    }
    else
    {
        return quent_OK;
    }
}

/* ======== 按键硬件初始化（PA0 EXTI0 下降沿触发） ======== */

void key_init(void)
{
    /* 初始化事件队列 */
    quent_Init(&key_queue);

    /* 使能 GPIOA 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    /* 使能 AFIO 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* PA0 上拉输入（按键按下 -> 低电平） */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 将 EXTI0 连接到 PA0 */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    /* EXTI0 配置：下降沿中断 */
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* NVIC 配置 */
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/* ======== EXTI0 中断服务函数 ======== */

void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {

        uint32_t now = get_system_tick();
        if(now - last_press_tick > 200)
        {
            last_press_tick = now;
            event_st event = {KEY1_EVENT, KEY1_PRESS};
            quent_push(&key_queue, &event);
        }
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
