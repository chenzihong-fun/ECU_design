#ifndef __BOARD_H
#define __BOARD_H


#include <stdint.h>
#include "led.h"
#include "key.h"
#include "key_desc.h"
#include "beep.h"
#include "timer_delay.h"
#include "SG90_TIMER_PWM.h"
#include "mpu6050.h"
#include "RTC.h"
#include "st7785.h
#include "HCSR04.h"


void board_lowlevel_init(void);
void device_init(void);


extern key_desc_t *key_1;
extern key_desc_t *key_2;
extern key_desc_t *key_3;
extern key_desc_t *key_4;

#endif /* __BOARD_H */