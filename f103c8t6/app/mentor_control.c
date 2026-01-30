#include "mentor_control.h"

#define MONTOR_WORK 1
#define MONTOR_STOP 0
#define MONTOR_FORWARD 0
#define MONTOR_BACKWARD 1
#define MOTOR_SPEED_LEVELS 5    // 电机速度等级数
#define MOTOR_DEFAULT_SPEED 1   // 默认速度等级

// 电机速度占空比表
const uint16_t sg90_front_speed[MOTOR_SPEED_LEVELS] = {
    SG90_FRONT_SPEED1_DUTY,
    SG90_FRONT_SPEED2_DUTY,
    SG90_FRONT_SPEED3_DUTY,
    SG90_FRONT_SPEED4_DUTY,
    SG90_FRONT_MAXSPEED_DUTY
};

const uint16_t sg90_back_speed[MOTOR_SPEED_LEVELS] = {
    SG90_BACK_SPEED1_DUTY,
    SG90_BACK_SPEED2_DUTY,
    SG90_BACK_SPEED3_DUTY,
    SG90_BACK_SPEED4_DUTY,
    SG90_BACK_MAXSPEED_DUTY
};




static void motor_start_stop_toggle(void)
{
    motor_start_flag = !motor_start_flag;
}

// 电机方向切换
static void motor_direction_toggle(void)
{
    motor_direction_flag = !motor_direction_flag;
}

// 电机加速
static void motor_speed_up(void)
{
    if(motor_speed_level < 5) // 假设最大速度等级为5
    {
        motor_speed_level++;
    }
}

// 电机减速
static void motor_speed_down(void)
{
    if(motor_speed_level > 1) // 最小速度等级为1
    {
        motor_speed_level--;
    }
}

// 按键回调函数，用于处理不同按键事件
void key_callback_handler(uint32_t exti_line)
{
    switch(exti_line)
    {
        case EXTI_Line5:  // key1 - PB5 - 启动/停止电机
            motor_start_stop_toggle();
            break;
        case EXTI_Line6:  // key2 - PB6 - 正转/反转电机
            motor_direction_toggle();
            break;
        case EXTI_Line7:  // key3 - PB7 - 电机加速
            motor_speed_up();
            break;
        case EXTI_Line8:  // key4 - PB8 - 电机减速
            motor_speed_down();
            break;
        default:
            break;
    }
}


void mentor_control()
{
    
}







//key1 --- PB5  开启/关闭
//key2 --- PB6  正转/反转
//key3 --- PB7  加档
//key4 --- PB8  减档

// PB5-PB8共用EXTI9_5_IRQHandler中断处理函数
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)  // key1 - PB5
    {
        EXTI_ClearITPendingBit(EXTI_Line5);
        key_callback_handler(EXTI_Line5);
    }
    
    if (EXTI_GetITStatus(EXTI_Line6) != RESET)  // key2 - PB6
    {
        EXTI_ClearITPendingBit(EXTI_Line6);
        key_callback_handler(EXTI_Line6);
    }
    
    if (EXTI_GetITStatus(EXTI_Line7) != RESET)  // key3 - PB7
    {
        EXTI_ClearITPendingBit(EXTI_Line7);
        key_callback_handler(EXTI_Line7);
    }
    
    if (EXTI_GetITStatus(EXTI_Line8) != RESET)  // key4 - PB8
    {
        EXTI_ClearITPendingBit(EXTI_Line8);
        key_callback_handler(EXTI_Line8);
    }
}