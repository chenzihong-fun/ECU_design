#ifndef SG90_H__ 
#define SG90_H__ 

#include "stm32f10x.h"

#define SG90_ARR                      20000-1  // 20ms周期

#define SG90_STOP_DUTY                SG90_ARR*0.05  // 停止时占空比为5%

#define SG90_FRONT_MAXSPEED_DUTY      SG90_ARR*0  // 最大前进速度占空比为0%
#define SG90_FRONT_SPEED1_DUTY        SG90_ARR*0.04  // 前进挡位1速度占空比为4% 
#define SG90_FRONT_SPEED2_DUTY        SG90_ARR*0.03  // 前进挡位2速度占空比为3% 
#define SG90_FRONT_SPEED3_DUTY        SG90_ARR*0.02  // 前进挡位3速度占空比为2% 
#define SG90_FRONT_SPEED4_DUTY        SG90_ARR*0.01  // 前进挡位4速度占空比为1% 

#define SG90_BACK_MAXSPEED_DUTY       SG90_ARR*0.1  // 最大后退速度占空比为10%
#define SG90_BACK_SPEED1_DUTY         SG90_ARR*0.06  // 后退挡位1速度占空比为6% 
#define SG90_BACK_SPEED2_DUTY         SG90_ARR*0.07  // 后退挡位2速度占空比为7% 
#define SG90_BACK_SPEED3_DUTY         SG90_ARR*0.08  // 后退挡位3速度占空比为8% 
#define SG90_BACK_SPEED4_DUTY         SG90_ARR*0.09  // 后退挡位4速度占空比为9% 

extern uint16_t SG90_FRONT_SPEED[4];
extern uint16_t SG90_BACK_SPEED[4];

void SG90_Init(void);
void SG90_SetSpeed(uint16_t speed);

#endif /* SG90_H__ */

