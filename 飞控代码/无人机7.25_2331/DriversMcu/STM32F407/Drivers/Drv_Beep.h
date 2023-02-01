#ifndef _BEEP_H_
#define _BEEP_H_

#include "SysConfig.h"

/***************LED GPIO定义******************/
#define RCC_BEEP  RCC_AHB1Periph_GPIOC
#define GPIO_BEEP GPIOC
#define Pin_BEEP  GPIO_Pin_6
/*********************************************/

/***************AUDIO GPIO定义******************/
#define RCC_AUDIO  RCC_AHB1Periph_GPIOC
#define GPIO_AUDIO GPIOC
#define Pin_AUDIO  GPIO_Pin_7
/*********************************************/

#define Beep_Pin_On     GPIO_BEEP->BSRRL = Pin_BEEP
#define Beep_Pin_Off    GPIO_BEEP->BSRRH = Pin_BEEP
#define Beep_Pin_TOGGLE GPIO_BEEP->ODR ^= Pin_BEEP

#define Audio_Pin_Off     GPIO_AUDIO->BSRRL = GPIO_AUDIO
#define Audio_Pin_On    GPIO_AUDIO->BSRRH = GPIO_AUDIO
#define Audio_Pin_TOGGLE GPIO_AUDIO->ODR ^= GPIO_AUDIO

void Beep_Init( void );
void Audio_Init( void );

#endif

