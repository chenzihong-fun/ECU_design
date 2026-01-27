#include "stm32f10x.h"
#include "board.h"


// PB1按键配置
key_desc_t *key_1 = {
    .port = GPIOB,
    .pin = GPIO_Pin_1,
    .exti_port = GPIO_PortSourceGPIOB,
    .exti_pinsource = GPIO_PinSource1,
    .exti_line = EXTI_Line1,
    .exti_irq = EXTI1_IRQn
};

// PB12按键配置
key_desc_t *key_2 = {
    .port = GPIOB,
    .pin = GPIO_Pin_12,
    .exti_port = GPIO_PortSourceGPIOB,
    .exti_pinsource = GPIO_PinSource12,
    .exti_line = EXTI_Line12,
    .exti_irq = EXTI15_10_IRQn  // STM32F103中EXTI10-15共享一个中断向量
};


key_desc_t *key_3 = {
    .port = GPIOB,
    .pin = GPIO_Pin_1,
    .exti_port = GPIO_PortSourceGPIOB,
    .exti_pinsource = GPIO_PinSource1,
    .exti_line = EXTI_Line1,
    .exti_irq = EXTI1_IRQn
};

// PB12按键配置
key_desc_t * key_4 = {
    .port = GPIOB,
    .pin = GPIO_Pin_12,
    .exti_port = GPIO_PortSourceGPIOB,
    .exti_pinsource = GPIO_PinSource12,
    .exti_line = EXTI_Line12,
    .exti_irq = EXTI15_10_IRQn  // STM32F103中EXTI10-15共享一个中断向量
};
void board_lowlevel_init(void)
{
    // 使能GPIOA、GPIOB、GPIOC、GPIOD、GPIOE时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    // 使能SPI1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    // 使能USART时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

    // 使能定时器2、3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    // 使能TIM1和AFIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO, ENABLE);
    // 使能DMA1时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // 使能RTC相关使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    BKP_DeInit();
    RCC_LSEConfig(RCC_LSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);

}

void device_init(void)
{ 
    led_init();
    key_init(key_1);
    key_init(key_2);
    key_init(key_3);
    key_init(key_4);
    beep_init();
    timer_tim2_init_us(); //72MHz / 72 = 1MHz = 1us
    HCSR04_init();
    st7735_init();
}

