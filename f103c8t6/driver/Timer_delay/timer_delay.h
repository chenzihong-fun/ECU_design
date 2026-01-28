#ifndef __TIMER_DELAY_H__
#define __TIMER_DELAY_H__


#include <stdbool.h>
#include <stdint.h>


typedef void (*timer_elapsed_callback_t)(void);


void timer_tim2_init_us(void);
void timer_delay_us(uint32_t us);
void timer_delay_ms(uint32_t ms);
void timer_elapsed_register(timer_elapsed_callback_t callback);


#endif /* __TIMER_H__ */

