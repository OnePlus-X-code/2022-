#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//��������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//����ģ��˿ڶ���
//#define AUDIO6 PFout(8)	// ����������IO 
//#define AUDIO5 PFout(7)	// ����������IO 
//#define AUDIO4 PFout(6)	// ����������IO 
//#define AUDIO3 PFout(5)	// ����������IO 
//#define AUDIO2 PFout(4)	// ����������IO 
//#define AUDIO1 PFout(3)	// ����������IO 

#define BEEP PFout(8)	// ����������IO 

void BEEP_Init(void);//��ʼ��		 

//#define AUDIO6_OFF GPIO_SetBits(GPIOF,GPIO_Pin_8)
//#define AUDIO6_ON GPIO_ResetBits(GPIOF,GPIO_Pin_8)

//#define AUDIO5_OFF GPIO_SetBits(GPIOF,GPIO_Pin_7)
//#define AUDIO5_ON GPIO_ResetBits(GPIOF,GPIO_Pin_7)

//#define AUDIO4_OFF GPIO_SetBits(GPIOF,GPIO_Pin_6)
//#define AUDIO4_ON GPIO_ResetBits(GPIOF,GPIO_Pin_6)

//#define AUDIO3_OFF GPIO_SetBits(GPIOF,GPIO_Pin_5)
//#define AUDIO3_ON GPIO_ResetBits(GPIOF,GPIO_Pin_5)

//#define AUDIO2_OFF GPIO_SetBits(GPIOF,GPIO_Pin_4)
//#define AUDIO2_ON GPIO_ResetBits(GPIOF,GPIO_Pin_4)

//#define AUDIO1_OFF GPIO_SetBits(GPIOF,GPIO_Pin_3)
//#define AUDIO1_ON GPIO_ResetBits(GPIOF,GPIO_Pin_3)


#define BEEP_OFF GPIO_SetBits(GPIOF,GPIO_Pin_8)
#define BEEP_ON GPIO_ResetBits(GPIOF,GPIO_Pin_8)

#endif

















