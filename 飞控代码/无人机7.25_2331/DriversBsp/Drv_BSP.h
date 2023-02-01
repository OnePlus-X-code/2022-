#ifndef _DRVBSP_H_
#define _DRVBSP_H_
#include "ANO_LX.h"
#include "Drv_Sys.h"
#include "SysConfig.h"

typedef struct
{
    u8 sig_mode;    // 0:无连接 1:PPM 2:SBUS
    //
    s16 ppm_ch[9];
    // ##: 使用的通信协议, 其中只有前面11个通道会进行使用
    s16 sbus_ch[16];
    u8  sbus_flag;
    //
    u16       signal_fre;
    u8        no_signal;
    u8        fail_safe;
    _rc_ch_un rc_ch;
    u16       signal_cnt_tmp;
    u8        rc_in_mode_tmp;
} _rc_input_st;

//==数据声明
extern _rc_input_st rc_in;

u8 All_Init( void );

void DrvRcInputInit( void );
void DrvPpmGetOneCh( u16 data );
void DrvSbusGetOneByte( u8 data );
void DrvRcInputTask( float dT_s );

#endif
