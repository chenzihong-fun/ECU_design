#ifndef KEY_DESC_H_
#define KEY_DESC_H_

#include "stm32f4xx.h"

//定义key_desc_t结构体
typedef struct{
    GPIO_TypeDef* port;
    uint16_t pin;
    uint32_t exti_line;
    uint8_t exti_port;
    BitAction statue_pressed;
    uint8_t exti_irq;   //EXTI线对应的中断向量号 数据类型为uint8_t
    uint8_t exti_pinsource;
}key_desc_t;

#endif /* KEY_DESC_H_ */
