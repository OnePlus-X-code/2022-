#include "Drv_FC.h"
#include "Drv_Screen.h"

u8 FC_buf[20];
u8 Last_FC_buf[20];
object_feature_t ObjectA;
object_flag_t FlagA;

u8 FC_Change_Flag=0;

static void FC_Data_Analysis( )
{
    if ( FC_buf[1] == 0x13 )
    {
				FlagA.flag=1;
        ObjectA.color = FC_buf[2];
				ObjectA.shape = FC_buf[3];
    }
    return;
}


void FC_GetOneByte( u8 bytedata )
{
    static u8 rec_sta = 0;    // 判定接收数量
    static u8 objtype = 0;    // 判定接收的数据类型

    FC_buf[rec_sta] = bytedata;

    if ( rec_sta == 0 )
    {
        if ( bytedata == 0xAA )
            rec_sta++;
        else
            rec_sta = 0;
    }
    else if ( rec_sta == 1 )
    {
        
        if ( bytedata == 0x13 )
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
            if ( rec_sta <= 3 )
            {
                rec_sta++;
            }
            else if ( rec_sta == 4 )
            {
                if ( bytedata != 0xFF )
                {
                    rec_sta = 0;
                    objtype = 0;
                }
                else
                {
							if( Compare( Last_FC_buf,FC_buf) ){//相同给0，不同给1
									FC_Change_Flag=1;
									//Assignment(Last_Screen_Buf,Screen_Buf);//将 后 赋值给 前
							}
							else{
								FC_Change_Flag=0;
							}
                    FC_Data_Analysis( );
                    rec_sta = 0;
                    objtype = 0;
                }
            }
        }
    }
}
