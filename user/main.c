#include "stm32f10x.h"
#include "usart.h"
#include "KEY.h"
#include "LED.h"
#include "Delay.h"

int main(void)
{
   
    SysTick_Config(SystemCoreClock / 1000);

  
    UART_init(115200);
    led_init();
    key_init();

    event_st event;
		printf("Event Start : \r\n");

    while(1)
    {
        /* 从事件队列中取事件 */
        if(quent_pop(&key_queue, &event) == quent_OK)
        {
            
            if(event.event_name == KEY1_EVENT && event.event_flag == KEY1_PRESS)
            {
                    printf("key_event START\r\n");
            }
        }
    }
}

void USART1_IRQHandler(void)
{
    uint8_t data;
    while (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        data = USART_ReceiveData(USART1);
    }
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        volatile uint32_t tmp = USART1->SR;
        tmp = USART1->DR;
        (void)tmp;

        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }
}
