#include "mentor_control.h"

// 全局电机控制实例
motor_ctrl_t g_motor_ctrl = {
    .state = MOTOR_STATE_STOP,
    .direction = MOTOR_DIR_FORWARD,
    .speed_level = MOTOR_DEFAULT_SPEED
};

// 全局按键事件实例
key_event_t g_key_event = {
    .event_type = KEY_EVT_NONE,
    .processed = 1
};

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


// 电机控制初始化
void motor_control_init(void)
{
    g_motor_ctrl.state = MOTOR_STATE_STOP;
    g_motor_ctrl.direction = MOTOR_DIR_FORWARD;
    g_motor_ctrl.speed_level = MOTOR_DEFAULT_SPEED;
    
    g_key_event.event_type = KEY_EVT_NONE;
    g_key_event.processed = 1;
}

// 处理启动/停止按键
static void handle_start_stop_event(void)
{
    // 切换电机状态
    g_motor_ctrl.state = (g_motor_ctrl.state == MOTOR_STATE_STOP) ? MOTOR_STATE_RUN : MOTOR_STATE_STOP;
    
    // 如果是从停止状态启动，默认为正向1档
    if(g_motor_ctrl.state == MOTOR_STATE_RUN && g_motor_ctrl.speed_level == 0) {
        g_motor_ctrl.direction = MOTOR_DIR_FORWARD;
        g_motor_ctrl.speed_level = MOTOR_DEFAULT_SPEED;
    }
}

// 处理方向按键
static void handle_direction_event(void)
{
    // 切换方向，速度重置为最小速度
    g_motor_ctrl.direction = (g_motor_ctrl.direction == MOTOR_DIR_FORWARD) ? MOTOR_DIR_REVERSE : MOTOR_DIR_FORWARD;
    g_motor_ctrl.speed_level = MOTOR_DEFAULT_SPEED; // 重置为最小速度1档
}

// 处理加速按键
static void handle_speed_up_event(void)
{
    // 加速，最大到MOTOR_SPEED_LEVELS档
    if(g_motor_ctrl.speed_level < MOTOR_SPEED_LEVELS) {
        g_motor_ctrl.speed_level++;
    }
}

// 处理减速按键
static void handle_speed_down_event(void)
{
    // 减速，最小到1档
    if(g_motor_ctrl.speed_level > MOTOR_DEFAULT_SPEED) {
        g_motor_ctrl.speed_level--;
    }
}

// 处理按键事件
void handle_key_events(void)
{
    // 检查是否有未处理的按键事件
    if(!g_key_event.processed) {
        switch(g_key_event.event_type) {
            case KEY_EVT_START_STOP:
                handle_start_stop_event();
                break;
                
            case KEY_EVT_DIRECTION:
                handle_direction_event();
                break;
                
            case KEY_EVT_SPEED_UP:
                handle_speed_up_event();
                break;
                
            case KEY_EVT_SPEED_DOWN:
                handle_speed_down_event();
                break;
                
            default:
                break;
        }
        
        // 标记事件已处理
        g_key_event.processed = 1;
    }
}

// 更新电机输出
void update_motor_output(void)
{
    uint16_t target_duty = 0;
    
    if(g_motor_ctrl.state == MOTOR_STATE_RUN) {
        // 电机运行状态下，根据方向和速度设置占空比
        if(g_motor_ctrl.direction == MOTOR_DIR_FORWARD) {
            // 正转，使用正转速度数组
            target_duty = sg90_front_speed[g_motor_ctrl.speed_level - 1];
        } else {
            // 反转，使用反转速度数组
            target_duty = sg90_back_speed[g_motor_ctrl.speed_level - 1];
        }
    } else {
        // 电机停止状态下，设置占空比为中间值（停止状态）
        target_duty = 375; // 中间值，具体数值取决于您的舵机
    }
    
    // 设置SG90舵机的占空比
    extern void SG90_SetDuty(uint16_t duty);
    SG90_SetDuty(target_duty);
}

// 主循环中调用此函数处理电机控制
void process_motor_control(void)
{
    // 处理按键事件
    handle_key_events();
    
    // 更新电机输出
    update_motor_output();
}

// 旧的控制函数保留兼容性
void mentor_control(void)
{
    process_motor_control();
}

// 按键回调函数，仅设置标志位，不处理业务逻辑
void key_callback_handler(uint32_t exti_line)
{
    // 只设置按键事件，不处理业务逻辑
    switch(exti_line)
    {
        case EXTI_Line5:  // key1 - PB5 - 启动/停止电机
            g_key_event.event_type = KEY_EVT_START_STOP;
            g_key_event.processed = 0;  // 标记未处理
            break;
        case EXTI_Line6:  // key2 - PB6 - 正转/反转电机
            g_key_event.event_type = KEY_EVT_DIRECTION;
            g_key_event.processed = 0;  // 标记未处理
            break;
        case EXTI_Line7:  // key3 - PB7 - 电机加速
            g_key_event.event_type = KEY_EVT_SPEED_UP;
            g_key_event.processed = 0;  // 标记未处理
            break;
        case EXTI_Line8:  // key4 - PB8 - 电机减速
            g_key_event.event_type = KEY_EVT_SPEED_DOWN;
            g_key_event.processed = 0;  // 标记未处理
            break;
        default:
            break;
    }
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