#ifndef __BOARD_H
#define __BOARD_H

#include <stdio.h>
#include <stdint.h>
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "key_desc.h"
#include "beep.h"
#include "timer_delay.h"
#include "SG90.h"
#include "mpu6050.h"
#include "st7735.h"
#include "HCSR_04.h"
#include "motor_control.h"
#include "hall.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "console_usart1.h"
#include "delay.h"


extern key_desc_t * key_1;
extern key_desc_t * key_2;
extern key_desc_t * key_3;
extern key_desc_t * key_4;


void board_lowlevel_init(void);
void device_init(void);


extern key_desc_t *key_1;
extern key_desc_t *key_2;
extern key_desc_t *key_3;
extern key_desc_t *key_4;

#endif /* __BOARD_H */

