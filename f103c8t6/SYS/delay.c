#include "delay.h"

// 全局变量
static uint32_t g_fac_us = 0;  // 微秒延时系数
static uint32_t g_fac_ms = 0;  // 毫秒延时系数

/**
 * @brief  初始化SysTick定时器
 * @param  无
 * @retval 无
 */
void delay_init(void)
{
    // 系统时钟频率
    uint32_t SystemCoreClock = 72000000;  // 假设系统时钟为72MHz

    // 计算延时系数
    g_fac_us = SystemCoreClock / 1000000;  // 每微秒的计数次数
    g_fac_ms = g_fac_us * 1000;  // 每毫秒的计数次数

    // 配置SysTick定时器
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  // 使用HCLK作为时钟源
}

/**
 * @brief  微秒级延时
 * @param  us: 延时微秒数
 * @retval 无
 */
void delay_us(uint32_t us)
{
    uint32_t start = SysTick->VAL;
    uint32_t ticks = us * g_fac_us;

    // 等待指定的时间
    while ((SysTick->VAL - start) < ticks)
    {
        // 空循环
    }
}

/**
 * @brief  毫秒级延时
 * @param  ms: 延时毫秒数
 * @retval 无
 */
void delay_ms(uint32_t ms)
{
    while (ms--)
    {
        delay_us(1000);  // 1毫秒 = 1000微秒
    }
}
