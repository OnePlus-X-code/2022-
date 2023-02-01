#ifndef __DRV_FC_H
#define __DRV_FC_H

#include "User_Task.h"

#include "stm32f4xx.h"    // ???51?????  #include <reg51.h>

void FC_Data_Analysis( void );
void FC_GetOneByte( u8 bytedata );

typedef struct
{
    // #: Task2
		u8  color;
		u8  shape;
} object_feature_t;

typedef struct
{
    // #: Task2
		u8  flag;
} object_flag_t;

extern u8 FC_buf[20];
extern u8 Last_FC_buf[20];
extern object_feature_t ObjectA;
extern object_flag_t FlagA;
extern u8 FC_Change_Flag;

#endif

