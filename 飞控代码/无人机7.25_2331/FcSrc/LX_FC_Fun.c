#include "LX_FC_Fun.h"
#include "ANO_DT_LX.h"
#include "LX_FC_State.h"
#include "User_Task.h"


/**
 * @brief  对某一个变量的出现程度进行判断 若连续3次置位, 则判定
 * 此值为置位, 若连续3次复位, 则判定为复位, 否则状态不确定.
 *
 * @param  flag: 变量此时的置位与复位情况.
 *
 * @retval -1:复位 1:置位 0:不确定
 */
s8 Judge_Appear( u8 flag )
{
    static u8 buff = 0x00;

    if ( flag != 0 )
    {
        buff <<= 1;
        buff++;
    }
    else
        buff <<= 1;

    if ( (buff & 0x0F) == 0x0F )
        return 1;
    else if ( (( ~buff ) & 0x0F) == 0x0F )
        return -1;
    else
        return 0;
}

// 有关运动控制的函数

void Set_vel_X( s16 x_velo )
{
    if ( x_velo > 20 )
        rt_tar.st_data.vel_x = 20;
    else if ( x_velo < -20 )
        rt_tar.st_data.vel_x = -20;
    else
        rt_tar.st_data.vel_x = x_velo;
}

/**
 * @brief  设置Y轴方向的速度
 *
 * @param  y_velo: 设置的速度值
 *
 * @retval None
 */
void Set_vel_Y( s16 y_velo )
{
    if ( y_velo > 20 )
        rt_tar.st_data.vel_y = 20;
    else if ( y_velo < -20 )
        rt_tar.st_data.vel_y = -20;
    else
        rt_tar.st_data.vel_y = y_velo;
}

/**
 * @brief  设置Z轴方向的速度
 *
 * @param  z_velo: 设置的速度值
 *
 * @retval None
 */
void Set_vel_Z( s16 z_velo )
{
    if ( z_velo > 40 )
        rt_tar.st_data.vel_z = 40;
    else if ( z_velo < -40 )
        rt_tar.st_data.vel_z = -40;
    else
        rt_tar.st_data.vel_z = z_velo;
}

/**
 * @brief  设置旋转的速度
 *
 * @param  yaw_dps: 设置的速度值
 *
 * @retval None
 */
void Set_Dps_Yaw( s16 yaw_dps ) { rt_tar.st_data.yaw_dps = yaw_dps; }

/**
 * @brief  停止一切运动
 *
 * @retval None
 */
void Stop_Motion( )
{
    rt_tar.st_data.vel_x   = 0;
    rt_tar.st_data.vel_y   = 0;
    rt_tar.st_data.vel_z   = 0;
    rt_tar.st_data.yaw_dps = 0;
}

/**
 * @brief  停止水平运动
 *
 * @retval None
 */
void Stop_Horizontal_Move( )
{
    rt_tar.st_data.vel_x   = 0;
    rt_tar.st_data.vel_y   = 0;
    rt_tar.st_data.yaw_dps = 0;
}
//////////////////////////////////////////////////////////////////////
//基本功能函数
//////////////////////////////////////////////////////////////////////

u8 FC_Unlock( )
{
    fc_sta.unlock_cmd = 1;    //解锁
    // 按协议发送指令
    if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
    {
        dt.cmd_send.CID    = 0x10;
        dt.cmd_send.CMD[0] = 0x00;
        dt.cmd_send.CMD[1] = 0x01;
        CMD_Send( 0XFF, &dt.cmd_send );
        return 1;
    }
    else
    {
        return 0;
    }
}

u8 FC_Lock( )
{
    fc_sta.unlock_cmd = 0;    //上锁
    //按协议发送指令
    if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
    {
        dt.cmd_send.CID    = 0x10;
        dt.cmd_send.CMD[0] = 0x00;
        dt.cmd_send.CMD[1] = 0x02;
        CMD_Send( 0XFF, &dt.cmd_send );
        return 1;
    }
    else
    {
        return 0;
    }
}

//改变飞控模式(模式0-3)
u8 LX_Change_Mode( u8 new_mode )
{
    static u8 old_mode;
    if ( old_mode != new_mode )
    {
        //
        if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
        {
            old_mode = fc_sta.fc_mode_cmd = new_mode;
            //按协议发送指令
            dt.cmd_send.CID    = 0X01;
            dt.cmd_send.CMD[0] = 0X01;
            dt.cmd_send.CMD[1] = 0X01;
            dt.cmd_send.CMD[2] = fc_sta.fc_mode_cmd;
            CMD_Send( 0xff, &dt.cmd_send );
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else    //已经在当前模式
    {
        return 1;
    }
}

u8 FC_Turn_Left( u16 angle, u16 acc_angle )
{
    //
    if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
    {
        //按协议发送指令
        dt.cmd_send.CID    = 0X10;
        dt.cmd_send.CMD[0] = 0X02;
        dt.cmd_send.CMD[1] = 0X07;
        dt.cmd_send.CMD[2] = BYTE0( angle );
        dt.cmd_send.CMD[3] = BYTE1( angle );
        dt.cmd_send.CMD[4] = BYTE0( acc_angle );
        dt.cmd_send.CMD[5] = BYTE1( acc_angle );
        CMD_Send( 0xff, &dt.cmd_send );
        return 1;
    }
    else
    {
        return 0;
    }
}

u8 FC_Turn_Right( u16 angle, u16 acc_angle )
{
    if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
    {
        //按协议发送指令
        dt.cmd_send.CID    = 0X10;
        dt.cmd_send.CMD[0] = 0X02;
        dt.cmd_send.CMD[1] = 0X08;
        dt.cmd_send.CMD[2] = BYTE0( angle );
        dt.cmd_send.CMD[3] = BYTE1( angle );
        dt.cmd_send.CMD[4] = BYTE0( acc_angle );
        dt.cmd_send.CMD[5] = BYTE1( acc_angle );
        CMD_Send( 0xff, &dt.cmd_send );
        return 1;
    }
    else
    {
        return 0;
    }
}

u8 FC_UP_DOWN( u16 Height, u8 dir, u16 speed )
{
    if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
    {
        //按协议发送指令
        dt.cmd_send.CID    = 0X10;
        dt.cmd_send.CMD[0] = 0X02;

        if ( dir )
            dt.cmd_send.CMD[1] = 0X01;
        else
            dt.cmd_send.CMD[1] = 0X02;

        dt.cmd_send.CMD[2] = BYTE0( Height );
        dt.cmd_send.CMD[3] = BYTE1( Height );
        dt.cmd_send.CMD[4] = BYTE0( speed );
        dt.cmd_send.CMD[5] = BYTE1( speed );
        CMD_Send( 0xff, &dt.cmd_send );
        return 1;
    }
    else
    {
        return 0;
    }
}

//一键返航
//需要注意，程控模式下才能执行返航
u8 OneKey_Return_Home( )
{
    //
    if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
    {
        //按协议发送指令
        dt.cmd_send.CID    = 0X10;
        dt.cmd_send.CMD[0] = 0X00;
        dt.cmd_send.CMD[1] = 0X07;
        CMD_Send( 0xff, &dt.cmd_send );
        return 1;
    }
    else
    {
        return 0;
    }
}

//一键起飞(高度cm)
u8 OneKey_Takeoff( u16 height_cm )
{
    //
    if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
    {
        //按协议发送指令
        dt.cmd_send.CID    = 0X10;
        dt.cmd_send.CMD[0] = 0X00;
        dt.cmd_send.CMD[1] = 0X05;
        dt.cmd_send.CMD[2] = BYTE0( height_cm );
        dt.cmd_send.CMD[3] = BYTE1( height_cm );
        CMD_Send( 0xff, &dt.cmd_send );
        return 1;
    }
    else
    {
        return 0;
    }
}

//一键降落
u8 OneKey_Land( )
{
    //
    if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
    {
        //按协议发送指令
        dt.cmd_send.CID    = 0X10;
        dt.cmd_send.CMD[0] = 0X00;
        dt.cmd_send.CMD[1] = 0X06;
        CMD_Send( 0xff, &dt.cmd_send );
        return 1;
    }
    else
    {
        return 0;
    }
}

//平移(距离cm，速度cmps，方向角度0-360度)
u8 Horizontal_Move( u16 distance_cm, u16 velocity_cmps, u16 dir_angle_0_360 )
{
    //
    if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
    {
        //按协议发送指令
        dt.cmd_send.CID    = 0X10;
        dt.cmd_send.CMD[0] = 0X02;
        dt.cmd_send.CMD[1] = 0X03;
        //
        dt.cmd_send.CMD[2] = BYTE0( distance_cm );
        dt.cmd_send.CMD[3] = BYTE1( distance_cm );
        dt.cmd_send.CMD[4] = BYTE0( velocity_cmps );
        dt.cmd_send.CMD[5] = BYTE1( velocity_cmps );
        dt.cmd_send.CMD[6] = BYTE0( dir_angle_0_360 );
        dt.cmd_send.CMD[7] = BYTE1( dir_angle_0_360 );
        //
        CMD_Send( 0xff, &dt.cmd_send );
        return 1;
    }
    else
    {
        return 0;
    }
}

//水平校准
u8 Horizontal_Calibrate( )
{
    //
    if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
    {
        //按协议发送指令
        dt.cmd_send.CID    = 0X01;
        dt.cmd_send.CMD[0] = 0X00;
        dt.cmd_send.CMD[1] = 0X03;
        CMD_Send( 0xff, &dt.cmd_send );
        return 1;
    }
    else
    {
        return 0;
    }
}

//磁力计校准
u8 Mag_Calibrate( )
{
    //
    if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
    {
        //按协议发送指令
        dt.cmd_send.CID    = 0X01;
        dt.cmd_send.CMD[0] = 0X00;
        dt.cmd_send.CMD[1] = 0X04;
        CMD_Send( 0xff, &dt.cmd_send );
        return 1;
    }
    else
    {
        return 0;
    }
}

// 6面加速度校准
u8 ACC_Calibrate( )
{
    //
    if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
    {
        //按协议发送指令
        dt.cmd_send.CID    = 0X01;
        dt.cmd_send.CMD[0] = 0X00;
        dt.cmd_send.CMD[1] = 0X05;
        CMD_Send( 0xff, &dt.cmd_send );
        return 1;
    }
    else
    {
        return 0;
    }
}

//陀螺仪校准
u8 GYR_Calibrate( )
{
    //
    if ( dt.wait_ck == 0 )    //没有其他等待校验的CMD时才发送本CMD
    {
        //按协议发送指令
        dt.cmd_send.CID    = 0X01;
        dt.cmd_send.CMD[0] = 0X00;
        dt.cmd_send.CMD[1] = 0X02;
        CMD_Send( 0xff, &dt.cmd_send );
        return 1;
    }
    else
    {
        return 0;
    }
}
