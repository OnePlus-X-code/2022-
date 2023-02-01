#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_
#include "SysConfig.h"

typedef struct
{
    void ( *task_func )( void );
    uint16_t rate_hz;
    uint16_t interval_ticks;
    uint32_t last_run;
} sched_task_t;

void Scheduler_Setup( void );
void Scheduler_Run( void );

extern u8  Working_Mode;
extern s16 Y_velo;
extern s16 X_velo;
extern s16 Z_velo;
extern s8  Yaw_velo;
#endif
