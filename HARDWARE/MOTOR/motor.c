#include "motor.h"

/*电机初始化函数*/
void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//开启时钟
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//初始化GPIO--PB12、PB13、PB14、PB15为推挽输出
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12 |GPIO_Pin_13 |GPIO_Pin_14 |GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);	
}

/*限幅函数*/
void Limit(int *motoA,int *motoB)
{
	if(*motoA>PWM_MAX)*motoA=PWM_MAX;
	if(*motoA<PWM_MIN)*motoA=PWM_MIN;
	
	if(*motoB>PWM_MAX)*motoB=PWM_MAX;
	if(*motoB<PWM_MIN)*motoB=PWM_MIN;
}

/*绝对值函数*/
int GFP_abs(int p)
{
	int q;
	q=p>0?p:(-p);
	return q;
}

/*赋值函数*/
/*入口参数：PID运算完成后的最终PWM值*/
void Load(int moto1,int moto2)//moto1=-200：反转200个脉冲
{
	//1.研究正负号，对应正反转
	if(moto1>0)	Ain1=1,Ain2=0;//正转
	else 				Ain1=0,Ain2=1;//反转
	//2.研究PWM值
	TIM_SetCompare1(TIM3,GFP_abs(moto1)+50);//电机加上死区300
	
	if(moto2>0)	Bin1=1,Bin2=0;
	else 				Bin1=0,Bin2=1;	
	TIM_SetCompare2(TIM3,GFP_abs(moto2)+100);//电机加上死区300
}


char PWM_Zero=0,stop=0;
void Stop(float *Med_Jiaodu,float *Jiaodu)
{
	if(GFP_abs(*Jiaodu+*Med_Jiaodu)>70)
	{
		Load(PWM_Zero,PWM_Zero);
		stop=1;
	}
}

