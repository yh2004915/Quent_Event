
#include "usart.h"


UART_QUEN_STATUS UART_QUEN_Init(UART_QUEN *quen)
{
    quen->count = 0;
    quen->read = 0;
    quen->writer = 0;
    return UART_QUEN_OK;  
}
UART_QUEN_STATUS UART_QUEN_ADD(UART_QUEN *quen,uint8_t data)
{
    if(UART_QUEN_IS_FULL(quen) == UART_QUEN_FULL)
    {
        return UART_QUEN_ERROR;
    }
    quen->buf[quen->writer] = data;
    quen->writer = (quen->writer + 1) % UART_QUEUE_SIZE;
    quen->count++;
    return UART_QUEN_OK;

}
UART_QUEN_STATUS UART_QUEN_GET(UART_QUEN *quen,uint8_t *data)
{
    if(UART_QUEN_IS_EMPTY(quen) == UART_QUEN_EMPTY)
    {
        return UART_QUEN_ERROR;
    }
    *data = quen->buf[quen->read];
    quen->read = (quen->read + 1) % UART_QUEUE_SIZE;
    quen->count--;
    return UART_QUEN_OK;
}
UART_QUEN_STATUS UART_QUEN_IS_EMPTY(UART_QUEN *quen)
{
    return (quen->count == 0) ? UART_QUEN_EMPTY : UART_QUEN_OK;
}
UART_QUEN_STATUS UART_QUEN_IS_FULL(UART_QUEN *quen)
{
    return (quen->count == UART_QUEUE_SIZE) ? UART_QUEN_FULL : UART_QUEN_OK;
}




void UART_init(int baudrate)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

int fputc(int ch, FILE *f)
{
    /* 等待发送数据寄存器为空 */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, (uint16_t)ch);
    
    return ch;
}

