#include "stm32f10x.h"
#include "sys.h" 

float Pitch,Roll,Yaw;						//角度
short gyrox,gyroy,gyroz;				//陀螺仪--角速度
short aacx,aacy,aacz;						//加速度
int Encoder_Left,Encoder_Right;	//编码器数据（速度）

int PWM_MAX=7200,PWM_MIN=-7200;	//PWM限幅变量
int MOTO1,MOTO2;								//电机装载变量

int Vertical_out,Velocity_out,Turn_out,PWM_out;

int PWM_speed_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;
char  Bluetooth_data;

float
	Vertical_Kp=320,	//直立环KP	470*0.6=282     280
	Vertical_Kd=1.1;	//直立环Kd	0.6*0.6=0.36  		1
	
float
	Velocity_Kp=-0.8,	//速度环KP -0.56		-0.8
	Velocity_Ki=-0.01;//速度环KI -0.01		-0.005

//Kd针对的是转向的约束，但Kp针对的是遥控的转向。
float 
	Turn_Kp=-1,//-2
	Turn_Kd=-1;//-1





int main(void)	
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE ); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	delay_init();
	
	uart2_init(115200);
	uart3_init(115200);
	NVIC_Config();
	NVIC_USART_Config();
	
	OLED_Init();
	OLED_Clear();
	
	MPU_Init();
	mpu_dmp_init();

	TIM1_Config();
	Encoder_TIM2_Init();
	Encoder_TIM4_Init();
	Motor_Init();
	PWM_Init_TIM3(1,7200);

  while(1)	
	{
//		printf("Pitch=%f\r\n",Pitch);
		
		OLED_Float(0,0,Pitch,3);
		OLED_Float(2,0,Fore,2);
		OLED_Float(3,0,Back,2);
		OLED_Float(4,0,Left,2);
		OLED_Float(5,0,Right,2);
//		simulink_test();
	} 	
}

