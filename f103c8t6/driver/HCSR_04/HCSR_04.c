#include "HCSR_04.h"
#include "timer_delay.h"
#include <stdlib.h>


//捕获到了才进入中断
//注意在使用rtos的时候避免频繁进入中断，否则可能会导致系统卡死

#define SOUND_SPEED 340u

// 定义GPIO端口和引脚
#define TRIG_PORT GPIOA
#define TRIG_PIN GPIO_Pin_12

#define ECHO_PORT GPIOA
#define ECHO_PIN GPIO_Pin_8

// 定义定时器
#define ECHO_TIM TIM1
#define ECHO_TIM_CHANNEL TIM_Channel_1

// 全局变量
static uint16_t g_echo_start_time = 0;
static uint16_t g_echo_end_time = 0;
static uint8_t g_capture_complete = 0;

static int compare_float(const void *a, const void *b)
{
    float fa = *(const float*)a;
    float fb = *(const float*)b;
    return (fa > fb) - (fa < fb);
}

/**
 * @brief  配置HCSR04的NVIC中断
 * @param  无
 * @retval 无
 */
static void HCSR04_NVIC_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    // 配置TIM1捕获中断
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief  配置HCSR04的GPIO引脚
 * @param  无
 * @retval 无
 */
static void HCSR04_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置Trigger引脚 (PA12)
    GPIO_InitStructure.GPIO_Pin = TRIG_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(TRIG_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(TRIG_PORT, TRIG_PIN);  // 初始化为低电平

    // 配置Echo引脚 (PA8)
    GPIO_InitStructure.GPIO_Pin = ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(ECHO_PORT, &GPIO_InitStructure);
}

/**
 * @brief  配置HCSR04的定时器
 * @param  无
 * @retval 无
 */
static void HCSR04_TIM_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    // 配置TIM1时间基础
    // 系统时钟=72MHz，TIM1挂载在APB2上，时钟频率为72MHz
    // 预分频器=71，计数频率=72MHz/(71+1)=1MHz
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;  // 最大计数值
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(ECHO_TIM, &TIM_TimeBaseStructure);

    // 配置TIM1输入捕获
    TIM_ICInitStructure.TIM_Channel = ECHO_TIM_CHANNEL;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  // 上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;  // 不分频
    TIM_ICInitStructure.TIM_ICFilter = 0x00;  // 无滤波
    TIM_ICInit(ECHO_TIM, &TIM_ICInitStructure);

    // 使能捕获中断
    TIM_ITConfig(ECHO_TIM, TIM_IT_CC1, ENABLE);

    // 启动TIM1
    TIM_Cmd(ECHO_TIM, ENABLE);
}

/**
 * @brief  初始化HCSR04传感器
 * @param  无
 * @retval 无
 */
void HCSR04_init(void)
{
    HCSR04_NVIC_Init();
    HCSR04_GPIO_Init();
    HCSR04_TIM_Init();
}

/**
 * @brief  发送Trigger脉冲
 * @param  无
 * @retval 无
 */
static void HCSR04_SendTrigger(void)
{
    // 发送至少10us的高电平脉冲
    GPIO_SetBits(TRIG_PORT, TRIG_PIN);
    timer_delay_us(15);  // 发送15us脉冲
    GPIO_ResetBits(TRIG_PORT, TRIG_PIN);
}

/**
 * @brief  TIM1捕获中断处理函数
 * @param  无
 * @retval 无
 */
void TIM1_CC_IRQHandler(void)
{
    static uint8_t capture_state = 0;

    // 检查是否是通道1的捕获中断
    if (TIM_GetITStatus(ECHO_TIM, TIM_IT_CC1) != RESET)
    {
        if (capture_state == 0)
        {
            // 第一个上升沿，记录开始时间
            g_echo_start_time = TIM_GetCapture1(ECHO_TIM);

            // 切换为下降沿捕获
            TIM_OC1PolarityConfig(ECHO_TIM, TIM_ICPolarity_Falling);
            capture_state = 1;
        }
        else
        {
            // 下降沿，记录结束时间
            g_echo_end_time = TIM_GetCapture1(ECHO_TIM);

            // 切换为上升沿捕获
            TIM_OC1PolarityConfig(ECHO_TIM, TIM_ICPolarity_Rising);
            capture_state = 0;

            // 标记捕获完成
            g_capture_complete = 1;
        }

        // 清除中断标志位
        TIM_ClearITPendingBit(ECHO_TIM, TIM_IT_CC1);
    }
}

/**
 * @brief  测量单次距离
 * @param  无
 * @retval 距离值(cm)，返回0表示测量失败
 */
static float HCSR04_MeasureSingle(void)
{
    uint32_t pulse_width = 0;
    float distance = -1.0f;

    // 重置捕获状态
    g_capture_complete = 0;

    // 发送Trigger脉冲
    HCSR04_SendTrigger();

    // 等待捕获完成，超时时间50ms
    uint32_t timeout = 50000;
    while (!g_capture_complete && timeout--)
    {
        timer_delay_us(1);
    }

    if (g_capture_complete)
    {
        // 计算脉冲宽度
        if (g_echo_end_time >= g_echo_start_time)
        {
            pulse_width = g_echo_end_time - g_echo_start_time;
        }
        else
        {
            // 处理计数器溢出
            pulse_width = 0xFFFF - g_echo_start_time + g_echo_end_time + 1;
        }

        // 计算距离：距离 = (声速 m/s * 时间 us) / 2 / 10000
        // 340 * pulse_width / 20000 = pulse_width / 58.82
        // 简化公式：distance = pulse_width / 58.8
        if (pulse_width > 0)
        {
            distance = (float)pulse_width / 58.8f;
        }
    }

    return distance;
}

float HCSR04_MeasureDistanceFiltered(void)
{
    #define SAMPLE_COUNT 5
    float samples[SAMPLE_COUNT];
    float valid_samples[SAMPLE_COUNT];
    int valid_count = 0;
    float result = 0.0f;

    // 1. 连续采样
    for (int i = 0; i < SAMPLE_COUNT; i++)
    {
        float dist = HCSR04_MeasureSingle();

        // 2. 剔除异常值：有效范围 2cm - 400cm
        if (dist >= 2.0f && dist <= 400.0f)
        {
            valid_samples[valid_count++] = dist;
        }

        // 采样间隔，避免余波干扰
        timer_delay_ms(5);
    }

    // 3. 如果没有有效数据，返回0
    if (valid_count == 0)
    {
        return 0.0f;
    }

    // 4. 中值滤波
    if (valid_count >= 3)
    {
        // 排序
        qsort(valid_samples, valid_count, sizeof(float), compare_float);

        // 取中值
        result = valid_samples[valid_count / 2];
    }
    else
    {
        // 数据太少，取平均值
        float sum = 0.0f;
        for (int i = 0; i < valid_count; i++)
        {
            sum += valid_samples[i];
        }
        result = sum / valid_count;
    }

    return result;
}
