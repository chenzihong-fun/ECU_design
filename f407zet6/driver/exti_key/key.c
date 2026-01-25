#include "stm32f4xx.h"
#include "key.h"
#include "key_desc.h"
#include "stdbool.h"

#define KEY_LONG_PRESS_TIME_MS 2000

static key_callback_t key_callback ;

void key_init(key_desc_t* key)
{
    // 配置GPIO
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = key->pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(key->port, &GPIO_InitStructure);

    SYSCFG_EXTILineConfig (key->exti_port, key->exti_pinsource);

    // 配置EXTI
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = key->exti_line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  // 按下后且松开才触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 配置中断
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = key->exti_irq;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void key_wait_released(key_desc_t* key)
{
    while (GPIO_ReadInputDataBit(key->port, key->pin) == 0);
}

bool key_pressed(key_desc_t* key)
{
    // 1. 检测按键是否按下（假设低电平有效，即按下时GPIO=0）
    if (GPIO_ReadInputDataBit(key->port, key->pin) == 0)
    {
        tim_delay_ms(10); // 按下时消抖：跳过抖动期
        if (GPIO_ReadInputDataBit(key->port, key->pin) == 0) // 二次检测：确认按下
        {
            // 2. 等待按键释放（关键！避免按住期间反复触发）
            key_wait_released(key);
            tim_delay_ms(10); // 释放时消抖：避免松开瞬间抖动误触发
            return true; // 一次完整的有效按键
        }
    }
    return false;
}


bool key_long_pressed(key_desc_t* key)
{
    uint64_t start_time = tim_get_ms();
    while (key_pressed(key))
    {
        if (tim_get_ms() - start_time > KEY_LONG_PRESS_TIME_MS)
        {
            return true;
        }
    }
    return false;
}

void key_set_callback(key_desc_t* key, key_callback_t callback)
{
    key_callback = callback ;
}

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line0);
        if (key_callback)
        {
            key_callback();
        }
    }
}

void EXTI1_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line1) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line1);
        if (key_callback)
        {
            key_callback();
        }
    }
}

