#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <stdint.h>
#include "stm32f4xx.h"
#include "console.h"

void console_init(void);
void console_write(char(*str));

#endif /* CONSOLE_H_ */
