#include "stm32f4xx.h"
#include "stdio.h"
#include "board.h"

typedef enum{
    usart_print1 = 0x01,
    usart_print3 = 0x03,
}usart_print_t;

void board_lowlevel_init(void)
{
	// Configure the clocks
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    //rtc
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    RCC_LSEConfig(RCC_LSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET); // 等待 LSE 就绪
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // 配置 RTC 时钟源为 LSE

}




//定义结构体实例
//led0,led1,led2分别代表led0,led1,led2的GPIO端口、引脚、点亮和熄灭的状态
led_desc_t _led0 = {.port = GPIOE,.pin = GPIO_Pin_5,.led_on = Bit_RESET,.led_off = Bit_SET};
led_desc_t _led1 = {.port = GPIOE,.pin = GPIO_Pin_6,.led_on = Bit_RESET,.led_off = Bit_SET};
led_desc_t _led2 = {.port = GPIOC,.pin = GPIO_Pin_13,.led_on = Bit_RESET,.led_off = Bit_SET};


//定义结构体数组体指针
//led_desc_t* led_desc_ptr[3] = {&led0,&led1,&led2};

//定义函数指针类型 这里我们单个定义方便识别
led_desc_t * led0 = &_led0;
led_desc_t * led1 = &_led1;
led_desc_t * led2 = &_led2;


//定义key结构体实例
//key3,key4分别代表着板子上面的按键
key_desc_t _key3 = {.port = GPIOE,.pin = GPIO_Pin_1,.exti_line = EXTI_Line1,.exti_port = EXTI_PortSourceGPIOE,
                        .statue_pressed = Bit_RESET,.exti_irq = EXTI1_IRQn,.exti_pinsource = EXTI_PinSource1};
key_desc_t _key4 = {.port = GPIOE,.pin = GPIO_Pin_0,.exti_line = EXTI_Line0,.exti_port = EXTI_PortSourceGPIOE,
                        .statue_pressed = Bit_RESET,.exti_irq = EXTI0_IRQn,.exti_pinsource = EXTI_PinSource0};

//定义key结构体数组体指针
key_desc_t * key3 = &_key3;
key_desc_t * key4 = &_key4;

rtc_time_t _rtc_time = {25, 11, 26, 14, 23, 0} ;
rtc_time_t * rtc_time = &_rtc_time;

usart_print_t usart_print = usart_print1;

int fputc(int ch, FILE *f)
{
    if(usart_print == usart_print3)
    {
        USART_SendData(USART2, (uint8_t)ch);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
    else if(usart_print == usart_print1)
    {
        USART_SendData(USART1, (uint8_t)ch);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
    return ch;
}



// void delay_ms(uint32_t ms)
// {
//     for(uint32_t i=0; i<ms; i++)
//     {
//         for(uint32_t j=0; j<1000; j++)
//         {
//             for (uint32_t k=0; k<168; k++)
//             {
//                 __NOP();
//             }
//         }
//     }
// }
