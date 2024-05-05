#ifndef  _PWM_H
#define  _PWM_H

#include "sys.h" 



//void PWM_Init_TIM1(u16 Psc,u16 Per);



#define PWM_TIM			TIM3

void PWM_Init_TIM3(u16 Psc ,u16 Per);
void PWM_SetCompare1and2(uint16_t Compare);
//void Motor_Init(void);
//void Motor_SetSpeed(int16_t Speed);

#endif
