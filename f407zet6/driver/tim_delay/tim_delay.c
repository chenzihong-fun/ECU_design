#include <stdint.h>
#include <string.h>
#include "stm32f4xx.h"
#include "tim_delay.h"

static volatile uint64_t tim_tick_count;
static tim_periodic_callback_t periodic_callback;

void tim_delay_init(void)
{
    RCC_ClocksTypeDef RCC_ClocksStruct;
    RCC_GetClocksFreq(&RCC_ClocksStruct);
    uint32_t apb1_tim_freq_mhz = RCC_ClocksStruct.PCLK1_Frequency / 1000 / 1000 * 2;

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = apb1_tim_freq_mhz - 1;
    TIM_TimeBaseStructure.TIM_Period = 999;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM6, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/* 
正常情况
进入函数后tim6不进入中断
tim_tick_count == last_time
异常情况
如果相等，说明发生抢占  
即当进入此函数时，tim6刚好进入了中断，导致了last_time =1000  ，而此时tim_tick_count=2000,造成了获取到的值不等
	
*/
uint64_t tim_now(void)
{
	uint64_t last_time;
    uint64_t now;
    do {
		last_time = tim_tick_count;  
        now = last_time + TIM_GetCounter(TIM6);
    }while (tim_tick_count != last_time); //如果while成立，则说明发生抢占，需进入循环，更新last_time
    return now;
}



uint64_t tim_get_us(void)
{
    return tim_now();
}

uint64_t tim_get_ms(void)
{
    return tim_now() / 1000;
}

void tim_delay_us(uint32_t us)
{
    uint64_t now = tim_now();
    while (tim_now() - now < (uint64_t)us);
}

void tim_delay_ms(uint32_t ms)
{
    uint64_t now = tim_now();
    while (tim_now() - now < (uint64_t)ms * 1000);
}

void tim_register_periodic_callback(tim_periodic_callback_t callback)
{
    periodic_callback = callback;
}

void TIM6_DAC_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        tim_tick_count += 1000;
        if (periodic_callback)
            periodic_callback();
    }
}
