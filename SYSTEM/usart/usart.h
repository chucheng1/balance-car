#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 



extern char usart_readbuff[];
extern	u8 temp;



void uart3_init(u32 bound);					//串口1初始化函数
void USART3_IRQHandler(void);     	//串口1中断服务程序

void uart2_init(u32 bound);
void USART2_IRQHandler(void);  
void USART2_Send_Data(char data);
void USART2_Send_String(char *String);
void NVIC_USART_Config(void);


u8 usart_get_data(char *cmd,float *d1,float *d2,float *d3);
void usart_senddatas(USART_TypeDef* USARTx,u8* addr,int size);
#endif


