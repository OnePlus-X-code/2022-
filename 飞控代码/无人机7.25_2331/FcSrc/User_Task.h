#ifndef __USER_TASK_H
#define __USER_TASK_H

#include "SysConfig.h"
#include "Drv_Uart.h"
#include "Drv_Ctrl.h"
#include "Ano_Math.h"

// 飞行时间
extern u32 Flag_Time;
// 实时飞行高度, 更改就能控制高度
extern u8 Height;

extern u8 _Forward_Flag_;
extern u8 _Height_PID_Flag_;
extern u8 _Color_PID_Flag_;
extern u8 _Hula_PID_Flag_;

extern u8 _Circle_PID_Flag_;
extern u8 _CharacterA_PID_Flag_;
extern u8 _Position_PID_Flag_;

extern s16 TargetX, TargetY;

// 蜂鸣器计数
extern u8 Beep_Flag;

extern u8 Recording_on_cmd[2];
extern u8 Recording_off_cmd[2];
extern u8 Circle_on_cmd[2];
extern u8 Circle_off_cmd[2];
extern u8 Line_on_cmd[2];
extern u8 Line_off_cmd[2];
extern u8 Color_on_cmd[2];
extern u8 Color_off_cmd[2];
extern u8 CharacterA_on_cmd[2];
extern u8 CharacterA_off_cmd[2];
extern u8 Pillar_on_cmd[2];
extern u8 Pillar_off_cmd[2];
extern u8 Detect_on_cmd[2];
extern u8 Detect_off_cmd[2];
extern u8 Red_Square_cmd[2];
extern u8 Red_Triangle_cmd[2];
extern u8 Red_Circle_cmd[2];
extern u8 Blue_Square_cmd[2];
extern u8 Blue_Triangle_cmd[2];
extern u8 Blue_Circle_cmd[2];

extern u8 Task_State;

// ##: User define
#define NextStep                                                                         \
    {                                                                                    \
        Task_State++;                                                                    \
        Flag_Time = GetSysRunTimeMs( );                                                  \
    }

#define Beep_on  Beep_Flag = 1
#define Beep_off Beep_Flag = 0

#define Step_time       GetSysRunTimeMs( ) - Flag_Time
#define Set_Height( h ) Height = h

#define Clockwise_Rotate Clockwise_Rotate_Flag = 1;
#define CounterClockwise_Rotate CounterClockwise_Rotate_Flag = 1;
#define Stop_Rotate CounterClockwise_Rotate_Flag = Clockwise_Rotate_Flag = 0;
		
#define Motor_up 		Motor_control = 2;
#define Motor_down	Motor_control = 1;
#define Motor_stop	Motor_control = 0;

#define Position_PID_on 		_Position_PID_Flag_= 1;
#define Position_PID_off	 	_Position_PID_Flag_= 0;

#define Hula_PID_on                                                                    \
    {                                                                                    \
        _Hula_PID_Flag_ = 1;                                                           \
    }
#define Hula_PID_off                                                                   \
    {                                                                                    \
        _Hula_PID_Flag_ = 0;                                                           \
    }		
		
#define Height_PID_on                                                                    \
    {                                                                                    \
        _Height_PID_Flag_ = 1;                                                           \
    }
#define Height_PID_off                                                                   \
    {                                                                                    \
        _Height_PID_Flag_ = 0;                                                           \
    }

// ##: Circle PID onoff
#define Circle_Center_on                                                                 \
    {                                                                                    \
        _Circle_PID_Flag_ = 1;                                                           \
    }
#define Circle_Center_off                                                                \
    {                                                                                    \
        _Circle_PID_Flag_ = 0;                                                           \
        Stop_Horizontal_Move( );                                                         \
    }

// ##: Character PID onoff
#define CharacterA_Center_on                                                             \
    {                                                                                    \
        _CharacterA_PID_Flag_ = 1;                                                       \
    }
#define CharacterA_Center_off                                                            \
    {                                                                                    \
        _CharacterA_PID_Flag_ = 0;                                                       \
        Stop_Horizontal_Move( );                                                         \
    }
// ##: color PID onoff
#define Color_PID_on                                                             \
    {                                                                                    \
        _Color_PID_Flag_ = 1;                                                       \
    }
#define Color_PID_off                                                            \
    {                                                                                    \
        _Color_PID_Flag_ = 0;                                                       \
        Stop_Horizontal_Move( );                                                         \
    }
// ##: OPENMV function switch
#define Motor_on                                                                     \
    {                                                                                    \
        DrvUart1SendBuf( Motor_on_cmd, 2 );                                          \
    }
#define Recording_on                                                                     \
    {                                                                                    \
        DrvUart3SendBuf( Recording_on_cmd, 2 );                                          \
    }
#define Recording_off                                                                    \
    {                                                                                    \
        DrvUart3SendBuf( Recording_off_cmd, 2 );                                         \
    }
#define Circle_on                                                                        \
    {                                                                                    \
        DrvUart3SendBuf( Circle_on_cmd, 2 );                                             \
    }
#define Circle_off                                                                       \
    {                                                                                    \
        DrvUart3SendBuf( Circle_off_cmd, 2 );                                            \
    }
#define CharacterA_on                                                                    \
    {                                                                                    \
        DrvUart3SendBuf( CharacterA_on_cmd, 2 );                                         \
    }
#define CharacterA_off                                                                   \
    {                                                                                    \
        DrvUart3SendBuf( CharacterA_off_cmd, 2 );                                        \
    }        
#define Detect_on                                                                       \
    {                                                                                    \
        DrvUart2SendBuf( Detect_on_cmd, 2 );                                             \
    }
#define Detect_off                                                                      \
    {                                                                                    \
        DrvUart2SendBuf( Detect_off_cmd, 2 );                                            \
    }		
#define Hula_on                                                                       \
    {                                                                                    \
        DrvUart2SendBuf( Hula_on_cmd, 2 );                                             \
    }
#define Hula_off                                                                      \
    {                                                                                    \
        DrvUart2SendBuf( Hula_off_cmd, 2 );                                            \
    }	
#define Find_Red_Square 		DrvUart3SendBuf( Red_Square_cmd, 2 );
#define Find_Red_Triangle 	DrvUart3SendBuf( Red_Triangle_cmd, 2 );
#define Find_Red_Circle 		DrvUart3SendBuf( Red_Circle_cmd, 2 );
#define Find_Blue_Square 		DrvUart3SendBuf( Blue_Square_cmd, 2 );
#define Find_Blue_Triangle 	DrvUart3SendBuf( Blue_Triangle_cmd, 2 );
#define Find_Blue_Circle 		DrvUart3SendBuf( Blue_Circle_cmd, 2 );

#define Red_Square_Detected			DrvUart1SendBuf( Red_Square_Detected_cmd, 5 );
#define Red_Triangle_Detected		DrvUart1SendBuf( Red_Triangle_Detected_cmd, 5 );
#define Red_Circle_Detected			DrvUart1SendBuf( Red_Circle_Detected_cmd, 5 );
#define Blue_Square_Detected		DrvUart1SendBuf( Red_Square_Detected_cmd, 5 );
#define Blue_Triangle_Detected	DrvUart1SendBuf( Red_Triangle_Detected_cmd, 5 );
#define Blue_Circle_Detected		DrvUart1SendBuf( Red_Circle_Detected_cmd, 5 );

#define Look_Left 					DrvUart2SendBuf( Look_left_cmd, 2 );
#define Look_Forward				DrvUart2SendBuf( Look_forward_cmd, 2 );
void Find_object_in_task1(u8 target);
		
#define Red 1
#define Blue 2
		

#define Fang 1
#define Sanjiao 2
#define Yuan 3
		
typedef struct
{
		u8 colour;
		u8 shape;
		s16 X;
		s16 Y;
} subject;
		
// ##: user_task functions
void Basic_Task( void );
void User_Task0(void);
void User_Task1(void);
void User_Task2(void);
void User_Task3(void);
#endif
