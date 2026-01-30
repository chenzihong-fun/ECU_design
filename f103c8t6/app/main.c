#include "board.h"


int main(void)
{
    board_lowlevel_init();
    device_init();
    key_set_callback(key_callback);

    while (1)
    {
        // Main loop
    }
}