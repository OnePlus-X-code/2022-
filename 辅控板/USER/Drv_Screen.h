#ifndef __DRV_SCREEN_H
#define __DRV_SCREEN_H

#include "User_Task.h"

#include "stm32f4xx.h"    // ???51?????  #include <reg51.h>

void Screen_Data_Analysis( void );
void Screen_GetOneByte( u8 bytedata );
u8 Compare(u8 buffer1[20],u8 buffer2[20]);
void Assignment(u8 buffer1[20],u8 buffer2[20]);

typedef struct
{
    // #: Task1
		u8  num;
} task_num_t;

typedef struct
{
    // #: Task1
		u8  target1;
		u8  target2;
} task_1_t;

typedef struct
{
    // #: Task2
		u8  cmd;
} task_2_t;

extern u8 Change_Flag;

extern u8 Screen_Buf[20];
extern task_num_t TaskNum; 
extern task_1_t Task1;
extern task_2_t Task2;
extern u8 Last_Screen_Buf[20];

#endif


