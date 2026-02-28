#include "board.h"
#include "motor_control.h"
#include <stdio.h>

void  HCSR04_test()
{
    float distance;
    char distance_str[20];
    distance = HCSR04_MeasureDistance();

    if (distance > 0)
    {
        sprintf(distance_str, "%.1f cm", distance);
    }
    else
    {
        sprintf(distance_str, "Error");
    }

	st7735_write_string(0, 0, distance_str, &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
}

void SG90_test()
{
    motor_init ();
    motor_control();
	char speed_level[20];
	sprintf(speed_level, "speed: %u", g_motor_ctrl.speed_level);
    st7735_write_string(8, 0, speed_level, &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
}

void mpu6050_test()
{

}

void hall_test()
{

}

void test_all()
{
    HCSR04_test();
    SG90_test();
    mpu6050_test();
    hall_test();
}
