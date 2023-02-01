#include "ANO_LX.h"
#include "Drv_Sys.h"
#include "SysConfig.h"

typedef struct
{
    // #: 直线数据
    u8  flag;
    s16 angle;
    s16 distance;
} opmv_line_t;

typedef struct
{
    // #: 点数据
    u8  flag;
    s16 X;
    s16 Y;
} opmv_dot_t;

typedef struct
{
    // #: 圆数据
    u8  flag;
    s16 X;
    s16 Y;
} opmv_circle_t;

typedef struct
{
		u8 flag;
		u8 color;
		u8 shape;
		s16 X;
		s16 Y;
} opmv_color_t;

extern u8 Colour_Wanted;

extern opmv_line_t    Sideline_feature;
extern opmv_circle_t  circle_feature;
extern opmv_dot_t     CharacterA_feature;
extern opmv_color_t 	color_feature;
extern opmv_circle_t 	hula_feature;

void OpenMVNo1_GetOneByte( u8 bytedata );
void OpenMVNo2_GetOneByte( u8 bytedata );
