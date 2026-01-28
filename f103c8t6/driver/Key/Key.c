#include "stm32f10x.h"  // 改为F103的头文件
#include "key.h"
#include "key_desc.h"
#include "stdbool.h"

#define KEY_LONG_PRESS_TIME_MS 2000

static key_callback_t key_callback ;

//采用上升沿触发 只有单按键按下且松开时才触发中断

void key_init(key_desc_t* key)
{
    // 配置GPIO
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = key->pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // F103中使用GPIO_Mode_IPU替代GPIO_PuPd_UP
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(key->port, &GPIO_InitStructure);

    GPIO_EXTILineConfig(key->exti_port, key->exti_pinsource);  // F103中使用GPIO_EXTILineConfig替代SYSCFG_EXTILineConfig

    // 配置EXTI
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = key->exti_line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  // 保持原有触发方式
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
        timer_delay_ms(10); // 按下时消抖：跳过抖动期
        if (GPIO_ReadInputDataBit(key->port, key->pin) == 0) // 二次检测：确认按下
        {
            // 2. 等待按键释放（关键！避免按住期间反复触发）
            key_wait_released(key);
            timer_delay_ms(10); // 释放时消抖：避免松开瞬间抖动误触发
            return true; // 一次完整的有效按键
        }
    }
    return false;
}


void key_set_callback(key_desc_t* key, key_callback_t callback)
{
    key_callback = callback ;
}


