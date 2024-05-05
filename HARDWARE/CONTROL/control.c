#include "control.h"


float Med_Angle=-3.2;//��е��ֵ--��ʹ��С������ƽ��ס�ĽǶȡ�
float Target_Speed=3;	//�����ٶȣ���������---���ο����ӿڣ����ڿ���С��ǰ�����˼����ٶȡ�4
float Turn_Speed=120;		//�����ٶȣ�ƫ����120

#define SPEED_Y 4 //����(ǰ��)����趨�ٶ�
#define SPEED_Z 250//ƫ��(����)����趨�ٶ� 
uint8_t sign = 0;



void TIM1_UP_IRQHandler(void)
{
		if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{	
			// ����жϱ�־λ
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		//1.�ɼ�����������&MPU6050�Ƕ���Ϣ��
		Encoder_Left=Read_Speed(2);//�������԰�װ���պ����180�ȣ�Ϊ�˱������������һ�£�����Ҫ������һ��ȡ����
		Encoder_Right=Read_Speed(4);
		mpu_dmp_get_data(&Pitch,&Roll,&Yaw);			//�Ƕ�
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//������
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//���ٶ�
		
		//��������
		/*********************************************************************************************/
		/*ǰ��*/
		if((Fore==0)&&(Back==0))Target_Speed=0;//δ���ܵ�ǰ������ָ��-->�ٶ����㣬����ԭ��
		if(Fore==1)Target_Speed--;//ǰ��1��־λ����-->��Ҫǰ��
		if(Back==1)Target_Speed++;//
		Target_Speed=Target_Speed>SPEED_Y?SPEED_Y:(Target_Speed<-SPEED_Y?(-SPEED_Y):Target_Speed);//�޷�
		
		/*����*/
		if((Left==0)&&(Right==0))Turn_Speed=0;
		if(Left==1)Turn_Speed+=1;	//��ת
		if(Right==1)Turn_Speed-=1;	//��ת
		Turn_Speed=Turn_Speed>SPEED_Z?SPEED_Z:(Turn_Speed<-SPEED_Z?(-SPEED_Z):Turn_Speed);//�޷�( (20*100) * 100)
		
		/*ת��Լ��*/
		if((Left==0)&&(Right==0))Turn_Kd=-1;//��������ת��ָ�����ת��Լ��
		else if((Left==1)||(Right==1))Turn_Kd=0;//������ת��ָ����յ�����ȥ��ת��Լ��
		/*********************************************************************************************/
		//2.������ѹ��ջ������У�����������������
		

		Velocity_out=Velocity(Target_Speed,Encoder_Left ,Encoder_Right);	//�ٶȻ�
		Vertical_out=Vertical(Med_Angle,Pitch,gyroy);											//ֱ����
		Turn_out=Turn(gyroz,Turn_Speed);																	//ת��
		PWM_out=Vertical_out-Vertical_Kp*Velocity_out;//�������
		//PWM_out=PWM_out>5760?5760:(PWM_out<(-5760)?(-5760):PWM_out);
		//3.�ѿ�����������ص�����ϣ�������յĵĿ��ơ�
		MOTO1=PWM_out-Turn_out;//����
		MOTO2=PWM_out+Turn_out;//�ҵ��
		Limit(&MOTO1,&MOTO2);//PWM�޷�			
		Load(MOTO1,MOTO2);//���ص�����ϡ�
		Stop(&Med_Angle,&Pitch);//��ȫ���
		if((PWM_out > 6000)||(PWM_out <- 6000))
		{
			sign++;
			if ((sign > 20))
			{
				sign = 0;
				__set_FAULTMASK(1); //���ж�
				NVIC_SystemReset(); //��λ
			}
		}		
	}
}


/*********************
ֱ�������ǶȻ���PD��������Kp*Ek+Kd*Ek_D

��ڣ������Ƕȡ���ʵ�Ƕȡ���ʵ���ٶ�
���ڣ�ֱ�������
NOTE��

*********************/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_erect_out;
	PWM_erect_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
	return PWM_erect_out;
}

/*********************
�ٶȻ�PI��Kp*Ek+Ki*Ek_S
*********************/
int Velocity(int Target,int encoder_left,int encoder_right)
{
//	static int PWM_speed_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;//
	float a=0.6;//
	
	//1.�����ٶ�ƫ��     
	Encoder_Err=(encoder_left+encoder_right)-Target;//��ȥ���
	//2.���ٶ�ƫ����е�ͨ�˲�
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//ʹ�ò��θ���ƽ�����˳���Ƶ���ţ���ֹ�ٶ�ͻ�䡣
	EnC_Err_Lowout_last=EnC_Err_Lowout;//��ֹ�ٶȹ����Ӱ��ֱ����������������
	//3.���ٶ�ƫ����֣����ֳ�λ��
	Encoder_S+=EnC_Err_Lowout;//
	//4.�����޷�
	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
		if(stop==1)Encoder_S=0,stop=0;//���������
	//5.�ٶȻ������������
	PWM_speed_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
	return PWM_speed_out;
}


/*********************
ת�򻷣�ϵ��*Z����ٶ�+ϵ��*ң������
*********************/
int Turn(int gyro_Z,int RC)
{
	int PWM_out;
	//�ⲻ��һ���ϸ��PD��������Kd��Ե���ת���Լ������Kp��Ե���ң�ص�ת��
	PWM_out=Turn_Kd*gyro_Z + Turn_Kp*RC;
	return PWM_out;
}



