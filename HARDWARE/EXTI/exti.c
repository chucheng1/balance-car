#include "exti.h"



void TIM1_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能TIM1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // 配置TIM1时基单元
    TIM_TimeBaseStructure.TIM_Period = 3600; // 自动重载值
    TIM_TimeBaseStructure.TIM_Prescaler = 100-1; // 预分频值为72,对应72MHz时钟
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频系数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // 使能TIM1中断
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    // 使能TIM1
    TIM_Cmd(TIM1, ENABLE);
}


