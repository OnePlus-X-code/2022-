/**************************************************
 * 描述    ：任务调度
 ***************************************************/
#include "Ano_Scheduler.h"

#include "Ano_Kalman.h"
#include "Ano_Math.h"
#include "Ano_Pid.h"

#include "Drv_AnoOf.h"
#include "Drv_Beep.h"
#include "Drv_Ctrl.h"
#include "Motor.h"
#include "Drv_OpenMV.h"

#include "LX_FC_Fun.h"
#include "User_Task.h"

u8 Working_Mode = 0;

s16 Y_velo   = 0;
s16 X_velo   = 0;
s16 Z_velo   = 0;
s8  Yaw_velo = 0;

//////////////////////////////////////////////////////////////////////
//用户程序调度器
//////////////////////////////////////////////////////////////////////

// static void Loop_1000Hz(void) //1ms执行一次
// {
// }

// static void Loop_500Hz(void) //2ms执行一次
// {
// }

// static void Loop_100Hz(void) //10ms执行一次
// {
// }

// static void Loop_50Hz( void ) // 20ms执行一次
// {
// }

 static void Loop_10000Hz(void) //5ms执行一次
 {
		if( Motor_control == 1 )//顺时针
		{
				Motor_state=(Motor_state+9)%8;
				WriteData(states[Motor_state]);
		}
		if( Motor_control == 2 )//逆时针
		{
				Motor_state=(Motor_state+7)%8;
				WriteData(states[Motor_state]);
		}
 }

static void Loop_30Hz( void )    // 33ms执行一次
{
    // #: 定高PID
    if ( _Height_PID_Flag_ )
    {
        if ( Height > 170 )
            Set_Height( 160 );
        Z_velo = PID_Height_Set( Height, ano_of.of_alt_cm );
        Set_vel_Z( Z_velo );
    }
}

static void Loop_20Hz( void )    // 50ms执行一次
{
    // 进行圆心的追踪
    if ( _Circle_PID_Flag_ && circle_feature.flag )
    {
        X_velo = -PID_XTracking( 0, circle_feature.Y ) * ano_of.of_alt_cm / 2;
        Y_velo = PID_YTracking( 0, circle_feature.X ) * ano_of.of_alt_cm / 2;
        Set_vel_X( X_velo );
        Set_vel_Y( Y_velo );
    }
		
		if ( _Position_PID_Flag_ && Position_Feature.flag )
		{
				
				X_velo = PID_XTracking( TargetX, Position_Feature.X )*40;
				Y_velo = PID_YTracking( TargetY, Position_Feature.Y )*40;
				Set_vel_X( X_velo );
        Set_vel_Y( Y_velo );
		}
		
		// 进行色块的追踪
		if ( _Color_PID_Flag_ && color_feature.flag )
		{
				X_velo = -PID_XTracking( 0, color_feature.Y ) * ano_of.of_alt_cm / 8;
        Y_velo = PID_YTracking( 0, color_feature.X ) * ano_of.of_alt_cm / 8;
        Set_vel_X( X_velo );
        Set_vel_Y( Y_velo );
		}
		 
		 if ( hula_feature.flag && _Hula_PID_Flag_ )
		 {
				Height_PID_off;
				Z_velo = -PID_XTracking( 0, circle_feature.Y ) * ano_of.of_alt_cm;
        Y_velo = PID_YTracking( 0, circle_feature.X ) * ano_of.of_alt_cm;
        Set_vel_Z( Z_velo );
        Set_vel_Y( Y_velo );
		 }
		 
}

static void Loop_10Hz( void )    // 100ms执行一次
{

}

static void Loop_5Hz( void )    // 200ms执行一次
{
    // ##: 工作模式的选择
    switch ( Working_Mode )
    {
				case 0: User_Task0( );break;
        case 1: User_Task1( );break;
				case 2: User_Task2( );break;
				case 3: User_Task3( );break;
        default: break;
    }
}

static void Loop_2Hz( void )    // 500ms执行一次
{
    // 蜂鸣器计数控制并进行报警
    if ( Beep_Flag != 0 )
        Beep_Pin_TOGGLE;
    else
        Beep_Pin_Off;

    // 控制信号接收
    if ( Ctrl_Signal.task_num == 0x01 && Working_Mode != 1 )
    {
        Ctrl_Signal.task_num = 0x00;
        Working_Mode         = 1;
    }
		else if( Ctrl_Signal.task_num == 0x02 && Working_Mode != 2 )
    {
        Ctrl_Signal.task_num = 0x00;
        Working_Mode         = 2;
    }
		else if( Ctrl_Signal.task_num == 0x03 && Working_Mode != 3 )
    {
        Ctrl_Signal.task_num = 0x00;
        Working_Mode         = 3;
    }
}

//////////////////////////////////////////////////////////////////////
//调度器初始化
//////////////////////////////////////////////////////////////////////
//系统任务配置，创建不同执行频率的“线程”
static sched_task_t sched_tasks[] = {
    // { Loop_1000Hz, 1000, 0, 0 }, { Loop_500Hz, 500, 0, 0 }, 
    // { Loop_100Hz, 100, 0, 0 },   { Loop_50Hz, 50, 0, 0 },
    { Loop_10000Hz, 20000, 0, 0 },{ Loop_30Hz, 30, 0, 0 }, { Loop_20Hz, 20, 0, 0 }, { Loop_10Hz, 10, 0, 0 },
    { Loop_5Hz, 5, 0, 0 },   { Loop_2Hz, 2, 0, 0 },
};

//根据数组长度，判断线程数量
#define TASK_NUM ( sizeof( sched_tasks ) / sizeof( sched_task_t ) )

void Scheduler_Setup( void )
{
    uint8_t index = 0;
    //初始化任务表
    for ( index = 0; index < TASK_NUM; index++ )
    {
        //计算每个任务的延时周期数
        sched_tasks[index].interval_ticks = TICK_PER_SECOND / sched_tasks[index].rate_hz;
        //最短周期为1，也就是1ms
        if ( sched_tasks[index].interval_ticks < 1 )
        {
            sched_tasks[index].interval_ticks = 1;
        }
    }
}

//这个函数放到main函数的while(1)中，不停判断是否有线程应该执行
void Scheduler_Run( void )
{
    uint8_t index = 0;
    //循环判断所有线程，是否应该执行

    for ( index = 0; index < TASK_NUM; index++ )
    {
        //获取系统当前时间，单位MS
        uint32_t tnow = GetSysRunTimeMs( );
        //进行判断，如果当前时间减去上一次执行的时间，大于等于该线程的执行周期，则执行线程
        if ( tnow - sched_tasks[index].last_run >= sched_tasks[index].interval_ticks )
        {
            //更新线程的执行时间，用于下一次判断
            sched_tasks[index].last_run = tnow;
            //执行线程函数，使用的是函数指针
            sched_tasks[index].task_func( );
        }
    }
}
/************************END OF FILE************************/
