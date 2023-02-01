/******************** (C) COPYRIGHT 2016 ANO Tech ***************************
 * 作者		 ：匿名科创
 * 文件名  ：ANO_PID.c
 * 描述    ：PID函数
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
 *****************************************************************************/
#include "Ano_Pid.h"
#include "Ano_Math.h"
#include "Drv_AnoOf.h"
#include "User_Task.h"

u8 _Angle_PID_InteClear_Flag_ = 0;

float PID_calculate( float        dT_s,          // 周期（单位：秒）
                     float        in_ff,         // 前馈值
                     float        expect,        // 期望值（设定值）
                     float        feedback,      // 反馈值（）
                     _PID_arg_st* pid_arg,       // PID参数结构体
                     _PID_val_st* pid_val,       // PID数据结构体
                     float        inte_d_lim,    // 积分误差限幅
                     float        inte_lim       // integration limit，积分限幅
)
{
    float differential, hz;
    // #: 测量一秒内调用此函数的频率, 在PID中作为 dt 分之一
    hz = safe_div( 1.0f, dT_s, 0 );
    // pid_arg->k_inc_d_norm = LIMIT(pid_arg->k_inc_d_norm,0,1);
    // #: 期望值微分
    pid_val->exp_d = ( expect - pid_val->exp_old ) * hz;

    if ( pid_arg->fb_d_mode == 0 )
    {
        // #: (feedback - pid_val->feedback_old)作为 de(t) 求得反馈的微分
        pid_val->fb_d = ( feedback - pid_val->feedback_old ) * hz;
    }
    else
    {
        // #: 从别处返回的反馈微分 ?:(我们应该用不上)
        pid_val->fb_d = pid_val->fb_d_ex;
    }
    // #: 采用微分先行的的策略, 期望值的微分系数和反馈值的微分系数不相同,
    //    调整更加准确, 如果kd_ex和kd_fb相同, 则成为传统的PID控制.
    differential = ( pid_arg->kd_ex * pid_val->exp_d - pid_arg->kd_fb * pid_val->fb_d );
    // #: 误差e(t)
    pid_val->err = ( expect - feedback );
    // #: 积分部分, 对积分e(τ)dτ和积分结果进行了限制, 防止位置限制产生超调.
    pid_val->err_i +=
        pid_arg->ki * LIMIT( ( pid_val->err ), -inte_d_lim, inte_d_lim ) * dT_s;
    pid_val->err_i = LIMIT( pid_val->err_i, -inte_lim, inte_lim );
    // ?: 这几个注释真没看懂在干啥
    //)*T;//+ differential/pid_arg->kp
    // pid_val->err_i += pid_arg->ki *(pid_val->err)*T;//)*T;//+
    // pid_arg->k_pre_d *pid_val->feedback_d
    pid_val->out = pid_arg->k_ff * in_ff + pid_arg->kp * pid_val->err +
                   differential
                   //	    + pid_arg->k_inc_d_norm *pid_val->err_d_lpf +
                   //(1.0f-pid_arg->k_inc_d_norm) *differential
                   + pid_val->err_i;
    // #: 将此值保存, 用作下一次调用.
    pid_val->feedback_old = feedback;
    pid_val->exp_old      = expect;
    // #: 输出返回值
    return ( pid_val->out );
}

float PID_Height_Set( u8 set_height, u8 curr_height )
{
    static _PID_arg_st Height_PID_arg = { 0, 1, 0, 1.5, 0.15, 0 };
    static _PID_val_st Height_PID_val = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    return PID_calculate( 0.033f, 0, set_height, curr_height, &Height_PID_arg,
                          &Height_PID_val, 10, 30 );
}

float PID_XTracking( s16 set_x_offset, s16 x_offset )
{
    static _PID_arg_st X_PID_arg = { 0, 0.008, 0, 0.001, 0, 0 };
    static _PID_val_st X_PID_val = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    // 0.003
    return PID_calculate( 0.05f, 0, set_x_offset, x_offset, &X_PID_arg, &X_PID_val, 10,
                          30 );
}

float PID_YTracking( s16 set_y_offset, s16 y_offset )
{
    static _PID_arg_st Y_PID_arg = { 0, 0.008, 0, 0.001, 0, 0 };
    static _PID_val_st Y_PID_val = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    return PID_calculate( 0.05f, 0, set_y_offset, y_offset, &Y_PID_arg, &Y_PID_val, 10,
                          30 );
}

float PID_AngleAdjusting( s16 set_offset, s16 offset )
{
    static _PID_arg_st Angle_PID_arg = { 0, 0.3, 0.1, 0, 0.05, 0 };
    static _PID_val_st Angle_PID_val = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    if ( _Angle_PID_InteClear_Flag_ != 0 )
    {
        Angle_PID_val.err_i        = 0;
        _Angle_PID_InteClear_Flag_ = 0;
    }

    return PID_calculate( 0.05f, 0, set_offset, offset, &Angle_PID_arg, &Angle_PID_val, 5,
                          20 );
}
