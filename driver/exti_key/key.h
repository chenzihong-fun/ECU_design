#ifndef __KEY_H__
#define __KEY_H__

#include "stm32f4xx.h"
#include "tim_delay.h"
#include "stdbool.h"
#include "key_desc.h"

typedef void (*key_callback_t) (void);

void key_init(key_desc_t* key);
bool key_pressed(key_desc_t* key);
void key_wait_released(key_desc_t* key);
bool key_long_pressed(key_desc_t* key);
void key_set_callback(key_desc_t* key, key_callback_t callback);

#endif
