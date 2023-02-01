#include "Motor.h"

void Motor_Init()
{
	GPIO_InitTypeDef GPIO_IS;
	GPIO_StructInit( &GPIO_IS );
	
	RCC_AHB1PeriphClockCmd( RCC_MOTOR, ENABLE );
	
  GPIO_IS.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_IS.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_IS.GPIO_OType = GPIO_OType_PP;
  GPIO_IS.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_IS.GPIO_Pin   = Pin_MOTOR1|Pin_MOTOR2|Pin_MOTOR3|Pin_MOTOR4;
  GPIO_Init( GPIO_MOTOR, &GPIO_IS );
	
	GPIO_ResetBits( GPIO_MOTOR, Pin_MOTOR1|Pin_MOTOR2|Pin_MOTOR3|Pin_MOTOR4 );
}

void WriteData(u8 data)
{
    GPIO_WriteBit(GPIO_MOTOR, Pin_MOTOR4,(data & 0x08) >> 3 ); 
    GPIO_WriteBit(GPIO_MOTOR, Pin_MOTOR3,(data & 0x04) >> 2 ); 
    GPIO_WriteBit(GPIO_MOTOR, Pin_MOTOR2,(data & 0x02) >> 1 ); 
    GPIO_WriteBit(GPIO_MOTOR, Pin_MOTOR1,(data & 0x01) ); 
 }

u8 states[8]={0x08,0x0c,0x04,0x06,0x02,0x03,0x01,0x09};
u8 Motor_control=0,Motor_state=0;
