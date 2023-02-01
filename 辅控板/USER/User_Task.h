#ifndef __USER_TASK_H
#define __USER_TASK_H

#include "sys.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "key.h"

#include "Drv_Screen.h"

extern u8 Task1Send[6];
extern u8 Task2Send[4];
//extern u8 Task3Send[4];
extern u8 TaskNumSend[4];
extern u8 LabelSend[8];


#define Task1Trigger                   \
{                                 \
	DrvUart1SendBuf(Task1Send, 6 ); \
}

#define Task2Trigger                   \
{                                 \
	DrvUart1SendBuf(Task2Send, 4 ); \
}

//#define Task3Trigger                     \
//{                                 \
//	DrvUart1SendBuf(Task3Send, 4 ); \
//}

#define TaskNumTrigger                     \
{                                 \
	DrvUart1SendBuf(TaskNumSend, 4 ); \
}

#define LabelTrigger                     \
{                                 \
	DrvUart1SendBuf(LabelSend, 8 ); \
}

#define NVIC_GROUP NVIC_PriorityGroup_3    //中断分组选择
void System_Init( void );
void SendBufToFC(void);
#endif
