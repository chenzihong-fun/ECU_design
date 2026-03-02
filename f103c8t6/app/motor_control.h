#ifndef __MOTOR_CONTROL_H__
#define __MOTOR_CONTROL_H__


#include "SG90.h"
#include "board.h"

typedef enum {
    MOTOR_STATE_STOP = 0,
    MOTOR_STATE_RUN = 1
} motor_state_t;

typedef enum{
    MOTOR_DIR_FORWARD = 0,
    MOTOR_DIR_REVERSE = 1
} motor_dir_t;

 typedef struct{
    volatile motor_state_t state;
    volatile motor_dir_t direction;
    volatile uint8_t speed_level;
 } motor_ctrl_t;

 typedef enum{
    KEY_EVT_NONE = 0,
    KEY_EVT_DIRECTION,
    KEY_EVT_SPEED_DOWN,
    KEY_EVT_SPEED_UP,
    KEY_EVT_START_STOP
 } key_event_type_t;

 typedef struct{
    volatile key_event_type_t event_type;
    volatile uint8_t need_to_processed;
 } key_event_t;

extern key_event_t g_key_event;
extern motor_ctrl_t g_motor_ctrl;

void motor_init(void);
void motor_control(void);


#endif /* __motor_CONTROL_H__ */
