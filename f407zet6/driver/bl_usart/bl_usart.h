#ifndef __BL_USART_H__
#define __BL_USART_H__

#include "stm32f4xx.h"

typedef void (*bl_callback_t)(uint8_t *data, uint16_t len);

void bl_set_rx_callback(bl_callback_t callback);
void bl_usart_init(void);
void bl_usart_write(const uint8_t *data, uint32_t size);

#endif /* __BL_USART_H__ */
