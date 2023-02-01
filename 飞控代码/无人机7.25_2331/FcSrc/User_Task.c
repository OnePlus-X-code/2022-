#include "User_Task.h"
#include "Ano_Math.h"
#include "Ano_Pid.h"
#include "Ano_Scheduler.h"
#include "Drv_Beep.h"
#include "math.h"

#include "Drv_Ctrl.h"
#include "Drv_OpenMV.h"
#include "Drv_Uart.h"
#include "Motor.h"
#include "LX_FC_Fun.h"

// 飞行时间
u32 Flag_Time;
// 实时飞行高度, 更改就能控制高度
u8 Height;
// 控制PID工作
u8 _Height_PID_Flag_ = 0;

u8 _Circle_PID_Flag_     = 0;
u8 _CharacterA_PID_Flag_ = 0;
u8 _Position_PID_Flag_   = 0;

u8 _Color_PID_Flag_      = 0;
u8 _Hula_PID_Flag_ 			 = 0;

s16 TargetX=0 , TargetY=0;
// 蜂鸣器计数
u8 Beep_Flag = 0;

u8 Recording_on_cmd[2]    = { 0xAA, 0x40 };
u8 Recording_off_cmd[2]   = { 0xAA, 0x41 };

u8 Red_Square_Detected_cmd[5]				= { 0xAA, 0x13, 0x01, 0x01, 0xFF};
u8 Red_Triangle_Detected_cmd[5]			= { 0xAA, 0x13, 0x01, 0x02, 0xFF};
u8 Red_Circle_Detected_cmd[5]				= { 0xAA, 0x13, 0x01, 0x03, 0xFF};
u8 Blue_Square_Detected_cmd[5]			= { 0xAA, 0x13, 0x02, 0x01, 0xFF};
u8 Blue_Triangle_Detected_cmd[5]		= { 0xAA, 0x13, 0x02, 0x02, 0xFF};
u8 Blue_Circle_Detected_cmd[5]			= { 0xAA, 0x13, 0x02, 0x03, 0xFF};

u8 Circle_on_cmd[2]       = { 0xAA, 0x30 };
u8 Circle_off_cmd[2]      = { 0xAA, 0x31 };
u8 Detect_on_cmd[2]       = { 0xAA, 0x32 };
u8 Detect_off_cmd[2]      = { 0xAA, 0x33 };
u8 Red_Square_cmd[2]			= { 0xAA, 0x34 };
u8 Red_Triangle_cmd[2]		= { 0xAA, 0x35 };
u8 Red_Circle_cmd[2]			= { 0xAA, 0x36 };
u8 Blue_Square_cmd[2]			= { 0xAA, 0x37 };
u8 Blue_Triangle_cmd[2]		= { 0xAA, 0x38 };
u8 Blue_Circle_cmd[2]			= { 0xAA, 0x39 };

u8 Look_forward_cmd[2]		= { 0xAA, 0x22 };
u8 Look_left_cmd[2]				= { 0xAA, 0x23 };

u8 Hula_on_cmd[2]         = { 0xAA, 0x28 };
u8 Hula_off_cmd[2]				= { 0xAA, 0x29 };

u8 Stoptime=0;
// 用于判定任务的状态, 在完成任务后一定要复位, 否则将出现问题
static u8 Task_State = 0;

s16 Position_Table1[14][2] = { { -10, 325 }, { -5, 325 }, { -5, 275 }, { -10, 275 },
                               { -10, 225 }, { -5, 225 }, { -5, 175 }, { -5, 125 },
                               { -5, 75 },   { -5, 25 },  { -10, 25 }, { -10, 75 },
                               { -10, 125 }, { -10, 175 } };
s16 subjects[13][4] = { {0, 0, 0, 0},
												{Red, Sanjiao, 50, 275}, {Red, Sanjiao, 200, 125},
												{Blue, Sanjiao, 275, 200}, {Blue, Sanjiao, 350, -25},
												{Red, Yuan, 350, 275}, {Red, Yuan, 275, 50},
												{Blue, Yuan, 125, 50}, {Blue, Yuan, 125, 200},
												{Red, Fang, 50, 125}, {Red, Fang, 200, -25},
												{Blue, Fang, 200, 275}, {Blue, Fang, 350, 125}};
u8 target_shape,target_colour;

//void Basic_Task(){
//	Beep_Flag=1;
//}	
												
void Find_object_in_task1(u8 target)
{
		if ((target+1)/2 == 1) Find_Red_Triangle;//DrvUart3SendBuf( Red_Square_cmd, 2 );//
		if ((target+1)/2 == 2) Find_Blue_Triangle;
		if ((target+1)/2 == 3) Find_Red_Circle;
		if ((target+1)/2 == 4) Find_Blue_Circle;
		if ((target+1)/2 == 5) Find_Red_Square;
		if ((target+1)/2 == 6) Find_Blue_Square;
}												
void User_Task0()
	{Beep_on;}
//{	Beep_off;}
void User_Task1()//解锁5秒
{
	switch ( Task_State )
	{
		case 0:{
			if ( Ctrl_TargetOne.flag )
			{
					FC_Unlock( );
          NextStep;
			}		}
			break;
		case 1:
      if ( Step_time >= 5000 )
       {
				 Set_Height(150);
				 Height_PID_on;
				 Circle_on;
				 NextStep;
       }
       break;
		case 2:
      if ( Step_time >= 5000 && circle_feature.flag )
       {
				 Circle_Center_on;
				 NextStep;
       }
       break;
		case 3:
      if ( Step_time >= 7000 )
       {
				 Circle_Center_off;
				 Circle_off;
				 circle_feature.flag=0;
				 TargetX = subjects[Ctrl_TargetOne.target1][2];
				 TargetY = subjects[Ctrl_TargetOne.target1][3];
				 Position_PID_on;
				 Find_object_in_task1(Ctrl_TargetOne.target1);
				 NextStep;
       }
       break;
		case 4:
			if ( my_abs(TargetX-Position_Feature.X)+my_abs(TargetY-Position_Feature.Y) <= 30 )
			{
					NextStep;
			}
			break;
		case 5:
      if ( Step_time>=3000 && color_feature.flag )
       {
				 Position_PID_off;
				 Stop_Motion();
				 Color_PID_on;
				 NextStep;
       }
       break;
		case 6:
      if ( Step_time >= 5000 )
       {
				 Stop_Motion();
				 Motor_down;
				 Set_Height(80);
				 NextStep;
       }
       break;
		case 7:
      if ( Step_time >= 5000 )//下降时间
       {
				 Motor_stop;
				 Beep_on;
				 NextStep;
       }
       break;
		case 8:
      if ( Step_time >= 5000 )//鸣叫时间
       {
				 Beep_off;
				 Color_PID_off;
				 Detect_off;
				 color_feature.flag=0;
				 Motor_up;
				 Set_Height(150);
				 NextStep;
       }
       break;
		case 9:
      if ( Step_time >= 5000 )
       {
				 Motor_stop;
				 TargetX = subjects[Ctrl_TargetOne.target2][2];
				 TargetY = subjects[Ctrl_TargetOne.target2][3];
				 Position_PID_on;
				 Find_object_in_task1(Ctrl_TargetOne.target2);
				 NextStep;
       }
       break;
		case 10:
			if ( my_abs(TargetX-Position_Feature.X)+my_abs(TargetY-Position_Feature.Y) <= 30 )
			{
					NextStep;
			}	 
		case 11:
			 if ( Step_time>= 3000 && color_feature.flag )
       {
				 Stop_Motion();
				 Position_PID_off;
				 Color_PID_on;
				 NextStep;
       }
       break;
		case 12:
      if ( Step_time >= 5000 )
       {
				 Stop_Motion();
				 Motor_down;
				 Set_Height(80);
				 NextStep;
       }
       break;
		case 13:
      if ( Step_time >= 5000 )//下降时间
       {
				 Motor_stop;
				 Beep_on;
				 NextStep;
       }
       break;
		case 14:
      if ( Step_time >= 5000 )//鸣叫时间
       {
				 Motor_up;
				 Beep_off;
				 Color_PID_off;
				 Detect_off;
				 color_feature.flag=0;
				 Set_Height(150);
				 NextStep;
       }
       break;
		case 15:
      if ( Step_time >= 5000 )
       {
				 Motor_stop;
				 TargetX = 0;
				 TargetY = 0;
				 Position_PID_on;
				 Circle_on;
				 NextStep;
       }
       break;
		case 16:
      if ( circle_feature.flag && my_abs(TargetX-Position_Feature.X)+my_abs(TargetY-Position_Feature.Y) <= 30)
       {
				 Stop_Motion();
				 Position_PID_off;
				 Circle_Center_on;
				 NextStep;
       }
       break;
		case 17:
      if ( Step_time>=5000 )
       {
				 Stop_Motion();
				 Height_PID_off;
				 OneKey_Land();
				 NextStep;
       }
       break;
		case 18:
      if ( Step_time >= 5000)
       {
				 FC_Lock();
				 Working_Mode=0;
				 Task_State=0;
       }
       break;
		default:break;
	}
}

void User_Task2()//解锁5秒
{
	switch ( Task_State )
	{
		case 0:
			{
					Detect_on;
          NextStep;
			}		
			break;
		case 1:
      if ( color_feature.flag )
       {
				 if( color_feature.color == Red && color_feature.shape == Sanjiao ) Red_Triangle_Detected;
				 if( color_feature.color == Red && color_feature.shape == Fang ) Red_Square_Detected;
				 if( color_feature.color == Red && color_feature.shape == Yuan ) Red_Circle_Detected;
				 if( color_feature.color == Blue && color_feature.shape == Sanjiao ) Red_Triangle_Detected;
				 if( color_feature.color == Blue && color_feature.shape == Fang ) Blue_Square_Detected;
				 if( color_feature.color == Blue && color_feature.shape == Yuan ) Blue_Circle_Detected;
				 NextStep;
       }
       break;
		case 2:
       {
				 if(Fully_Recognized)
				 {
					target_colour=color_feature.color;
					target_shape=color_feature.shape;
					Ctrl_TargetTwo.target1=2*(target_colour+target_shape-2)+1;
					Ctrl_TargetTwo.target2=2*(target_colour+target_shape-2)+2;
					NextStep;
				 }
       }
       break;
		case 3:
			if ( Step_time>=10000 )
			{
					FC_Unlock( );
          NextStep;
			}		
			break;
		case 4:
      if ( Step_time >= 3000 )
       {
				 Set_Height(150);
				 Height_PID_on;
				 NextStep;
       }
       break;
		case 5:
      if ( Step_time >= 3000 )
       {
				 TargetX = subjects[Ctrl_TargetTwo.target1][2];
				 TargetY = subjects[Ctrl_TargetTwo.target1][3];
				 Position_PID_on;
				 Find_object_in_task1(Ctrl_TargetTwo.target1);
       }
       break;
		case 6:
      if ( my_abs(TargetX-Position_Feature.X)+my_abs(TargetY-Position_Feature.Y) <= 20 && color_feature.flag )
       {
				 Stop_Motion();
				 Position_PID_off;
				 Color_PID_on;
				 NextStep;
       }
       break;
		case 7:
      if ( Step_time >= 5000 )
       {
				 Stop_Motion();
				 Motor_down;
				 Set_Height(80);
				 NextStep;
       }
       break;
		case 8:
      if ( Step_time >= 5000 )//下降时间
       {
				 Motor_stop;
				 Beep_on;
				 NextStep;
       }
       break;
		case 9:
      if ( Step_time >= 5000 )//鸣叫时间
       {
				 Beep_off;
				 Color_PID_off;
				 Detect_off;
				 color_feature.flag=0;
				 Motor_up;
				 Set_Height(150);
				 NextStep;
       }
       break;
		case 10:
      if ( Step_time >= 5000 )
       {
				 Motor_stop;
				 TargetX = subjects[Ctrl_TargetTwo.target2][2];
				 TargetY = subjects[Ctrl_TargetTwo.target2][3];
				 Position_PID_on;
				 Find_object_in_task1(Ctrl_TargetTwo.target2);
				 NextStep;
       }
       break;
		case 11:
			 if ( my_abs(TargetX-Position_Feature.X)+my_abs(TargetY-Position_Feature.Y) <= 20 && color_feature.flag )
       {
				 Stop_Motion();
				 Position_PID_off;
				 Color_PID_on;
				 NextStep;
       }
       break;
		case 12:
      if ( Step_time >= 5000 )
       {
				 Stop_Motion();
				 Motor_down;
				 Set_Height(80);
				 NextStep;
       }
       break;
		case 13:
      if ( Step_time >= 5000 )//下降时间
       {
				 Motor_stop;
				 Beep_on;
				 NextStep;
       }
       break;
		case 14:
      if ( Step_time >= 5000 )//鸣叫时间
       {
				 Motor_up;
				 Beep_off;
				 Color_PID_off;
				 Detect_off;
				 color_feature.flag=0;
				 Set_Height(150);
				 NextStep;
       }
       break;
		case 15:
      if ( Step_time >= 5000 )
       {
				 Motor_stop;
				 TargetX = 0;
				 TargetY = 0;
				 Position_PID_on;
				 Circle_on;
				 NextStep;
       }
       break;
		case 16:
      if ( circle_feature.flag && my_abs(TargetX-Position_Feature.X)+my_abs(TargetY-Position_Feature.Y) <= 20)
       {
				 Stop_Motion();
				 Position_PID_off;
				 Circle_Center_on;
				 NextStep;
       }
       break;
		case 17:
      if ( Step_time>=5000 )
       {
				 Stop_Motion();
				 Height_PID_off;
				 OneKey_Land();
				 NextStep;
       }
       break;
		case 18:
      if ( Step_time >= 5000)
       {
				 FC_Lock();
				 Working_Mode=0;
				 Task_State=0;
       }
       break;
		default:break;
	}
}

void User_Task3()
{
	Motor_control=1;
}
															 

						

								 
	




