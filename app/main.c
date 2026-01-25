#include "stm32f4xx.h"                  // Device header
#include "board.h"

#define APP_BASE_ADDRESSS    0x8010000

#define PACKET_BUFFER_SIZE   4096

uint8_t packet_buffer[PACKET_BUFFER_SIZE];
uint16_t packet_pointer = 0;

void packet_handler(uint8_t* data, uint16_t size)
{
    packet_buffer[packet_pointer++]=*data;
    if(packet_pointer == PACKET_BUFFER_SIZE)
    {
        // process the packet
        packet_pointer = 0;
    }
}


int main(void)
{
    board_lowlevel_init();
    // device_init();
    led_init(led0);
	tim_delay_init();
    key_init(key4);
    // rtc_init();
    console_init();
	bl_usart_init();
}
