#include "console.h"

// USE USART1
// RX: PA10
// TX: PA9
// MODE: 8-N-1
// BAUD: 115200
// DMA: TX/RX

static void console_usart_init(void)
{
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1, ENABLE);
}

static void console_gpio_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
}


void console_init(void)
{
    console_usart_init();
    console_gpio_init();
}


void console_write(char(*str))
{
    while (*str != '\0')
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) != SET); // wait for TXE flag 等待数据加载到串口上
        USART_SendData(USART1, (uint8_t)*str);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);// wait for TC flag 等待数据发送完成
        str++;
    }
}
