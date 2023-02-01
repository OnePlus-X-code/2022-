#include "beep.h" 
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

//初始化PF8为输出口		    
//BEEP IO初始化
//PF 3 4 5 6 7 8
void BEEP_Init(void)//语音播报模块
{   
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOF时钟
  
  //初始化语音模块对应引脚GPIOF8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
	
//  //初始化语音模块对应引脚GPIOF7
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
//  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO	
//	
//	//初始化语音模块对应引脚GPIOF6
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
//  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
//	
//  //初始化语音模块对应引脚GPIOF5
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
//  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
//	
//  //初始化语音模块对应引脚GPIOF4
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
//  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
//	
//  //初始化语音模块对应引脚GPIOF3
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
//  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化GPIO
	
  GPIO_SetBits(GPIOF,GPIO_Pin_8);  //语音模块对应引脚GPIOF8拉高 
//	GPIO_SetBits(GPIOF,GPIO_Pin_7);  //语音模块对应引脚GPIOF7拉高 
//	GPIO_SetBits(GPIOF,GPIO_Pin_6);  //语音模块对应引脚GPIOF6拉高 
//	GPIO_SetBits(GPIOF,GPIO_Pin_5);  //语音模块对应引脚GPIOF5拉高 
//	GPIO_SetBits(GPIOF,GPIO_Pin_4);  //语音模块对应引脚GPIOF4拉高 
//	GPIO_SetBits(GPIOF,GPIO_Pin_3);  //语音模块对应引脚GPIOF3拉高 
}










