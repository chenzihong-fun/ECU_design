#include "Hall.h"

// 霍尔传感器引脚定义
#define HALL_PORT      GPIOC
#define HALL_PIN       GPIO_Pin_14
#define HALL_EXTI_LINE EXTI_Line14
#define HALL_IRQn      EXTI15_10_IRQn

// 全局变量
volatile uint32_t hall_rate = 0;      // 霍尔传感器计数
volatile uint32_t hall_rpm = 0;       // 计算得到的转速

/**
 * @brief  霍尔传感器初始化
 * @param  无
 * @retval 无
 */
void hall_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    // 配置PC14为输入模式
    GPIO_InitStructure.GPIO_Pin = HALL_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HALL_PORT, &GPIO_InitStructure);

    // 配置EXTI线14
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource14);

    EXTI_InitStructure.EXTI_Line = HALL_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // 下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = HALL_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  TIM4定时器初始化（1秒中断）
 * @param  无
 * @retval 无
 */
void hall_timer_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 配置TIM4定时器（1秒中断）
    // 时钟频率为72MHz，预分频系数为7199，计数器周期为9999
    // 72MHz / (7199+1) = 10kHz，10kHz * (9999+1) = 1秒
    TIM_TimeBaseStructure.TIM_Period = 9999;
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    // 使能TIM4更新中断
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    // 配置NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 启动TIM4
    TIM_Cmd(TIM4, ENABLE);
}

/**
 * @brief  计算转速
 * @param  无
 * @retval 转速值（RPM）
 */
uint32_t hall_calculate_rpm(void)
{
    // 计算公式：rate * 60 / 2
    return (hall_rate * 60) / 2;
}

/**
 * @brief  EXTI15_10中断处理函数（霍尔传感器中断）
 * @param  无
 * @retval 无
 */
void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(HALL_EXTI_LINE) != RESET)
    {
        // 霍尔传感器触发，计数加1
        hall_rate++;

        // 清除中断标志位
        EXTI_ClearITPendingBit(HALL_EXTI_LINE);
    }
}

/**
 * @brief  TIM4中断处理函数（1秒定时）
 * @param  无
 * @retval 无
 */
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        // 计算转速
        hall_rpm = hall_calculate_rpm();

        // 重置计数
        hall_rate = 0;

        // 清除中断标志位
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}
