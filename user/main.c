#include "stm32f10x.h"// Device header
#include "usart.h"
#include "KEY.h"
#include "LED.h"





task_manager_t LED_TASK_manager;

void LED_TASK(void)
{
	printf("LED_TASK is running\r\n");
	task_SetPeriod(&LED_TASK_manager,2,500);
}
void KEY_TASK(void)
{
    printf("KEY_TASK is running\r\n");
}
void USART_TASK(void)
{
    printf("USART_TASK is running\r\n");
}

void main(void)
{
		SysTick_Config(SystemCoreClock / 1000);  
		led_init();
    UART_init(115200);
    task_Init(&LED_TASK_manager);
    task_Register(&LED_TASK_manager, 1, LED_TASK, 500);
    task_Register(&LED_TASK_manager, 2, KEY_TASK, 2000);
    task_Register(&LED_TASK_manager, 3, USART_TASK, 1000);
    task_StartScheduler(&LED_TASK_manager);
		printf("Task Scheduler Start\r\n");

    while (1)
    {   
       task_Run(&LED_TASK_manager);
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
        /* 
         * 关键：必须先读 DR，再读 SR，才能清除 IDLE 标志
         * 这是 STM32F1 的硬件特性
         */
        volatile uint32_t tmp = USART1->SR;  // 读状态寄存器
        tmp = USART1->DR;                     // 读数据寄存器（清除 IDLE 位）
        (void)tmp;
        
        
        USART_ClearITPendingBit(USART1, USART_IT_IDLE);
    }
   
}


