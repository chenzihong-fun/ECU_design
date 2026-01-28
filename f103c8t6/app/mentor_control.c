#include "mentor_control.h"

void mentor_control()
{
    
}





















//key1 --- PB5  开启/关闭
//key2 --- PB6  正转/反转
//key3 --- PB7  加档
//key4 --- PB8  减档

// PB5-PB8都属于EXTI_Line5-9，共享EXTI9_5_IRQHandler中断处理函数
void EXTI9_5_IRQHandler(void)  // PB5-PB8对应的中断处理函数
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)  // key1 - PB5
    {
        EXTI_ClearITPendingBit(EXTI_Line5);
        if (key_callback)
        {
            key_callback();  // 可扩展为传递按键ID参数
        }
    }
    
    if (EXTI_GetITStatus(EXTI_Line6) != RESET)  // key2 - PB6
    {
        EXTI_ClearITPendingBit(EXTI_Line6);
        if (key_callback)
        {
            key_callback();  // 可扩展为传递按键ID参数
        }
    }
    
    if (EXTI_GetITStatus(EXTI_Line7) != RESET)  // key3 - PB7
    {
        EXTI_ClearITPendingBit(EXTI_Line7);
        if (key_callback)
        {
            key_callback();  // 可扩展为传递按键ID参数
        }
    }
    
    if (EXTI_GetITStatus(EXTI_Line8) != RESET)  // key4 - PB8
    {
        EXTI_ClearITPendingBit(EXTI_Line8);
        if (key_callback)
        {
            key_callback();  // 可扩展为传递按键ID参数
        }
    }
}