#include "Beep.h"


#define BEEP_PORT      GPIOB
#define BEEP_PIN       GPIO_Pin_15

/**
  * @brief  初始化蜂鸣器引脚
  * @param  无
  * @retval 无
  */
void beep_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 配置PB15为推挽输出
    GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
    
    // 初始状态为低电平，蜂鸣器不响
    GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
}

/**
  * @brief  启动蜂鸣器
  * @param  无
  * @retval 无
  */
void beep_start(void)
{
    GPIO_SetBits(BEEP_PORT, BEEP_PIN);
}

/**
  * @brief  停止蜂鸣器
  * @param  无
  * @retval 无
  */
void beep_stop(void)
{
    GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
}