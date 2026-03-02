#include "motor_control.h"

#define MOTOR_SPEED_LEVELS 6    // 电机速度等级数
#define MOTOR_DEFAULT_SPEED 1   // 默认速度等级

key_event_t g_key_event;
motor_ctrl_t g_motor_ctrl;

// 电机速度占空比表
const uint16_t sg90_front_speed[MOTOR_SPEED_LEVELS] = {
    SG90_STOP_DUTY,
    SG90_FRONT_SPEED1_DUTY,
    SG90_FRONT_SPEED2_DUTY,
    SG90_FRONT_SPEED3_DUTY,
    SG90_FRONT_SPEED4_DUTY,
    SG90_FRONT_MAXSPEED_DUTY
};

const uint16_t sg90_back_speed[MOTOR_SPEED_LEVELS] = {
    SG90_STOP_DUTY,
    SG90_BACK_SPEED1_DUTY,
    SG90_BACK_SPEED2_DUTY,
    SG90_BACK_SPEED3_DUTY,
    SG90_BACK_SPEED4_DUTY,
    SG90_BACK_MAXSPEED_DUTY
};

void motor_init(void)
{
    g_motor_ctrl.state = MOTOR_STATE_STOP;
    g_motor_ctrl.direction = MOTOR_DIR_FORWARD;
    g_motor_ctrl.speed_level = MOTOR_DEFAULT_SPEED;

    g_key_event.event_type = KEY_EVT_NONE;
    g_key_event.need_to_processed = 0;
}

static void motor_start_stop_toggle(void)
{
    // 切换电机状态
    g_motor_ctrl.state = (g_motor_ctrl.state == MOTOR_STATE_STOP) ? MOTOR_STATE_RUN : MOTOR_STATE_STOP;
    if(g_motor_ctrl.state == MOTOR_STATE_RUN)
    {
        // 停止转为运动时，默认为正向1档
        g_motor_ctrl.direction = MOTOR_DIR_FORWARD;
        g_motor_ctrl.speed_level = MOTOR_DEFAULT_SPEED;
        TIM_SetCompare3(TIM3, sg90_front_speed[g_motor_ctrl.speed_level]);
    }
    else
    {
        // 运动转为停止
        g_motor_ctrl.state = MOTOR_STATE_STOP;
        g_motor_ctrl.speed_level = 0; // 停止状态速度等级为0
        TIM_SetCompare3(TIM3, sg90_front_speed[g_motor_ctrl.speed_level]);
    }
}

// 电机方向切换
static void motor_direction_toggle(void)
{
    g_motor_ctrl.direction = (g_motor_ctrl.direction == MOTOR_DIR_FORWARD) ? MOTOR_DIR_REVERSE : MOTOR_DIR_FORWARD;
    // 更新占空比
    if(g_motor_ctrl.direction == MOTOR_DIR_FORWARD && g_motor_ctrl.state == MOTOR_STATE_RUN)
    {
        TIM_SetCompare3(TIM3, sg90_front_speed[g_motor_ctrl.speed_level]);
    }
    else
    {
        TIM_SetCompare3(TIM3, sg90_back_speed[g_motor_ctrl.speed_level]);
    }
}

// 电机加速
static void motor_speed_up(void)
{
    if(g_motor_ctrl.speed_level < 5) // 假设最大速度等级为5
    {
        g_motor_ctrl.speed_level++;
        // 更新占空比
        if(g_motor_ctrl.direction == MOTOR_DIR_FORWARD)
        {
            TIM_SetCompare3(TIM3, sg90_front_speed[g_motor_ctrl.speed_level]);
        }
        else
        {
            TIM_SetCompare3(TIM3, sg90_back_speed[g_motor_ctrl.speed_level]);
        }
    }
}

// 电机减速
static void motor_speed_down(void)
{
    if(g_motor_ctrl.speed_level > 1) // 最小速度等级为1
    {
        g_motor_ctrl.speed_level--;
        // 更新占空比
        if(g_motor_ctrl.direction == MOTOR_DIR_FORWARD && g_motor_ctrl.state == MOTOR_STATE_RUN)
        {
            TIM_SetCompare3(TIM3, sg90_front_speed[g_motor_ctrl.speed_level]);
        }
        else
        {
            TIM_SetCompare3(TIM3, sg90_back_speed[g_motor_ctrl.speed_level]);
        }
    }
}


void f_key_callback_flag_handler(uint32_t exti_line)
{
    switch (exti_line)
    {
        case EXTI_Line5:
            g_key_event.event_type = KEY_EVT_START_STOP;
            break;
        case EXTI_Line6:
            g_key_event.event_type = KEY_EVT_DIRECTION;
            break;
        case EXTI_Line7:
            g_key_event.event_type = KEY_EVT_SPEED_UP;
            break;
        case EXTI_Line8:
            g_key_event.event_type = KEY_EVT_SPEED_DOWN;
            break;
        default:
            break;
    }
    g_key_event.need_to_processed = 1;
}

void key_event_handle (void)
{
    if (g_key_event.need_to_processed)
    {
        switch (g_key_event.event_type)
        {
            case KEY_EVT_START_STOP:
                motor_start_stop_toggle();
                break;
            case KEY_EVT_DIRECTION:
                motor_direction_toggle();
                break;
            case KEY_EVT_SPEED_UP:
                motor_speed_up();
                break;
            case KEY_EVT_SPEED_DOWN:
                motor_speed_down();
                break;
                default:
                    break;
        }
        g_key_event.need_to_processed = 0;
        g_key_event.event_type = KEY_EVT_NONE;
    }
}


void motor_control(void)
{
    key_event_handle();
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
        f_key_callback_flag_handler(EXTI_Line5);
    }

    if (EXTI_GetITStatus(EXTI_Line6) != RESET)  // key2 - PB6
    {
        EXTI_ClearITPendingBit(EXTI_Line6);
        f_key_callback_flag_handler(EXTI_Line6);
    }

    if (EXTI_GetITStatus(EXTI_Line7) != RESET)  // key3 - PB7
    {
        EXTI_ClearITPendingBit(EXTI_Line7);
        f_key_callback_flag_handler(EXTI_Line7);
    }

    if (EXTI_GetITStatus(EXTI_Line8) != RESET)  // key4 - PB8
    {
        EXTI_ClearITPendingBit(EXTI_Line8);
        f_key_callback_flag_handler(EXTI_Line8);
    }
}
