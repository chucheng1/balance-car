#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniSTM32F103������
//MPU6050 IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/4/18
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	   		   
//IO��������
#define MPU_SDA_IN()   {GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=8<<16;}
#define MPU_SDA_OUT()  {GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=3<<16;}

//#define MPU_SDA_IN()   {GPIOB->CRL&=0XFFFFFF0F;GPIOB->CRL|=8<<4;}
//#define MPU_SDA_OUT()  {GPIOB->CRL&=0XFFFFFF0F;GPIOB->CRL|=3<<4;}

//IO��������	 
//#define MPU_IIC_SCL     PBout(4) //SCL
//#define MPU_IIC_SDA     PBout(1) //SDA	
//#define MPU_READ_SDA   	PBin(1) //SDA	

#define MPU_IIC_SCL     PBout(1) //SCL
#define MPU_IIC_SDA     PBout(4) //SDA	
#define MPU_READ_SDA   	PBin(4) //SDA	


//IIC���в�������
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

