#ifndef __DRV_UWB_H
#define __DRV_UWB_H

#include "User_Task.h"
#include "stm32f4xx.h"    // ???51?????  #include <reg51.h>

typedef struct
{
    // #: Task2
		u8 flag;
		u8 XH;
		u8 XL;
		u8 YH;
		u8 YL;
} coordinate_t;



extern u8 UWB_Buf[20];
extern coordinate_t Label;

#endif


