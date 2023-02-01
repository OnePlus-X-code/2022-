#ifndef __LX_FC_FUN_H
#define __LX_FC_FUN_H

//==引用
#include "SysConfig.h"

//==定义/声明

//==数据声明

//==函数声明
// static

// public

s8   Judge_Appear( u8 flag );
void Set_vel_X( s16 x_velo );
void Set_vel_Y( s16 y_velo );
void Set_vel_Z( s16 z_velo );
void Set_Dps_Yaw( s16 yaw_dps );
void Stop_Motion( void );
void Stop_Horizontal_Move( void );

u8 FC_Unlock( void );
u8 FC_Lock( void );
u8 LX_Change_Mode( u8 new_mode );
u8 FC_Turn_Left( u16 angle, u16 acc_angle );
u8 FC_Turn_Right( u16 angle, u16 acc_angle );
u8 FC_UP_DOWN( u16 Height, u8 dir, u16 speed );
u8 OneKey_Takeoff( u16 height_cm );
u8 OneKey_Land( void );
u8 OneKey_Return_Home( void );
u8 Horizontal_Calibrate( void );
u8 Horizontal_Move( u16 distance_cm, u16 velocity_cmps, u16 dir_angle_0_360 );
u8 Mag_Calibrate( void );
u8 ACC_Calibrate( void );
u8 GYR_Calibrate( void );
#endif
