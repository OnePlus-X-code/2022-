#include "Drv_Uwb.h"

u8 UWB_buf[20];
coordinate_t Label;

static void UWB_Data_Analysis( )
{
    if ( UWB_buf[1] == 0x12 )
    {
			Label.flag=1;
			Label.XH=UWB_buf[2];
			Label.XL=UWB_buf[3];
			Label.YH=UWB_buf[4];
			Label.YL=UWB_buf[5];
    }
    return;
}


void UWB_GetOneByte( u8 bytedata )
{
    static u8 rec_sta = 0;    // 判定接收数量
    static u8 objtype = 0;    // 判定接收的数据类型

    UWB_buf[rec_sta] = bytedata;

    if ( rec_sta == 0 )
    {
        if ( bytedata == 0xAA )
            rec_sta++;
        else
            rec_sta = 0;
    }
    else if ( rec_sta == 1 )
    {
        
        if ( bytedata == 0x12 )
        {
            rec_sta++;
            objtype = 1;
            return;
        }
        else
        {
            rec_sta = 0;
            objtype = 0;
            return;
        }
    }
    else if ( rec_sta >= 2 )
    {
        if ( objtype == 1 )
        {
            if ( rec_sta <= 5 )
            {
                rec_sta++;
            }
            else if ( rec_sta == 6 )
            {
                if ( bytedata != 0xFF )
                {
                    rec_sta = 0;
                    objtype = 0;
                }
                else
                {
                    UWB_Data_Analysis( );
                    rec_sta = 0;
                    objtype = 0;
                }
            }
        }
    }
}

