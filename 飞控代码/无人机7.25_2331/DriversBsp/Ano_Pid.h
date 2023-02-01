#ifndef __ANO_PID_H
#define __ANO_PID_H
#include "SysConfig.h"
#include "User_Task.h"

// ##: 参数结构体
typedef struct
{
    u8    fb_d_mode;
    float kp;       // 比例系数
    float ki;       // 积分系数
    float kd_ex;    // 微分系数
    float kd_fb;    // previous_d 微分先行
                    // float inc_hz;
                    //不完全微分低通系数
                    // float k_inc_d_norm;
                    // Incomplete 不完全微分 归一（0,1）
    float k_ff;     // 前馈

} _PID_arg_st;

// ##: 历史数据记录结构体
typedef struct
{
    float err;             // 误差
    float exp_old;         // 上一次调用函数的期望值
    float feedback_old;    // 上一次的反馈值

    float fb_d;    // 反馈的微分
    float fb_d_ex;
    float exp_d;    // 期望值的误差

    //	float err_d_lpf;

    float err_i;    // 误差微分
    float ff;
    float pre_d;

    float out;    // 输出值
} _PID_val_st;

float PID_calculate( float        T,           //周期
                     float        in_ff,       //前馈
                     float        expect,      //期望值（设定值）
                     float        feedback,    //反馈值
                     _PID_arg_st* pid_arg,     // PID参数结构体
                     _PID_val_st* pid_val,     // PID数据结构体
                     float        inte_d_lim,
                     float        inte_lim    // integration limit，积分限幅
);                                            //输出

extern u8 _Angle_PID_InteClear_Flag_;

float PID_Height_Set( u8 set_height, u8 curr_height );
float PID_XTracking( s16 set_x_offset, s16 x_offset );
float PID_YTracking( s16 set_y_offset, s16 y_offset );
float PID_AngleAdjusting( s16 set_offset, s16 offset );
#endif
