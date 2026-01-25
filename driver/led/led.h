#ifndef LED_H
#define LED_H

#include "stdbool.h"
#include <stdint.h>
#include "led_desc.h"


void led_init(led_desc_t * led);
void led_set(led_desc_t * led , bool state);
void led_on(led_desc_t * led);
void led_off(led_desc_t * led);
void led_toggle(led_desc_t * led);


#endif /* LED_H */
