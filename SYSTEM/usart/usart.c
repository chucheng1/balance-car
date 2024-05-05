//#include "usart.h"	  
#include "sys.h" 


//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   检测到中断
    USART3->DR = (u8) ch;      //一直发送
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/

void uart3_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	//USART3_TX   PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//USART3_RX	  PB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART3, ENABLE);                    //使能串口 
}

void uart2_init(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//时钟GPIOB、USART3
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//USART2_TX   PA2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//USART2_RX	  PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART2, ENABLE);                    //使能串口 
}

void NVIC_USART_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);//4级抢占,4级响应。
	
	NVIC_InitStruct.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);	
	
	NVIC_InitStruct.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStruct);	
}

//u8 Fore,Back,Left,Right;
//void USART3_IRQHandler(void) 
//{
//	
//	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//接收中断标志位拉高
//	{
//		Bluetooth_data=USART_ReceiveData(USART3);//保存接收的数据
//		
//		if(Bluetooth_data=='e')			Fore=0,Back=0,Left=0,Right=0;//刹
//		else if(Bluetooth_data=='a')Fore=1,Back=0,Left=0,Right=0;//前
//		else if(Bluetooth_data=='b')Fore=0,Back=1,Left=0,Right=0;//后
//		else if(Bluetooth_data=='c')Fore=0,Back=0,Left=1,Right=0;//左
//		else if(Bluetooth_data=='d')Fore=0,Back=0,Left=0,Right=1;//右
//		else												Fore=0,Back=0,Left=0,Right=0;//刹
//	}
////		printf("Received data: %c\n", Bluetooth_data); 
//}


void usart_senddatas(USART_TypeDef* USARTx,u8* addr,int size)
{
	while(size--) //判断数据发送完没有
	{
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);//等待上一个byte的数据发送结束。
		USART_SendData(USARTx,*addr);//调用STM32标准库函数发送数据
		addr++; //地址偏移
	}
}
/***********************************蓝牙遥控************************************/


u8 Fore,Back,Left,Right;
void USART2_IRQHandler(void) 
{
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//接收中断标志位拉高
	{
		Bluetooth_data=USART_ReceiveData(USART2);//保存接收的数据
		
		if(Bluetooth_data=='e')			Fore=0,Back=0,Left=0,Right=0;//刹
		else if(Bluetooth_data=='a')Fore=1,Back=0,Left=0,Right=0;//前
		else if(Bluetooth_data=='b')Fore=0,Back=1,Left=0,Right=0;//后
		else if(Bluetooth_data=='c')Fore=0,Back=0,Left=1,Right=0;//左
		else if(Bluetooth_data=='d')Fore=0,Back=0,Left=0,Right=1;//右
		else												Fore=0,Back=0,Left=0,Right=0;//刹
	}
		printf("Received data: %c\n", Bluetooth_data); 
}

//一个
void USART2_Send_Data(char data)
{
	USART_SendData(USART2,data);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
}

//一串
void USART2_Send_String(char *String)
{
	u16 len,j;
	
	len=strlen(String);
	for(j=0;j<len;j++)
	{
		USART2_Send_Data(*String++);
	}
}

