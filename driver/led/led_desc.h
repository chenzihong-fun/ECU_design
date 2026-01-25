#ifndef LED_DESC_H_
#define LED_DESC_H_

#include "stm32f4xx.h"

//此头文件用于定义与结构体相关的内容

//定义led_desc_t结构体
typedef struct{
    GPIO_TypeDef* port;
    uint16_t pin;
    BitAction led_on;
    BitAction led_off;
}led_desc_t;



#endif /* LED_DESC_H_ */
