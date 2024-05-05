#include "sys.h" 

/*
  ============================================================================================================================
    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
  ============================================================================================================================
   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
                         |                                   |                             |   4 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
                         |                                   |                             |   3 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
                         |                                   |                             |   2 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
                         |                                   |                             |   1 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_4  |                0-15               |            0                |   4 bits for pre-emption priority
                         |                                   |                             |   0 bits for subpriority                       
  ============================================================================================================================
*/

float datas[8];
float cmd_P,cmd_I,cmd_D;

void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//4����ռ,4����Ӧ��
	
	
		// ����TIM1�ж�
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn; // TIM1�����ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0; // �����ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; // ʹ���ж�
	NVIC_Init(&NVIC_InitStruct);

}

#if cheshi	


void simulink_test(void)
{
    static float dest_array[(SIMULINK_TX_NUMBER+1)] = {0};
    static uint32_t load_cnt = 0;
    if (load_cnt == 0) {
        load_cnt++;
        
        /* ֡ͷ */
        dest_array[0] = *(float *) "SSSS";
        /* ������ */
        dest_array[1] = (float)Pitch;				
        /* ֱ������� */                              
        dest_array[2] = (float)Vertical_out;      
        /* ����ת�� */                             
        dest_array[3] = (float)Encoder_Left;           
        /* ����ת�� */                               	
        dest_array[4] = (float)Encoder_Right;   
        /* �ٶȻ���� */                                
        dest_array[5] = (float)Velocity_out;
        /* ������ */
        dest_array[6] = (float)PWM_out;
        /* �ٶ�ƫ��*/
        dest_array[7] = (float)Encoder_Err;
        /* �ٶ�ƫ��Ļ���*/
        dest_array[8] = (float)Encoder_S;
        /*�����Ǽ��ٶ�*/
        dest_array[9] = (float)gyroy;
				usart_senddatas(USART3,(u8*)dest_array,(SIMULINK_TX_NUMBER+1)*4);
    }
    else if (load_cnt == 9) {
        load_cnt = 0;
        /* ������ */
        dest_array[0] = (float)Pitch;				
        /* ֱ������� */                              
        dest_array[1] = (float)Vertical_out;      
        /* ����ת�� */                             
        dest_array[2] =(float) Encoder_Left;           
        /* ����ת�� */                               	
        dest_array[3] = (float)Encoder_Right;   
        /* �ٶȻ���� */                                
        dest_array[4] = (float)Velocity_out;
        /* ������ */
        dest_array[5] = (float)PWM_out;
        /* �ٶ�ƫ��*/
        dest_array[6] = (float)Encoder_Err;
        /* �ٶ�ƫ��Ļ���*/
        dest_array[7] =  (float)Encoder_S;
        /*�����Ǽ��ٶ�*/
        dest_array[8] = (float)gyroy;

        dest_array[9] = *(float *) "EEEE";

       usart_senddatas(USART3,(u8*)dest_array,(SIMULINK_TX_NUMBER+1)*4);
    }
    else{
        load_cnt++;
        
        /* ������ */
        dest_array[0] = (float)Pitch;				
        /* ֱ������� */                              
        dest_array[1] = (float)Vertical_out;      
        /* ����ת�� */                             
        dest_array[2] = (float)Encoder_Left;           
        /* ����ת�� */                               	
        dest_array[3] = (float)Encoder_Right;   
        /* �ٶȻ���� */                                
        dest_array[4] = (float)Velocity_out;
        /* ������ */
        dest_array[5] = (float)PWM_out;
        /* �ٶ�ƫ��*/
        dest_array[6] = (float)Encoder_Err;
        /* �ٶ�ƫ��Ļ���*/
        dest_array[7] =  Encoder_S;
        /*�����Ǽ��ٶ�*/
        dest_array[8] = (float)gyroy;
        
       usart_senddatas(USART3,(u8*)dest_array,(SIMULINK_TX_NUMBER)*4);
    }

}


#else


#endif
