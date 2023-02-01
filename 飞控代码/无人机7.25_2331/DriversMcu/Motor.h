#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "SysConfig.h"

#define RCC_MOTOR RCC_AHB1Periph_GPIOD

#define GPIO_MOTOR GPIOD

#define Pin_MOTOR1 GPIO_Pin_12
#define Pin_MOTOR2 GPIO_Pin_13
#define Pin_MOTOR3 GPIO_Pin_14
#define Pin_MOTOR4 GPIO_Pin_15

extern u8 states[8];
extern u8 Motor_control,Motor_state;

void Motor_Init(void);
void WriteData(u8 data);

#endif
