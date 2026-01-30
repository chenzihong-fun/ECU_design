#ifndef __MENTOR_CONTROL_H__
#define __MENTOR_CONTROL_H__


#include "SG90.h"
#include "board.h"

#define MOTOR_SPEED_LEVELS 5    // 电机速度等级数
#define MOTOR_DEFAULT_SPEED 1   // 默认速度等级

// 电机状态枚举
typedef enum {
    MOTOR_STATE_STOP = 0,      // 停止状态
    MOTOR_STATE_RUN            // 运行状态
} motor_state_t;

// 电机方向枚举
typedef enum {
    MOTOR_DIR_FORWARD = 0,     // 正转
    MOTOR_DIR_REVERSE          // 反转
} motor_direction_t;

// 按键类型枚举
typedef enum {
    KEY_EVT_NONE = 0,          // 无按键
    KEY_EVT_START_STOP,        // 启动/停止按键
    KEY_EVT_DIRECTION,         // 方向按键
    KEY_EVT_SPEED_UP,          // 加速按键
    KEY_EVT_SPEED_DOWN         // 减速按键
} key_event_type_t;

// 电机控制结构体
typedef struct {
    volatile motor_state_t state;          // 电机状态
    volatile motor_direction_t direction;  // 电机方向
    volatile uint8_t speed_level;          // 速度等级 (1-MOTOR_SPEED_LEVELS)
} motor_ctrl_t;

// 按键事件结构体
typedef struct {
    volatile key_event_type_t event_type;  // 按键事件类型
    volatile uint8_t processed;            // 事件是否已处理
} key_event_t;

// 电机控制实例
extern motor_ctrl_t g_motor_ctrl;

// 按键事件实例
extern key_event_t g_key_event;

// SG90速度占空比定义（这些值可能需要根据实际情况调整）
#define SG90_FRONT_SPEED1_DUTY  300
#define SG90_FRONT_SPEED2_DUTY  350
#define SG90_FRONT_SPEED3_DUTY  400
#define SG90_FRONT_SPEED4_DUTY  450
#define SG90_FRONT_MAXSPEED_DUTY 500

#define SG90_BACK_SPEED1_DUTY   250
#define SG90_BACK_SPEED2_DUTY   200
#define SG90_BACK_SPEED3_DUTY   150
#define SG90_BACK_SPEED4_DUTY   100
#define SG90_BACK_MAXSPEED_DUTY 50

extern const uint16_t sg90_front_speed[MOTOR_SPEED_LEVELS];
extern const uint16_t sg90_back_speed[MOTOR_SPEED_LEVELS];

// 电机控制函数
void motor_control_init(void);
void process_motor_control(void);              // 主循环中调用此函数处理电机控制
void handle_key_events(void);                 // 处理按键事件
void update_motor_output(void);               // 更新电机输出
void mentor_control(void);                    // 旧的控制函数保留兼容性

#endif /* __MENTOR_CONTROL_H__ */