#ifndef __HALL_H__
#define __HALL_H__

#include "stm32f10x.h"


// 外部变量声明
extern volatile uint32_t hall_rpm;

// 函数声明
void hall_init(void);
void hall_timer_init(void);
uint32_t hall_calculate_rpm(void);

#endif /* __HALL_H__ */
