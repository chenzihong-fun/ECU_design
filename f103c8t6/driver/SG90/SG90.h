#ifndef SG90_H__
#define SG90_H__

#include "stm32f10x.h"

#define SG90_ARR                      (20000-1)  // 20ms周期

#define SG90_STOP_DUTY                (SG90_ARR*0.075)  // 停止时占空比为 7.5% (1.5ms)

#define SG90_FRONT_MAXSPEED_DUTY      (SG90_ARR*0.025)  // 最大前进速度占空比为 2.5% (0.5ms)
#define SG90_FRONT_SPEED5_DUTY        (SG90_ARR*0.025)  // 前进挡位 5 速度占空比为 2.5% (0.5ms)
#define SG90_FRONT_SPEED4_DUTY        (SG90_ARR*0.035)  // 前进挡位 4 速度占空比为 3.5% (0.7ms)
#define SG90_FRONT_SPEED3_DUTY        (SG90_ARR*0.045)  // 前进挡位 3 速度占空比为 4.5% (0.9ms)
#define SG90_FRONT_SPEED2_DUTY        (SG90_ARR*0.055)  // 前进挡位 2 速度占空比为 5.5% (1.1ms)
#define SG90_FRONT_SPEED1_DUTY        (SG90_ARR*0.065)  // 前进挡位 1 速度占空比为 6.5% (1.3ms)

#define SG90_BACK_MAXSPEED_DUTY       (SG90_ARR*0.125)  // 最大后退速度占空比为 12.5% (2.5ms)
#define SG90_BACK_SPEED5_DUTY         (SG90_ARR*0.125)  // 后退挡位 5 速度占空比为 12.5% (2.5ms)
#define SG90_BACK_SPEED4_DUTY         (SG90_ARR*0.115)  // 后退挡位 4 速度占空比为 11.5% (2.3ms)
#define SG90_BACK_SPEED3_DUTY         (SG90_ARR*0.105)  // 后退挡位 3 速度占空比为 10.5% (2.1ms)
#define SG90_BACK_SPEED2_DUTY         (SG90_ARR*0.095)  // 后退挡位 2 速度占空比为 9.5% (1.9ms)
#define SG90_BACK_SPEED1_DUTY         (SG90_ARR*0.085)  // 后退挡位 1 速度占空比为 8.5% (1.7ms)

extern uint16_t SG90_FRONT_SPEED[4];
extern uint16_t SG90_BACK_SPEED[4];

void SG90_init(void);
void SG90_SetSpeed(uint16_t speed);

#endif /* SG90_H__ */

