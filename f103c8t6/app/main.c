#include "board.h"

extern void test_all();

int main(void)
{
    board_lowlevel_init();
    device_init();

    while (1)
    {
        test_all();
        // Main loop
    }
}
