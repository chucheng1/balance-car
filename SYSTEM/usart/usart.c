//#include "usart.h"	  
#include "sys.h" 


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   ��⵽�ж�
    USART3->DR = (u8) ch;      //һֱ����
	return ch;
}
#endif 

/*ʹ��microLib�ķ���*/
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
	//GPIO�˿�����
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
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
}

void uart2_init(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʱ��GPIOB��USART3
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
	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
}

void NVIC_USART_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);//4����ռ,4����Ӧ��
	
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
//	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)//�����жϱ�־λ����
//	{
//		Bluetooth_data=USART_ReceiveData(USART3);//������յ�����
//		
//		if(Bluetooth_data=='e')			Fore=0,Back=0,Left=0,Right=0;//ɲ
//		else if(Bluetooth_data=='a')Fore=1,Back=0,Left=0,Right=0;//ǰ
//		else if(Bluetooth_data=='b')Fore=0,Back=1,Left=0,Right=0;//��
//		else if(Bluetooth_data=='c')Fore=0,Back=0,Left=1,Right=0;//��
//		else if(Bluetooth_data=='d')Fore=0,Back=0,Left=0,Right=1;//��
//		else												Fore=0,Back=0,Left=0,Right=0;//ɲ
//	}
////		printf("Received data: %c\n", Bluetooth_data); 
//}


void usart_senddatas(USART_TypeDef* USARTx,u8* addr,int size)
{
	while(size--) //�ж����ݷ�����û��
	{
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);//�ȴ���һ��byte�����ݷ��ͽ�����
		USART_SendData(USARTx,*addr);//����STM32��׼�⺯����������
		addr++; //��ַƫ��
	}
}
/***********************************����ң��************************************/


u8 Fore,Back,Left,Right;
void USART2_IRQHandler(void) 
{
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)//�����жϱ�־λ����
	{
		Bluetooth_data=USART_ReceiveData(USART2);//������յ�����
		
		if(Bluetooth_data=='e')			Fore=0,Back=0,Left=0,Right=0;//ɲ
		else if(Bluetooth_data=='a')Fore=1,Back=0,Left=0,Right=0;//ǰ
		else if(Bluetooth_data=='b')Fore=0,Back=1,Left=0,Right=0;//��
		else if(Bluetooth_data=='c')Fore=0,Back=0,Left=1,Right=0;//��
		else if(Bluetooth_data=='d')Fore=0,Back=0,Left=0,Right=1;//��
		else												Fore=0,Back=0,Left=0,Right=0;//ɲ
	}
		printf("Received data: %c\n", Bluetooth_data); 
}

//һ��
void USART2_Send_Data(char data)
{
	USART_SendData(USART2,data);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=1);
}

//һ��
void USART2_Send_String(char *String)
{
	u16 len,j;
	
	len=strlen(String);
	for(j=0;j<len;j++)
	{
		USART2_Send_Data(*String++);
	}
}

