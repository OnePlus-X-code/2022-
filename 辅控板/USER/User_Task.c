#include "User_Task.h"
#include <stdarg.h>
#include "stm32f4xx.h"

//#include "Drv_BlueTooth.h"
//#include "Drv_Wave.h"
#include "timer.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "Drv_Uwb.h"
#include "Drv_FC.h"
#include "delay.h"

u8 Task1Send[6]={0xAA,0x11,0x01,0x00,0x00,0xFF};
u8 Task2Send[4]={0xAA,0x12,0x00,0xFF};
//u8 Task3Send[4]={0xAA,0x10,0x03,0xFF};
u8 TaskNumSend[4]={0xAA,0x10,0x01,0xFF};
u8 LabelSend[8]={0xAA,0x13,0x01,0x00,0x00,0x00,0x00,0xFF};
u8 hexEND[3]={0xFF,0xFF,0xFF};


#define TX_BUF_LEN  256     /* 发送缓冲区容量，根据需要进行调整 */
uint8_t TxBuf[TX_BUF_LEN];  /* 发送缓冲区                       */
//void MyPrintf(const char *__format, ...)
//{
//  va_list ap;
//  va_start(ap, __format);
//    
//  /* 清空发送缓冲区 */
//  memset(TxBuf, 0x0, TX_BUF_LEN);
//    
//  /* 填充发送缓冲区 */
//  vsnprintf((char*)TxBuf, TX_BUF_LEN, (const char *)__format, ap);
//  va_end(ap);
//  int len = strlen((const char*)TxBuf);
//  
//  /* 往串口发送数据 */
//  for (int i = 0; i < len; i++)
//  {
// while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);    
// USART_SendData(USART2, TxBuf[i]);
//  }
//}




void System_Init( )
{
    NVIC_SetPriorityGrouping( NVIC_GROUP );
		//SysTick_Init();
    Sys_Timer_Init( );

    // #: Fly Control
    DrvUart1Init( 921600 );
    // #: Screen
    DrvUart2Init( 115200 );
    // #: UtrSound
    DrvUart3Init( 115200 );
		//初始化与LED连接的硬件接口
		LED_Init();
		BEEP_Init();
		KEY_Init();
}

u8 Mission_Start=0;
u8 First_Time_Flag=0;
u8 Wait_Flag=0;

void CheckObject(void){
	u8 times,color;
	u8 i=0;
	if((ObjectA.shape==0x01||ObjectA.shape==0x02||ObjectA.shape==0x03)&&(ObjectA.color==0x01||ObjectA.color==0x02)){
	times=ObjectA.shape;
	color=ObjectA.color;
	}
	else {
	times=0;
	color=0;		
	}
	BEEP_OFF;
	while(i<times){
		BEEP_ON;LED_TOGGLE;
		delay_ms(500);
		BEEP_OFF;LED_TOGGLE;
		delay_ms(500);
		i++;
	}
	BEEP_OFF;
	LED_OFF;
	if(color==1)LED_RED_ON;
	else if(color==2)LED_BLUE_ON;
}

void SendBufToFC(){
	//CheckObject();
	//LED_OFF;
if(Mission_Start==1){
	if(First_Time_Flag){
		delay_ms(500);
		First_Time_Flag=0;
	}
		delay_ms(100);
		LabelSend[3]=Label.XH;
		LabelSend[4]=Label.XL;
		LabelSend[5]=Label.YH;
		LabelSend[6]=Label.YL;
		LabelTrigger;
	}

if((Screen_Buf[1]==0x12&&Task2.cmd==0x01)&&Change_Flag==0){
			if(Wait_Flag==1){
			CheckObject();
			
			Wait_Flag=0;
			Task2.cmd=0x00;
			}
			else if(Wait_Flag==0){
				Wait_Flag=1;
			}
}
if(Change_Flag==1){
	//LED_ON;
	//LED_RED_ON;
	//LED_BLUE_OFF;
	if(Screen_Buf[1]==0x10){
		TaskNumSend[2]=TaskNum.num;
		TaskNumTrigger;
		Change_Flag=0;
		if(Screen_Buf[2]==0x03||Screen_Buf[2]==0x04){
			Mission_Start=1;
			First_Time_Flag=1;
		}
		else if(Screen_Buf[2]==0x02){
			Mission_Start=0;
			if((FC_buf[2]==0x01||FC_buf[2]==0x02)&&(FC_buf[3]==0x01||FC_buf[3]==0x02||FC_buf[3]==0x03)){
				CheckObject();
				Change_Flag=0;
			}
			else Change_Flag=1;
		}
		else Mission_Start=0;
		Assignment(Last_Screen_Buf,Screen_Buf);
	}
	else if(Screen_Buf[1]==0x11){
		Task1Send[3]=Screen_Buf[3];
		Task1Send[4]=Screen_Buf[4];
		Task1Trigger;
		Change_Flag=0;
		Mission_Start=1;
		First_Time_Flag=1;
		Assignment(Last_Screen_Buf,Screen_Buf);
		}
	else if(Screen_Buf[1]==0x12){
		Task2Send[2]=Task2.cmd;
		Task2Trigger;
		Change_Flag=0;
		if(Task2.cmd==0x02){
			Mission_Start=1;
			First_Time_Flag=1;
			//Assignment(Last_Screen_Buf,Screen_Buf);
		}
		else if(Task2.cmd==0x01) {
			Mission_Start=0;
			//Screen_Buf[2]=0x00;
			//Task2.cmd=0x00;
			Assignment(Last_FC_buf,FC_buf);
		}
		Assignment(Last_Screen_Buf,Screen_Buf);
		}
	}
//	if(Cancel_Flag==1){
//		CheckObject();
//		Assignment(Last_FC_buf,FC_buf);
//	}

}





	
