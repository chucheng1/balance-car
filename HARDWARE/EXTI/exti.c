#include "exti.h"



void TIM1_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

    // ʹ��TIM1ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    // ����TIM1ʱ����Ԫ
    TIM_TimeBaseStructure.TIM_Period = 3600; // �Զ�����ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = 100-1; // Ԥ��ƵֵΪ72,��Ӧ72MHzʱ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // ʱ�ӷ�Ƶϵ��
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    // ʹ��TIM1�ж�
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

    // ʹ��TIM1
    TIM_Cmd(TIM1, ENABLE);
}


