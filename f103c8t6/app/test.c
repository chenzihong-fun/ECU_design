#include "board.h"
#include "motor_control.h"
#include "hall.h"
#include "ui.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

//屏幕字体设定为8x16   8为字体宽度（横轴），16为字体高度（纵轴）
//rtos中任务优先级的值越大，任务优先级越高
//stm32中断优先级的值越小，优先级越高

static char last_distance_str[20] = {'\0'};
static char last_angle_str[20] = {'\0'};
static char last_speed_str[20] = {'\0'};
static char last_dir_str[20] = {'\0'};
static char last_rpm_str[20] = {'\0'};


void  HCSR04_test()
{
    float distance;
    char distance_str[20];
    distance = HCSR04_MeasureDistanceFiltered();

        // --- 1. 超声波测试 ---
        {
            float distance = HCSR04_MeasureDistanceFiltered();
            char distance_str[20];

            if (distance > 0)
				{
                if (distance < 10.0)
					sprintf(distance_str, "Warning!");
                else
					sprintf(distance_str, "%.1f cm", distance);
				}
				else
				{
					sprintf(distance_str, "Error");
				}

            // 只有内容变了才刷
            if (strcmp(distance_str, last_distance_str) != 0) {
                ui_clear_line(0, 16, BACKGROUND_COLOR);
                ui_write_string(0, 0, distance_str, &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
                strcpy(last_distance_str, distance_str);
            }
        }

}

void SG90_test()
{
    motor_control();
	char speed_level[20];
    char speed_dir[20];
	sprintf(speed_level, "speed: %u", g_motor_ctrl.speed_level);
    sprintf(speed_dir, "dir: %s", g_motor_ctrl.direction == MOTOR_DIR_FORWARD ? "Forward" : "Back");

    // 只有内容变了才刷
    if (strcmp(speed_level, last_speed_str) != 0) {
        ui_clear_line(1, 16, BACKGROUND_COLOR);
        ui_write_string(0, 1*16, speed_level, &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
        strcpy(last_speed_str, speed_level);
    }

    if (strcmp(speed_dir, last_dir_str) != 0) {
        ui_clear_line(2, 16, BACKGROUND_COLOR);
        ui_write_string(0, 2*16, speed_dir, &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
        strcpy(last_dir_str, speed_dir);
    }
}

void mpu6050_test()
{
	float pitch,roll,yaw;
	int tmp;
    char angle_str[20];

    if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)/*返回值为0，表示dmp处理数据正常*/
    {
        tmp = roll * 100;
        if (tmp < 0)
        {
                tmp = -tmp;
                sprintf(angle_str, "angle-%02d", tmp/100);
        }
        else
        {
                sprintf(angle_str, "angle:%02d", tmp/100);
        }

        // 只有内容变了才刷
        if (strcmp(angle_str, last_angle_str) != 0) {
            ui_clear_line(3, 16, BACKGROUND_COLOR);
            ui_write_string(0, 3*16, angle_str, &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
            strcpy(last_angle_str, angle_str);
        }

        if(tmp>4500)/*由于tmp是*100后的数，那么roll>45度时，此时tmp值应该为4500*/
        {
            ui_clear_line(3, 16, BACKGROUND_COLOR);
            ui_write_string(0, 3*16, "Caution!", &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
        }
        timer_delay_ms(100);//延时尽量短，保证读取数据的频率足够高，否则时间长了会看不到数据变化和FIFO溢出,最高延时不能超过300ms
    }
}

void hall_test()
{
    char rpm_str[20];
    sprintf(rpm_str, "rpm: %u", hall_rpm);

    // 只有内容变了才刷
    if (strcmp(rpm_str, last_rpm_str) != 0) {
        ui_clear_line(4, 16, BACKGROUND_COLOR);
        ui_write_string(0, 4*16, rpm_str, &font_ascii_8x16, TEXT_COLOR, BACKGROUND_COLOR);
        strcpy(last_rpm_str, rpm_str);
    }
}

void test_all()
{
    HCSR04_test();
    SG90_test();
    mpu6050_test();
    hall_test();
}
