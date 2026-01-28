#include "SG90.h"

//PB0 作为SG90的PWM输出引脚
//对应的是tim3_channel3
#define PWM_PORT                     GPIOB
#define PWM_PIN                      GPIO_Pin_0
#define PWM_TIM                      TIM3
#define PWM_CHANNEL                  TIM_OC3Init

uint16_t SG90_FRONT_SPEED[4] = {SG90_FRONT_SPEED1_DUTY, SG90_FRONT_SPEED2_DUTY, SG90_FRONT_SPEED3_DUTY, SG90_FRONT_SPEED4_DUTY};
uint16_t SG90_BACK_SPEED[4] = {SG90_BACK_SPEED1_DUTY, SG90_BACK_SPEED2_DUTY, SG90_BACK_SPEED3_DUTY, SG90_BACK_SPEED4_DUTY};

static void SG90_GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 配置PB0为复用推挽输出
    GPIO_InitStructure.GPIO_Pin = PWM_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PWM_PORT, &GPIO_InitStructure);
}

static void SG90_TIMER_PWM_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    // 初始化TIM3时间基准
    TIM_TimeBaseStructure.TIM_Period = SG90_ARR;  // 周期20ms
    TIM_TimeBaseStructure.TIM_Prescaler = 72-1;  // 预分频器，72MHz/72=1MHz计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(PWM_TIM, &TIM_TimeBaseStructure);
    
    // 初始化TIM3通道3为PWM模式1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = SG90_STOP_PULSE;  // 初始状态：停止
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    PWM_CHANNEL(PWM_TIM, &TIM_OCInitStructure);
    
    // 启动TIM3的预装载寄存器
    TIM_OC3PreloadConfig(PWM_TIM, TIM_OCPreload_Enable);
    
    // 启动TIM3的自动重装载预装载寄存器
    TIM_ARRPreloadConfig(PWM_TIM, ENABLE);
    
    // 启动TIM3
    TIM_Cmd(PWM_TIM, ENABLE);
}


void SG90_init(void)
{
    SG90_GPIO_init();
    SG90_TIMER_PWM_init();
}

void SG90_SetSpeed(uint16_t speed)
{
    TIM_SetCompare3(PWM_TIM, speed);
}