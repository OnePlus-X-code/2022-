#include "Drv_OpenMV.h"

u8 openmv1_buf[20];
u8 openmv2_buf[20];

// 0x3X用于下视的摄像头, 0x2X用于前视的摄像头
// 0x30:dot 0x31:circle 0x32:color
// 0x20:呼啦圈
// OV 1:下视摄像头  2:前视摄像头

// 点信息
opmv_dot_t CharacterA_feature;
// 圆信息
opmv_circle_t  circle_feature;
opmv_circle_t hula_feature;
//色块
opmv_color_t color_feature;
 u8 Colour_Wanted=0;

static void OpenMVNo1_Data_Analysis( )
{
    if ( openmv1_buf[1] == 0x30 )
    {
        CharacterA_feature.flag = openmv1_buf[2];
        CharacterA_feature.X    = (s16)( openmv1_buf[3] << 8 | openmv1_buf[4] );
        CharacterA_feature.Y    = (s16)( openmv1_buf[5] << 8 | openmv1_buf[6] );
    }
    else if ( openmv1_buf[1] == 0x31 )
    {
        circle_feature.flag = openmv1_buf[2];
        circle_feature.X    = (s16)( openmv1_buf[3] << 8 | openmv1_buf[4] );
        circle_feature.Y    = (s16)( openmv1_buf[5] << 8 | openmv1_buf[6] );
    }
		else if ( openmv1_buf[1] == 0x32)
		{
				color_feature.flag = openmv1_buf[2];
				color_feature.color = openmv1_buf[3];
				color_feature.shape = openmv1_buf[4];
				color_feature.X = (s16)( openmv1_buf[5] << 8 | openmv1_buf[6] );
				color_feature.Y = (s16)( openmv1_buf[7] << 8 | openmv1_buf[8] );
		}
    return;
}

static void OpenMVNo2_Data_Analysis( )
{
		if ( openmv2_buf[1] == 0x20 )
    {
        hula_feature.flag = openmv2_buf[2];
        hula_feature.X    = (s16)( openmv2_buf[3] << 8 | openmv2_buf[4] );
        hula_feature.Y    = (s16)( openmv2_buf[5] << 8 | openmv2_buf[6] );
    }
    return;
}

void OpenMVNo1_GetOneByte( u8 bytedata )
{
    static u8 rec_sta = 0;    // 判定接收数量
    static u8 objtype = 0;    // 判定接收的数据类型

    openmv1_buf[rec_sta] = bytedata;

    if ( rec_sta == 0 )
    {
        if ( bytedata == 0xAA )
            rec_sta++;
        else
            rec_sta = 0;
    }
    else if ( rec_sta == 1 )
    {
        // receive characterA position
        if ( bytedata == 0x30 )
        {
            rec_sta++;
            objtype = 1;
            return;
        }
        // receive circle
        else if ( bytedata == 0x31 )
        {
            rec_sta++;
            objtype = 2;
            return;
        }
				// receive colour block
        else if ( bytedata == 0x32 )
        {
            rec_sta++;
            objtype = 3;
            return;
        }
        // clear and exit
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
                    OpenMVNo1_Data_Analysis( );
                    rec_sta = 0;
                    objtype = 0;
                }
            }
        }
        else if ( objtype == 2 )
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
                    OpenMVNo1_Data_Analysis( );
                    rec_sta = 0;
                    objtype = 0;
                }
            }
        }
				else if (objtype == 3)
				{
					if ( rec_sta <= 8 )
					{
						rec_sta++;
					}
					else if ( rec_sta == 9)
					{
							if ( bytedata != 0xFF )
							{
									rec_sta = 0;
                  objtype = 0;
							}
							else
							{
									OpenMVNo1_Data_Analysis( );
                  rec_sta = 0;
                  objtype = 0;
							}
					}
				}
    }
}

void OpenMVNo2_GetOneByte( u8 bytedata )
{
    static u8 rec_sta    = 0;
    static u8 objtype    = 0;
    openmv2_buf[rec_sta] = bytedata;

    if ( rec_sta == 0 )
    {
        if ( bytedata == 0xAA )
            rec_sta++;
        else
            rec_sta = 0;
    }
    else if ( rec_sta == 1 )
    {
        if ( bytedata == 0x20 )
        {
            rec_sta++;
            objtype = 1;
            return;
        }
    }
    else if ( rec_sta >= 2 )
    {
        if ( objtype == 1 )
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
                    OpenMVNo2_Data_Analysis( );
                    rec_sta = 0;
                    objtype = 0;
                }
            }
        }
    }
}
