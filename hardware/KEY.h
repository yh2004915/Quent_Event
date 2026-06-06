#ifndef __KEY_H__
#define __KEY_H__

#define quent_size 16

#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

/* 按键事件定义 */
#define KEY1_EVENT      1
#define KEY1_PRESS      1

/* 事件结构体 */
typedef struct
{
    uint8_t event_name;
    uint8_t event_flag;
}event_st;

typedef enum
{
    quent_OK,
    quent_full,
    quent_empty,
    quent_error,
    quent_normal
}quent_em;

/* 事件队列结构体（环形缓冲区） */
typedef struct
{
    event_st quent[quent_size];
    uint8_t head;
    uint8_t write;
    uint8_t count;
    quent_em quent_state;
}quent_st;

quent_em quent_Init(quent_st *q);
quent_em quent_push(quent_st *q, event_st *event);
quent_em quent_pop(quent_st *q, event_st *event);
quent_em quent_is_empty(quent_st *q);
quent_em quent_is_full(quent_st *q);


void key_init(void);

extern quent_st key_queue;

#endif
