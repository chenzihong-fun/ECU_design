#include <stdint.h>
#include "stdbool.h"
#include "led.h"
#include "led_desc.h"


void led_init(led_desc_t * led)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = led->pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(led->port, &GPIO_InitStructure);

    GPIO_WriteBit(led->port, led->pin, led->led_off);
}

void led_set(led_desc_t * led , bool state)
{
    GPIO_WriteBit(led->port, led->pin, state ? led -> led_on : led -> led_off);
}

void led_on(led_desc_t * led)
{
    GPIO_WriteBit(led->port, led->pin, led->led_on);
}

void led_off(led_desc_t * led)
{
    GPIO_WriteBit(led->port, led->pin, led->led_off);
}

void led_toggle(led_desc_t * led)
{
    if (GPIO_ReadInputDataBit(led->port, led->pin) == led->led_on)
        led_off(led);
    else
        led_on(led);
}
