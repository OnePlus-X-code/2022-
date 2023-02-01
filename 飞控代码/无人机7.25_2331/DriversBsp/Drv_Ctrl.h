#ifndef _DRV_CTRL_H_
#define _DRV_CTRL_H_

#include "ANO_LX.h"
#include "Drv_Sys.h"
#include "SysConfig.h"

typedef struct
{
    u8 task_num;
} Ctrl_Sgn_t;

typedef struct
{
		u8 flag;
		u8 target1;
		u8 target2;
} Ctrl_Target_t;

typedef struct
{
		u8 flag;
		s16 X;
		s16 Y;
} Ctrl_Position_t;

extern Ctrl_Sgn_t      Ctrl_Signal;
extern Ctrl_Target_t 	 Ctrl_TargetOne;
extern Ctrl_Target_t 	 Ctrl_TargetTwo;
extern u8 						 Fully_Recognized;
extern Ctrl_Position_t Position_Feature;

void Ctrl_GetOneByte( u8 bytedata );

#endif
