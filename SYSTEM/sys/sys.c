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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//4级抢占,4级响应。
	
	
		// 配置TIM1中断
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn; // TIM1更新中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0; // 子优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE; // 使能中断
	NVIC_Init(&NVIC_InitStruct);

}

#if cheshi	


void simulink_test(void)
{
    static float dest_array[(SIMULINK_TX_NUMBER+1)] = {0};
    static uint32_t load_cnt = 0;
    if (load_cnt == 0) {
        load_cnt++;
        
        /* 帧头 */
        dest_array[0] = *(float *) "SSSS";
        /* 俯仰角 */
        dest_array[1] = (float)Pitch;				
        /* 直立环输出 */                              
        dest_array[2] = (float)Vertical_out;      
        /* 左轮转速 */                             
        dest_array[3] = (float)Encoder_Left;           
        /* 右轮转速 */                               	
        dest_array[4] = (float)Encoder_Right;   
        /* 速度环输出 */                                
        dest_array[5] = (float)Velocity_out;
        /* 最后输出 */
        dest_array[6] = (float)PWM_out;
        /* 速度偏差*/
        dest_array[7] = (float)Encoder_Err;
        /* 速度偏差的积累*/
        dest_array[8] = (float)Encoder_S;
        /*俯仰角加速度*/
        dest_array[9] = (float)gyroy;
				usart_senddatas(USART3,(u8*)dest_array,(SIMULINK_TX_NUMBER+1)*4);
    }
    else if (load_cnt == 9) {
        load_cnt = 0;
        /* 俯仰角 */
        dest_array[0] = (float)Pitch;				
        /* 直立环输出 */                              
        dest_array[1] = (float)Vertical_out;      
        /* 左轮转速 */                             
        dest_array[2] =(float) Encoder_Left;           
        /* 右轮转速 */                               	
        dest_array[3] = (float)Encoder_Right;   
        /* 速度环输出 */                                
        dest_array[4] = (float)Velocity_out;
        /* 最后输出 */
        dest_array[5] = (float)PWM_out;
        /* 速度偏差*/
        dest_array[6] = (float)Encoder_Err;
        /* 速度偏差的积累*/
        dest_array[7] =  (float)Encoder_S;
        /*俯仰角加速度*/
        dest_array[8] = (float)gyroy;

        dest_array[9] = *(float *) "EEEE";

       usart_senddatas(USART3,(u8*)dest_array,(SIMULINK_TX_NUMBER+1)*4);
    }
    else{
        load_cnt++;
        
        /* 俯仰角 */
        dest_array[0] = (float)Pitch;				
        /* 直立环输出 */                              
        dest_array[1] = (float)Vertical_out;      
        /* 左轮转速 */                             
        dest_array[2] = (float)Encoder_Left;           
        /* 右轮转速 */                               	
        dest_array[3] = (float)Encoder_Right;   
        /* 速度环输出 */                                
        dest_array[4] = (float)Velocity_out;
        /* 最后输出 */
        dest_array[5] = (float)PWM_out;
        /* 速度偏差*/
        dest_array[6] = (float)Encoder_Err;
        /* 速度偏差的积累*/
        dest_array[7] =  Encoder_S;
        /*俯仰角加速度*/
        dest_array[8] = (float)gyroy;
        
       usart_senddatas(USART3,(u8*)dest_array,(SIMULINK_TX_NUMBER)*4);
    }

}


#else


#endif
