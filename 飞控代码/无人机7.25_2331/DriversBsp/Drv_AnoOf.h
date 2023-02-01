#ifndef __DRV_ANO_OF_H
#define __DRV_ANO_OF_H

// 引用
#include "SysConfig.h"

// 定义/声明

typedef struct
{
    u8 of_update_cnt;     //光流数据更新计数。
    u8 alt_update_cnt;    //高度数据更新计数。

    u8 link_sta;    //连接状态：0，未连接。1，已连接。
    u8 work_sta;    //工作状态：0，异常。1，正常

    u8 of_quality;    // 光流数据质量

    u8 of0_sta;    // 原始光流数据
    s8 of0_dx;     // 光流模块x速度信息
    s8 of0_dy;     // 光流模块y速度信息

    u8  of1_sta;
    s16 of1_dx;    // 光流模块x速度信息 融合高度信息
    s16 of1_dy;    // 光流模块y速度信息

    u8  of2_sta;
    s16 of2_dx;
    s16 of2_dy;
    s16 of2_dx_fix;
    s16 of2_dy_fix;
    s16 intergral_x;
    s16 intergral_y;
    // 惯导融合的光流数据
    // STATE：状态标记位， 0：无效，1：有效。
    // DX_2、DY_2：X、Y
    // 轴的光流信息，对应融合的移动的速度（地面速度,单位：厘米/秒）；
    // DX_FIX、DY_FIX：修正后的 X、Y
    // 轴的移动速度，适用于积分计算（地面速度,单位：厘米/秒）；
    // INTEG_X、INTEG_Y：X、Y
    // 轴的速度积分值（单纯积分，仅供参考，单位厘米，-32768~+32767 循环）。
    u32 of_alt_cm;    // #: 光流高度
    // u32 kal_of_alt_cm;    // #: 经过卡尔曼滤波后的光流高度值

    float quaternion[4];

    s16 acc_data_x;
    s16 acc_data_y;
    s16 acc_data_z;
    s16 gyr_data_x;
    s16 gyr_data_y;
    s16 gyr_data_z;

} _ano_of_st;

//飞控状态

//==数据声明
extern _ano_of_st ano_of;
//==函数声明
// static
static void AnoOF_DataAnl( uint8_t* data_buf, uint8_t num );

// public
void AnoOF_GetOneByte( uint8_t data );
void AnoOF_Check_State( float dT_s );
#endif
