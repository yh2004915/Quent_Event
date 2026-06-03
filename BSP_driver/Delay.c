#include "stm32f10x.h"
#include "KEY.h"

/**
  * @brief  毫秒级延时（基于调度器的系统滴答，不破坏 SysTick 配置）
  * @param  xms 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_ms(uint32_t xms)
{
	uint32_t target = get_system_tick() + xms;
	while (get_system_tick() < target);
}
 
/**
  * @brief  秒级延时
  * @param  xs 延时时长，范围：0~4294967295
  * @retval 无
  */
void Delay_s(uint32_t xs)
{
	while(xs--)
	{
		Delay_ms(1000);
	}
} 
