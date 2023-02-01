#ifndef __BEEP_H
#define __BEEP_H	 
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//蜂鸣器驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//语音模块端口定义
//#define AUDIO6 PFout(8)	// 蜂鸣器控制IO 
//#define AUDIO5 PFout(7)	// 蜂鸣器控制IO 
//#define AUDIO4 PFout(6)	// 蜂鸣器控制IO 
//#define AUDIO3 PFout(5)	// 蜂鸣器控制IO 
//#define AUDIO2 PFout(4)	// 蜂鸣器控制IO 
//#define AUDIO1 PFout(3)	// 蜂鸣器控制IO 

#define BEEP PFout(8)	// 蜂鸣器控制IO 

void BEEP_Init(void);//初始化		 

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

















