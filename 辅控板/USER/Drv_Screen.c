#include "Drv_Screen.h"
#include "usart.h"
#include "led.h"

u8 Screen_Buf[20];
u8 Last_Screen_Buf[20];

u8 Change_Flag=1;

task_num_t TaskNum;
task_1_t Task1;
task_2_t Task2;

u8 Compare(u8 buffer1[20],u8 buffer2[20]){//相同给0，不同给1
	int i=0;
	while(i<=20){
		if(buffer1[i]==buffer2[i])i++;
		else {return 1;}
	}
	return 0;
}	

void Assignment(u8 buffer1[20],u8 buffer2[20]){//将 后 赋值给 前
	int i=0;
	while(i<=20){
	buffer1[i]=buffer2[i];
		i++;
	}
}

void Screen_Data_Analysis( )
{
		//LED_TOGGLE;
    if ( Screen_Buf[1] == 0x10 )
    {
        TaskNum.num= Screen_Buf[2];
    }
		else if(Screen_Buf[1] == 0x11)
		{
			Task1.target1=Screen_Buf[3];
			Task1.target2=Screen_Buf[4];
		}
		else if(Screen_Buf[1] == 0x12)
		{
			Task2.cmd=Screen_Buf[2];
		}
}

void Screen_GetOneByte( u8 bytedata )
{
    static u8 rec_sta = 0;
    static u8 objtype = 0;    // 判定接收的数据类型
    Screen_Buf[rec_sta] = bytedata;

    if ( rec_sta == 0 )
    {
        if ( bytedata == 0xAA )
            rec_sta++;
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
				else if( bytedata == 0x11 )
        {
            rec_sta++;
						objtype = 2;
            return;
        }
				else if( bytedata == 0x12 )
        {
            rec_sta++;
						objtype = 3;
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
        if ( rec_sta <= 2 )
        {
            rec_sta++;
        }
        else if ( rec_sta == 3 )
        {
            if ( bytedata != 0xFF )
            {
                rec_sta = 0;
            }
            else
            {
							if( Compare( Last_Screen_Buf,Screen_Buf) ){//相同给0，不同给1
									Change_Flag=1;
									//Assignment(Last_Screen_Buf,Screen_Buf);//将 后 赋值给 前
							}
							else{
								Change_Flag=0;
							}
                Screen_Data_Analysis( );
                rec_sta = 0;
								objtype = 0;
							}
            }
        }
			else if( objtype == 2 )
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
            }
            else
            {
							if( Compare( Last_Screen_Buf,Screen_Buf) ){//相同给0，不同给1
									Change_Flag=1;
									//Assignment(Last_Screen_Buf,Screen_Buf);//将 后 赋值给 前
							}
							else{
								Change_Flag=0;
							}							
                Screen_Data_Analysis( );
                rec_sta = 0;
								objtype = 0;
            }
        }				
			}
			else if( objtype == 3 )
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
            }
            else
            {
							if( Compare( Last_Screen_Buf,Screen_Buf) ){//相同给0，不同给1
									Change_Flag=1;
									//Assignment(Last_Screen_Buf,Screen_Buf);//将 后 赋值给 前
							}
							else{
								Change_Flag=0;
							}	
                Screen_Data_Analysis( );
                rec_sta = 0;
								objtype = 0;
            }
        }				
			}
    }
	}
