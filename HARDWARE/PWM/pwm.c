#include "pwm.h"


//void PWM_Init_TIM1(u16 Psc,u16 Per)
//{
//	GPIO_InitTypeDef GPIO_InitStruct;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
//	TIM_OCInitTypeDef TIM_OCInitStruct;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO,ENABLE);//����ʱ��
//	
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//��ʼ��GPIO--PA8��PA11Ϊ�����������
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8 |GPIO_Pin_11;
//	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIO_InitStruct);
//	
//	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);//��ʼ����ʱ����
//	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
//	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruct.TIM_Period=Per;
//	TIM_TimeBaseInitStruct.TIM_Prescaler=Psc;
//	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);/*��2��*///TIM2
//	
//	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;//��ʼ������Ƚ�
//	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
//	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
//	TIM_OCInitStruct.TIM_Pulse=0;
//	TIM_OC1Init(TIM1,&TIM_OCInitStruct);
//	TIM_OC4Init(TIM1,&TIM_OCInitStruct);
//	
//	//TIM_CtrlPWMOutputs(TIM1,ENABLE);//�߼���ʱ��ר��--MOE�����ʹ��
//	
//	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);/*��3��*///ENABLE//OC1Ԥװ�ؼĴ���ʹ��
//	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable);//ENABLE//OC4Ԥװ�ؼĴ���ʹ��
//	TIM_ARRPreloadConfig(TIM1,ENABLE);//TIM1��ARR��Ԥװ�ؼĴ���ʹ��
//	
//	TIM_Cmd(TIM1,ENABLE);//����ʱ����
//}



void PWM_Init_TIM3(u16 Psc,u16 Per)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��ʼ��GPIO�˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	TIM_InternalClockConfig(PWM_TIM);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//��ʼ����ʱ��
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = Per-1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = Psc-1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(PWM_TIM, &TIM_TimeBaseInitStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure);						//��ʼ������Ƚ�ģʽ
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC2Init(PWM_TIM, &TIM_OCInitStructure);
	TIM_OC1Init(PWM_TIM, &TIM_OCInitStructure);

	
	//TIM_CtrlPWMOutputs(PWM_TIM,ENABLE);				//�߼���ʱ�����뿪���ģ�MOEʹ�ܣ�
	TIM_OC1PolarityConfig(PWM_TIM,TIM_OCPreload_Enable);//OC1Ԥװ�ؼĴ���ʹ��
	TIM_OC2PolarityConfig(PWM_TIM,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(PWM_TIM,ENABLE);
	
	TIM_Cmd(PWM_TIM,ENABLE);
}


//ֱ������pwmֵ
void PWM_SetCompare1and2(uint16_t Compare)
{
	TIM_SetCompare1(PWM_TIM, Compare);
	TIM_SetCompare2(PWM_TIM, Compare);
}

