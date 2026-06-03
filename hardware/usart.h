#ifndef __UART_H__
#define __UART_H__ 

#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

void UART_init(int baudrate);

/*
 *  UART+循环缓冲队列进行串口数据的收发
 */
#define UART_QUEUE_SIZE 128 

//环形队列的主要的结构体
 typedef struct  
 {
    int writer;//写指针
    int read;//读指针
    char buf[UART_QUEUE_SIZE];//主要的存放载体
    int count;//队列中存放的元素个数
 }UART_QUEN;

 //环形队列的状态
 typedef enum
 {
    UART_QUEN_OK,
    UART_QUEN_FULL,
    UART_QUEN_EMPTY,
    UART_QUEN_ERROR
 }UART_QUEN_STATUS;


 UART_QUEN_STATUS UART_QUEN_Init(UART_QUEN *quen);
 UART_QUEN_STATUS UART_QUEN_ADD(UART_QUEN *quen,uint8_t data);
 UART_QUEN_STATUS UART_QUEN_GET(UART_QUEN *quen,uint8_t *data);
 UART_QUEN_STATUS UART_QUEN_IS_EMPTY(UART_QUEN *quen);
 UART_QUEN_STATUS UART_QUEN_IS_FULL(UART_QUEN *quen);




#endif