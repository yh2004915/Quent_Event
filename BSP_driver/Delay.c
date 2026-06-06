#include "Delay.h"
#include "stm32f10x.h"

static volatile uint32_t systick_counter = 0;

void SysTick_Handler(void)
{
    systick_counter++;
}

uint32_t get_system_tick(void)
{
    return systick_counter;
}

void Delay_ms(uint32_t xms)
{
    uint32_t target = systick_counter + xms;
    while (systick_counter < target);
}

void Delay_s(uint32_t xs)
{
    while(xs--)
    {
        Delay_ms(1000);
    }
}
