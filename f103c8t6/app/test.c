#include "board.h"
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
    
    st7735_write_string(0, 0, "Distance:", &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
    st7735_write_string(0, 17, distance_str, &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
}

void SG90_test()
{

}

void mpu6050_test()
{

}