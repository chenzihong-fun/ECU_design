#include "board.h"
#include "motor_control.h"
#include "hall.h"
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

	st7735_write_string(0*8, 0, distance_str, &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
}

void SG90_test()
{
    motor_init();
    motor_control();
	char speed_level[20];
	sprintf(speed_level, "speed: %u", g_motor_ctrl.speed_level);
    st7735_write_string(1*8, 0, speed_level, &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
}

void mpu6050_test()
{
	float pitch,roll,yaw;
	int tmp;
    char angle_str[20];
    if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)/*返回值为0，表示dmp处理数据正常*/
    {
        tmp = roll;
        if (tmp < 0)
        {
                tmp = -tmp;
                sprintf(angle_str, "angle-%02d.%02d", tmp/100, tmp%100);
                st7735_write_string(3*8, 40, angle_str, &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
        }
        else
        {
                sprintf(angle_str, "angle:%02d.%02d", tmp/100, tmp%100);
                st7735_write_string(3*8, 40, angle_str, &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
        }
        if(tmp>4500)/*由于tmp是*100后的数，那么roll>45度时，此时tmp值应该为4500*/
        {
                TIM_SetCompare3(TIM3,8);//表示的此时电机减速
                st7735_write_string(0*8, 20, "Caution!", &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
        }
        delay_ms(100);//延时尽量短，保证读取数据的频率足够高，否则时间长了会看不到数据变化和FIFO溢出,最高延时不能超过300ms
    }
}

void hall_test()
{
    char rpm_str[20];
    sprintf(rpm_str, "rpm: %u", hall_rpm);
    st7735_write_string(2*16, 0, rpm_str, &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
}

void test_all()
{
    HCSR04_test();
    SG90_test();
    mpu6050_test();
    hall_test();
}
