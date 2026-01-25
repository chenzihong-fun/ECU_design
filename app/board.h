#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "stdio.h"

// RTC时间结构体定义
typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} rtc_time_t;

#include "led.h"
#include "led_desc.h"
#include "key.h"
#include "key_desc.h"
#include "tim_delay.h"
#include "console.h"
#include "bl_usart.h"

void board_lowlevel_init(void);

extern led_desc_t *led0;
extern led_desc_t *led1;
extern led_desc_t *led2;

extern key_desc_t *key3;
extern key_desc_t *key4;

extern rtc_time_t * rtc_time;
#endif // BOARD_H
