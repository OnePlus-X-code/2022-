#ifndef _TIME_H_
#define _TIME_H_

#include "stm32f4xx.h"

#define NVIC_TIME_P 7    //∂® ±∆˜÷–∂œ≈‰÷√
#define NVIC_TIME_S 1

#define Rcev_TIMx_IRQn TIM7_IRQn
#define Recv_TIMx      TIM7
#define Rcev_RCC_TIMx  RCC_APB1Periph_TIM7


#define Send_TIMx_IRQn TIM4_IRQn
#define Send_TIMx      TIM4
#define Send_RCC_TIMx  RCC_APB1Periph_TIM4


void Sys_Timer_Init( void );

#endif

