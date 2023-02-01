#include "Drv_Beep.h"

void Beep_Init( )
{
    GPIO_InitTypeDef GPIO_IS;
    GPIO_StructInit( &GPIO_IS );

    RCC_AHB1PeriphClockCmd( RCC_BEEP, ENABLE );

    GPIO_IS.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_IS.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_IS.GPIO_OType = GPIO_OType_PP;
    GPIO_IS.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_IS.GPIO_Pin   = Pin_BEEP;
    GPIO_Init( GPIO_BEEP, &GPIO_IS );

    GPIO_ResetBits( GPIO_BEEP, Pin_BEEP );
}


void Audio_Init( )
{
    GPIO_InitTypeDef GPIO_IS;
    GPIO_StructInit( &GPIO_IS );

    RCC_AHB1PeriphClockCmd( RCC_BEEP, ENABLE );

    GPIO_IS.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_IS.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_IS.GPIO_OType = GPIO_OType_PP;
    GPIO_IS.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_IS.GPIO_Pin   = Pin_AUDIO;
    GPIO_Init( GPIO_AUDIO, &GPIO_IS );

    GPIO_ResetBits( GPIO_AUDIO, Pin_AUDIO );
}
