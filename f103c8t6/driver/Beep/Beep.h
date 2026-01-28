#ifndef __BEEP_H_
#define __BEEP_H_

#include "stm32f10x.h"

void beep_init(void);
void beep_start(void);
void beep_stop(void);
//void beep_set_freq(uint16_t freq);


#endif // !__BEEP_H_
