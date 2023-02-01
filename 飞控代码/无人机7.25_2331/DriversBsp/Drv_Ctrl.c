#include "Drv_Ctrl.h"
#include "Ano_Kalman.h"
#include "Drv_Beep.h"

u8 Ctrl_Buf[20];
// #: 控制信号接收格式: 0xAA 0x10 num 0xFF
// #: 距离目标接收格式: 0xAA 0x11 0x01 0x?? 0x?? 0xFF
// #：接受是否识别完成进行下一步： 0xAA 0x12 0x0? 0xFF
// #：UWB位置信息： 0xAA 0x13 0x01 0x?? 0x??(x) 0x?? 0x??(y) 0xFF
Ctrl_Sgn_t      Ctrl_Signal;
Ctrl_Target_t 	Ctrl_TargetOne,Ctrl_TargetTwo;
Ctrl_Position_t Position_Feature;
u8 						  Fully_Recognized=0;
static void Ctrl_Data_Analysis( )
{
    if ( Ctrl_Buf[1] == 0x10 )
    {
        Ctrl_Signal.task_num = Ctrl_Buf[2];
				
    }
    else if ( Ctrl_Buf[1] == 0x11 )
    {
				Ctrl_TargetOne.flag		= Ctrl_Buf[2];
				Ctrl_TargetOne.target1 = Ctrl_Buf[3];
				Ctrl_TargetOne.target2 = Ctrl_Buf[4];
    }
		else if ( Ctrl_Buf[1] == 0x12 )
		{
				Fully_Recognized = Ctrl_Buf[2];
		}
		else if ( Ctrl_Buf[1] == 0x13 )
		{
				Position_Feature.flag =  Ctrl_Buf[2];
				Position_Feature.X = (s16)( Ctrl_Buf[3] << 8 | Ctrl_Buf[4] );
				Position_Feature.Y = (s16)((s16)( Ctrl_Buf[5] << 8 | Ctrl_Buf[6] )-30);
		}
}

void Ctrl_GetOneByte( u8 bytedata )
{
    static u8 rec_sta = 0;
    static u8 objtype = 0;
	
    Ctrl_Buf[rec_sta] = bytedata;

    if ( rec_sta == 0 )
    {
        if ( bytedata == 0xAA ){
            rec_sta++;}
        else
            rec_sta = 0;
    }
    else if ( rec_sta == 1 )
    {
        if ( bytedata == 0x10 )
        {
            rec_sta++;
            objtype = 1;
            return;
        }
        if ( bytedata == 0x11 )
        {
            rec_sta++;
            objtype = 2;
            return;
        }
				if ( bytedata == 0x12 )
        {
            rec_sta++;
            objtype = 3;
            return;
        }
				if ( bytedata == 0x13 )
        {
            rec_sta++;
            objtype = 4;
            return;
        }
    }
    else if ( rec_sta >= 2 )
    {
        if ( objtype == 1 )
        {
            if ( rec_sta <= 2 )
            {
                rec_sta++;
            }
            else if ( rec_sta == 3 )
            {
                if ( bytedata != 0xFF )
                {
                    rec_sta = 0;
                    objtype = 0;
                }
                else
                {
                    Ctrl_Data_Analysis( );
                    rec_sta = 0;
                    objtype = 0;
                }
            }
        }
        else if ( objtype == 2 )
        {
            if ( rec_sta <= 4 )
            {
                rec_sta++;
            }
            else if ( rec_sta == 5 )
            {
                if ( bytedata != 0xFF )
                {
                    rec_sta = 0;
                    objtype = 0;
                }
                else
                {
                    Ctrl_Data_Analysis( );
                    rec_sta = 0;
                    objtype = 0;
                }
            }
        }
				else if ( objtype == 3 )
        {
            if ( rec_sta <= 2 )
            {
                rec_sta++;
            }
            else if ( rec_sta == 3 )
            {
                if ( bytedata != 0xFF )
                {
                    rec_sta = 0;
                    objtype = 0;
                }
                else
                {
                    Ctrl_Data_Analysis( );
                    rec_sta = 0;
                    objtype = 0;
                }
            }
        }
				else if ( objtype == 4 )
        {
            if ( rec_sta <= 6 )
            {
                rec_sta++;
            }
            else if ( rec_sta == 7 )
            {
                if ( bytedata != 0xFF )
                {
                    rec_sta = 0;
                    objtype = 0;
                }
                else
                {
                    Ctrl_Data_Analysis( );
                    rec_sta = 0;
                    objtype = 0;
                }
            }
        }
    }
}
