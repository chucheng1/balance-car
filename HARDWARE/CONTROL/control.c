#include "control.h"


float Med_Angle=-3.2;//机械中值--能使得小车真正平衡住的角度。
float Target_Speed=3;	//期望速度（俯仰）。---二次开发接口，用于控制小车前进后退及其速度。4
float Turn_Speed=120;		//期望速度（偏航）120

#define SPEED_Y 4 //俯仰(前后)最大设定速度
#define SPEED_Z 250//偏航(左右)最大设定速度 
uint8_t sign = 0;



void TIM1_UP_IRQHandler(void)
{
		if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{	
			// 清除中断标志位
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		//1.采集编码器数据&MPU6050角度信息。
		Encoder_Left=Read_Speed(2);//电机是相对安装，刚好相差180度，为了编码器输出极性一致，就需要对其中一个取反。
		Encoder_Right=Read_Speed(4);
		mpu_dmp_get_data(&Pitch,&Roll,&Yaw);			//角度
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//陀螺仪
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//加速度
		
		//二开部分
		/*********************************************************************************************/
		/*前后*/
		if((Fore==0)&&(Back==0))Target_Speed=0;//未接受到前进后退指令-->速度清零，稳在原地
		if(Fore==1)Target_Speed--;//前进1标志位拉高-->需要前进
		if(Back==1)Target_Speed++;//
		Target_Speed=Target_Speed>SPEED_Y?SPEED_Y:(Target_Speed<-SPEED_Y?(-SPEED_Y):Target_Speed);//限幅
		
		/*左右*/
		if((Left==0)&&(Right==0))Turn_Speed=0;
		if(Left==1)Turn_Speed+=1;	//左转
		if(Right==1)Turn_Speed-=1;	//右转
		Turn_Speed=Turn_Speed>SPEED_Z?SPEED_Z:(Turn_Speed<-SPEED_Z?(-SPEED_Z):Turn_Speed);//限幅( (20*100) * 100)
		
		/*转向约束*/
		if((Left==0)&&(Right==0))Turn_Kd=-1;//若无左右转向指令，则开启转向约束
		else if((Left==1)||(Right==1))Turn_Kd=0;//若左右转向指令接收到，则去掉转向约束
		/*********************************************************************************************/
		//2.将数据压入闭环控制中，计算出控制输出量。
		

		Velocity_out=Velocity(Target_Speed,Encoder_Left ,Encoder_Right);	//速度环
		Vertical_out=Vertical(Med_Angle,Pitch,gyroy);											//直立环
		Turn_out=Turn(gyroz,Turn_Speed);																	//转向环
		PWM_out=Vertical_out-Vertical_Kp*Velocity_out;//最终输出
		//PWM_out=PWM_out>5760?5760:(PWM_out<(-5760)?(-5760):PWM_out);
		//3.把控制输出量加载到电机上，完成最终的的控制。
		MOTO1=PWM_out-Turn_out;//左电机
		MOTO2=PWM_out+Turn_out;//右电机
		Limit(&MOTO1,&MOTO2);//PWM限幅			
		Load(MOTO1,MOTO2);//加载到电机上。
		Stop(&Med_Angle,&Pitch);//安全检测
		if((PWM_out > 6000)||(PWM_out <- 6000))
		{
			sign++;
			if ((sign > 20))
			{
				sign = 0;
				__set_FAULTMASK(1); //关中断
				NVIC_SystemReset(); //复位
			}
		}		
	}
}


/*********************
直立环（角度环）PD控制器：Kp*Ek+Kd*Ek_D

入口：期望角度、真实角度、真实角速度
出口：直立环输出
NOTE：

*********************/
int Vertical(float Med,float Angle,float gyro_Y)
{
	int PWM_erect_out;
	PWM_erect_out=Vertical_Kp*(Angle-Med)+Vertical_Kd*(gyro_Y-0);
	return PWM_erect_out;
}

/*********************
速度环PI：Kp*Ek+Ki*Ek_S
*********************/
int Velocity(int Target,int encoder_left,int encoder_right)
{
//	static int PWM_speed_out,Encoder_Err,Encoder_S,EnC_Err_Lowout,EnC_Err_Lowout_last;//
	float a=0.6;//
	
	//1.计算速度偏差     
	Encoder_Err=(encoder_left+encoder_right)-Target;//舍去误差
	//2.对速度偏差进行低通滤波
	//low_out=(1-a)*Ek+a*low_out_last;
	EnC_Err_Lowout=(1-a)*Encoder_Err+a*EnC_Err_Lowout_last;//使得波形更加平滑，滤除高频干扰，防止速度突变。
	EnC_Err_Lowout_last=EnC_Err_Lowout;//防止速度过大的影响直立环的正常工作。
	//3.对速度偏差积分，积分出位移
	Encoder_S+=EnC_Err_Lowout;//
	//4.积分限幅
	Encoder_S=Encoder_S>10000?10000:(Encoder_S<(-10000)?(-10000):Encoder_S);
		if(stop==1)Encoder_S=0,stop=0;//清零积分量
	//5.速度环控制输出计算
	PWM_speed_out=Velocity_Kp*EnC_Err_Lowout+Velocity_Ki*Encoder_S;
	return PWM_speed_out;
}


/*********************
转向环：系数*Z轴角速度+系数*遥控数据
*********************/
int Turn(int gyro_Z,int RC)
{
	int PWM_out;
	//这不是一个严格的PD控制器，Kd针对的是转向的约束，但Kp针对的是遥控的转向。
	PWM_out=Turn_Kd*gyro_Z + Turn_Kp*RC;
	return PWM_out;
}



