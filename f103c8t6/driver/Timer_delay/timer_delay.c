#include <stdbool.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "timer_delay.h"


static timer_elapsed_callback_t timer_elapsed_callback;



void timer_tim2_init_us(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void tim_delay_us(uint32_t us)
{
    if (us == 0)
        return;
    
    // 清零计数器
    TIM_SetCounter(TIM2, 0);
    
    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);
    
    // 等待计数器达到指定值
    while (TIM_GetCounter(TIM2) < us);
    
    // 关闭定时器
    TIM_Cmd(TIM2, DISABLE);
    
}

/**
 * @brief  毫秒级延时函数
 * @param  ms: 需要延时的毫秒数
 * @retval 无
 */
void tim_delay_ms(uint32_t ms)
{
    while (ms--)
    {
        tim_delay_us(1000);  // 调用1000次微秒延时函数
    }
}

void timer_elapsed_register(timer_elapsed_callback_t callback)
{
    timer_elapsed_callback = callback;
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        if (timer_elapsed_callback)
        {
            timer_elapsed_callback();
        }
    }
}
