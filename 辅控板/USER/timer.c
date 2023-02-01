#include "timer.h"

static void Rcev_Timer_Init( u16 period_ms )    // APB1  84M
{
    TIM_TimeBaseInitTypeDef TIM_TBIS;
    // 使能时钟
    RCC_APB1PeriphClockCmd( Rcev_RCC_TIMx, ENABLE );
    TIM_DeInit( Recv_TIMx );

    TIM_TBIS.TIM_Period = period_ms;    // 自动重装载寄存器周期的值(计数值)
    TIM_TBIS.TIM_Prescaler =
        84 - 1;    // 累计 TIM_Period个频率后产生一个更新或者中断 时钟预分频数为84
    TIM_TBIS.TIM_ClockDivision =
        TIM_CKD_DIV1;    // 对外部时钟进行采样的时钟分频,这里没有用到
    TIM_TBIS.TIM_CounterMode = TIM_CounterMode_Up;    //向上计数

    TIM_TimeBaseInit( Recv_TIMx, &TIM_TBIS );

    TIM_ClearFlag( Recv_TIMx, TIM_FLAG_Update );
    TIM_ITConfig( Recv_TIMx, TIM_IT_Update, ENABLE );

    TIM_Cmd( Recv_TIMx, ENABLE );

    RCC_APB1PeriphClockCmd( Rcev_RCC_TIMx, DISABLE );    //先关闭等待使
}


static void Send_Timer_Init( u32 period_ms )    // APB1  84M
{
    TIM_TimeBaseInitTypeDef TIM_TBIS;
    // 使能时钟
    RCC_APB1PeriphClockCmd( Send_RCC_TIMx, ENABLE );
    TIM_DeInit( Send_TIMx );

    TIM_TBIS.TIM_Period = period_ms;    // 自动重装载寄存器周期的值(计数值)
    TIM_TBIS.TIM_Prescaler =
        84 - 1;    // 累计 TIM_Period个频率后产生一个更新或者中断 时钟预分频数为84
    TIM_TBIS.TIM_ClockDivision =
        TIM_CKD_DIV1;    // 对外部时钟进行采样的时钟分频,这里没有用到
    TIM_TBIS.TIM_CounterMode = TIM_CounterMode_Up;    //向上计数

    TIM_TimeBaseInit( Send_TIMx, &TIM_TBIS );

    TIM_ClearFlag( Send_TIMx, TIM_FLAG_Update );
    TIM_ITConfig( Send_TIMx, TIM_IT_Update, ENABLE );

    TIM_Cmd( Send_TIMx, ENABLE );

    RCC_APB1PeriphClockCmd( Send_RCC_TIMx, DISABLE );    //先关闭等待使
}

static void TIM_NVIC( )
{
    NVIC_InitTypeDef NVIC_IS;

    // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_IS.NVIC_IRQChannel                   = Rcev_TIMx_IRQn;
    NVIC_IS.NVIC_IRQChannelPreemptionPriority = NVIC_TIME_P;
    NVIC_IS.NVIC_IRQChannelSubPriority        = NVIC_TIME_S;
    NVIC_IS.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init( &NVIC_IS );

    NVIC_IS.NVIC_IRQChannel                   = Send_TIMx_IRQn;
    NVIC_IS.NVIC_IRQChannelPreemptionPriority = NVIC_TIME_P;
    NVIC_IS.NVIC_IRQChannelSubPriority        = NVIC_TIME_S + 1;
    NVIC_IS.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init( &NVIC_IS );
}

void Sys_Timer_Init( void )
{
    // 定时1ms毫秒运行
    Rcev_Timer_Init( 1000 );
    Send_Timer_Init( 1000 );
    TIM_NVIC( );

    // TIM7 重新开时钟，开始计时
    RCC_APB1PeriphClockCmd( Rcev_RCC_TIMx, ENABLE );
		// TIM4 重新开时钟，开始计时
    RCC_APB1PeriphClockCmd( Send_RCC_TIMx, ENABLE );
}
//****************** (C) COPYRIGHT 2014 ANO TECH *****END OF FILE**********
