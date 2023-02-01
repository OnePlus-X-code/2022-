/******************** (C) COPYRIGHT 2017 ANO Tech
 ********************************* ??    :???? ??    :www.anotc.com
 * ??    :anotc.taobao.com
 * ??Q? :190169595
 * ??    :????
 **********************************************************************************/

#include "usart.h"
//#include "Drv_BlueTooth.h"
//#include "Drv_Wave.h"
#include "Drv_Screen.h"
#include "Drv_FC.h"
#include "Drv_Uwb.h"

void NoUse( u8 data ) {}
//??????????,????????????,???????????????????,???????????
#define U1GetOneByte FC_GetOneByte
#define U2GetOneByte Screen_GetOneByte
#define U3GetOneByte UWB_GetOneByte
#define U4GetOneByte NoUse
#define U5GetOneByte NoUse
#define U6GetOneByte NoUse
//====uart1
void DrvUart1Init( u32 br_num )
{
    USART_InitTypeDef      USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef       NVIC_InitStructure;
    GPIO_InitTypeDef       GPIO_InitStructure;

    USART_StructInit( &USART_InitStructure );

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE );    //??USART1??
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );

    //???????
    NVIC_InitStructure.NVIC_IRQChannel                   = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART1_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = NVIC_UART1_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    GPIO_PinAFConfig( GPIOA, GPIO_PinSource9, GPIO_AF_USART1 );
    GPIO_PinAFConfig( GPIOA, GPIO_PinSource10, GPIO_AF_USART1 );

    //??PA9??USART1 Tx
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    //??PA10??USART1 Rx
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    //
    USART_DeInit( USART1 );
    //??USART1
    //??????
    USART_InitStructure.USART_BaudRate = br_num;    //????????????
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    // 8???
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //??????1????
    USART_InitStructure.USART_Parity = USART_Parity_No;    //??????
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;    //???????
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    //???????
    //??USART1??
    USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;    //???????
    USART_ClockInitStruct.USART_CPOL =
        USART_CPOL_Low;    // SLCK??????????->???
    USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;    //?????????????
    USART_ClockInitStruct.USART_LastBit =
        USART_LastBit_Disable;    //?????????????SCLK??

    USART_Init( USART1, &USART_InitStructure );
    USART_ClockInit( USART1, &USART_ClockInitStruct );

    //??USART1????
    USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );
    //??USART1
    USART_Cmd( USART1, ENABLE );
}

u8   Tx1Buffer[256];
u8   Tx1Counter = 0;
u8   count1     = 0;
void DrvUart1SendBuf( unsigned char* DataToSend, u8 data_num )
{
    u8 i;
    for ( i = 0; i < data_num; i++ ) { Tx1Buffer[count1++] = *( DataToSend + i ); }

    if ( !( USART1->CR1 & USART_CR1_TXEIE ) )
    {
        USART_ITConfig( USART1, USART_IT_TXE, ENABLE );    //??????
    }
}

u8 U1RxDataTmp[100];
u8 U1RxInCnt  = 0;
u8 U1RxoutCnt = 0;

void drvU1GetByte( u8 data )
{
    U1RxDataTmp[U1RxInCnt++] = data;
    if ( U1RxInCnt >= 100 )
        U1RxInCnt = 0;
}

void drvU1DataCheck( void )
{
    while ( U1RxInCnt != U1RxoutCnt )
    {
        U1GetOneByte( U1RxDataTmp[U1RxoutCnt++] );
        if ( U1RxoutCnt >= 100 )
            U1RxoutCnt = 0;
    }
}

void Usart1_IRQ( void )
{
    u8 com_data;

    if ( USART1->SR & USART_SR_ORE )    // ORE??
    {
        com_data = USART1->DR;
    }
    //????
    if ( USART_GetITStatus( USART1, USART_IT_RXNE ) )
    {
        USART_ClearITPendingBit( USART1, USART_IT_RXNE );    //??????
        com_data = USART1->DR;
        drvU1GetByte( com_data );
    }
    //??(????)??
    if ( USART_GetITStatus( USART1, USART_IT_TXE ) )
    {
        USART1->DR = Tx1Buffer[Tx1Counter++];    //?DR??????
        if ( Tx1Counter == count1 )
        {
            USART1->CR1 &= ~USART_CR1_TXEIE;    //??TXE(????)??
        }
    }
}

//====uart2
void DrvUart2Init( u32 br_num )
{
    USART_InitTypeDef      USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef       NVIC_InitStructure;
    GPIO_InitTypeDef       GPIO_InitStructure;

    USART_StructInit( &USART_InitStructure );

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE );    //??USART2??
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE );

    //???????
    NVIC_InitStructure.NVIC_IRQChannel                   = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART2_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = NVIC_UART2_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    GPIO_PinAFConfig( GPIOD, GPIO_PinSource5, GPIO_AF_USART2 );
    GPIO_PinAFConfig( GPIOD, GPIO_PinSource6, GPIO_AF_USART2 );

    //??PD5??USART2 Tx
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init( GPIOD, &GPIO_InitStructure );
    //??PD6??USART2 Rx
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    //??USART2
    //??????
    USART_InitStructure.USART_BaudRate = br_num;    //????????????
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    // 8???
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //??????1????
    USART_InitStructure.USART_Parity = USART_Parity_No;    //??????
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;    //???????
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    //???????
    //??USART2??
    USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;    //???????
    USART_ClockInitStruct.USART_CPOL =
        USART_CPOL_Low;    // SLCK??????????->???
    USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;    //?????????????
    USART_ClockInitStruct.USART_LastBit =
        USART_LastBit_Disable;    //?????????????SCLK??

    USART_Init( USART2, &USART_InitStructure );
    USART_ClockInit( USART2, &USART_ClockInitStruct );

    //??USART2????
    USART_ITConfig( USART2, USART_IT_RXNE, ENABLE );
    //??USART2
    USART_Cmd( USART2, ENABLE );
}

u8 Tx2Buffer[256];
u8 Tx2Counter = 0;
u8 count      = 0;

void DrvUart2SendBuf( unsigned char* DataToSend, u8 data_num )
{
    u8 i;
    for ( i = 0; i < data_num; i++ ) { Tx2Buffer[count++] = *( DataToSend + i ); }
    if ( !( USART2->CR1 & USART_CR1_TXEIE ) )
    {
        USART_ITConfig( USART2, USART_IT_TXE, ENABLE );    //??????
    }
}

u8 U2RxDataTmp[100];
u8 U2RxInCnt  = 0;
u8 U2RxoutCnt = 0;

void drvU2GetByte( u8 data )
{
    U2RxDataTmp[U2RxInCnt++] = data;
    if ( U2RxInCnt >= 100 )
        U2RxInCnt = 0;
}

void drvU2DataCheck( void )
{
    while ( U2RxInCnt != U2RxoutCnt )
    {
        U2GetOneByte( U2RxDataTmp[U2RxoutCnt++] );
        if ( U2RxoutCnt >= 100 )
            U2RxoutCnt = 0;
    }
}

void Usart2_IRQ( void )
{
    u8 com_data;

    if ( USART2->SR & USART_SR_ORE )    // ORE??
    {
        com_data = USART2->DR;
    }

    //????
    if ( USART_GetITStatus( USART2, USART_IT_RXNE ) )
    {
        USART_ClearITPendingBit( USART2, USART_IT_RXNE );    //??????
        com_data = USART2->DR;
        drvU2GetByte( com_data );
    }
    //??(????)??
    if ( USART_GetITStatus( USART2, USART_IT_TXE ) )
    {
        USART2->DR = Tx2Buffer[Tx2Counter++];    //?DR??????
        if ( Tx2Counter == count )
        {
            USART2->CR1 &= ~USART_CR1_TXEIE;    //??TXE(????)??
        }
    }
}

//====uart3
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DrvUart3Init( u32 br_num )
{
    USART_InitTypeDef      USART_InitStructure;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef       NVIC_InitStructure;
    GPIO_InitTypeDef       GPIO_InitStructure;

    USART_StructInit( &USART_InitStructure );

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE );    //??USART2??
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE );

    //???????
    NVIC_InitStructure.NVIC_IRQChannel                   = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART3_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = NVIC_UART3_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    GPIO_PinAFConfig( GPIOB, GPIO_PinSource10, GPIO_AF_USART3 );
    GPIO_PinAFConfig( GPIOB, GPIO_PinSource11, GPIO_AF_USART3 );

    //??PD5??USART2 Tx
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init( GPIOB, &GPIO_InitStructure );
    //??PD6??USART2 Rx
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init( GPIOB, &GPIO_InitStructure );

    //??USART2
    //??????
    USART_InitStructure.USART_BaudRate = br_num;    //????????????
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    // 8???
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //??????1????
    USART_InitStructure.USART_Parity = USART_Parity_No;    //??????
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;    //???????
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    //???????
    //??USART2??
    USART_ClockInitStruct.USART_Clock = USART_Clock_Disable;    //???????
    USART_ClockInitStruct.USART_CPOL =
        USART_CPOL_Low;    // SLCK??????????->???
    USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge;    //?????????????
    USART_ClockInitStruct.USART_LastBit =
        USART_LastBit_Disable;    //?????????????SCLK??

    USART_Init( USART3, &USART_InitStructure );
    USART_ClockInit( USART3, &USART_ClockInitStruct );

    //??USART2????
    USART_ITConfig( USART3, USART_IT_RXNE, ENABLE );
    //??USART2
    USART_Cmd( USART3, ENABLE );
}

u8 Tx3Buffer[256];
u8 Tx3Counter = 0;
u8 count3     = 0;

void DrvUart3SendBuf( unsigned char* DataToSend, u8 data_num )
{
    u8 i;
    for ( i = 0; i < data_num; i++ ) { Tx3Buffer[count3++] = *( DataToSend + i ); }
    if ( !( USART3->CR1 & USART_CR1_TXEIE ) )
    {
        USART_ITConfig( USART3, USART_IT_TXE, ENABLE );    //??????
    }
}

u8 U3RxDataTmp[100];
u8 U3RxInCnt  = 0;
u8 U3RxoutCnt = 0;

void drvU3GetByte( u8 data )
{
    U3RxDataTmp[U3RxInCnt++] = data;
    if ( U3RxInCnt >= 100 )
        U3RxInCnt = 0;
}

void drvU3DataCheck( void )
{
    while ( U3RxInCnt != U3RxoutCnt )
    {
        U3GetOneByte( U3RxDataTmp[U3RxoutCnt++] );
        if ( U3RxoutCnt >= 100 )
            U3RxoutCnt = 0;
    }
}

void Usart3_IRQ( void )
{
    u8 com_data;

    if ( USART3->SR & USART_SR_ORE )    // ORE??
        com_data = USART3->DR;

    //????
    if ( USART_GetITStatus( USART3, USART_IT_RXNE ) )
    {
        USART_ClearITPendingBit( USART3, USART_IT_RXNE );    //??????
        com_data = USART3->DR;
        drvU3GetByte( com_data );
    }

    //??(????)??
    if ( USART_GetITStatus( USART3, USART_IT_TXE ) )
    {
        USART3->DR = Tx3Buffer[Tx3Counter++];    //?DR??????
        if ( Tx3Counter == count3 )
        {
            USART3->CR1 &= ~USART_CR1_TXEIE;    //??TXE(????)??
        }
    }
}

//====uart4
void DrvUart4Init( u32 br_num )
{
    USART_InitTypeDef USART_InitStructure;
    // USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    USART_StructInit( &USART_InitStructure );

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4, ENABLE );    //??USART2??
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE );

    //???????
    NVIC_InitStructure.NVIC_IRQChannel                   = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART4_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = NVIC_UART4_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    GPIO_PinAFConfig( GPIOA, GPIO_PinSource0, GPIO_AF_UART4 );
    GPIO_PinAFConfig( GPIOA, GPIO_PinSource1, GPIO_AF_UART4 );

    //??PC12??UART5 Tx
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init( GPIOA, &GPIO_InitStructure );
    //??PD2??UART5 Rx
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init( GPIOA, &GPIO_InitStructure );

    //??UART5
    //??????
    USART_InitStructure.USART_BaudRate = br_num;    //????????????
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    // 8???
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //??????1????
    USART_InitStructure.USART_Parity = USART_Parity_No;    //??????
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;    //???????
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    //???????
    USART_Init( UART4, &USART_InitStructure );

    //??UART5????
    USART_ITConfig( UART4, USART_IT_RXNE, ENABLE );
    //??USART5
    USART_Cmd( UART4, ENABLE );
}

u8 Tx4Buffer[256];
u8 Tx4Counter = 0;
u8 count4     = 0;

void DrvUart4SendBuf( unsigned char* DataToSend, u8 data_num )
{
    u8 i;
    for ( i = 0; i < data_num; i++ ) { Tx4Buffer[count4++] = *( DataToSend + i ); }

    if ( !( UART4->CR1 & USART_CR1_TXEIE ) )
    {
        USART_ITConfig( UART4, USART_IT_TXE, ENABLE );    //??????
    }
}

u8 U4RxDataTmp[100];
u8 U4RxInCnt  = 0;
u8 U4RxoutCnt = 0;

void drvU4GetByte( u8 data )
{
    U4RxDataTmp[U4RxInCnt++] = data;
    if ( U4RxInCnt >= 100 )
        U4RxInCnt = 0;
}

void drvU4DataCheck( void )
{
    while ( U4RxInCnt != U4RxoutCnt )
    {
        U4GetOneByte( U4RxDataTmp[U4RxoutCnt++] );
        if ( U4RxoutCnt >= 100 )
            U4RxoutCnt = 0;
    }
}

void Uart4_IRQ( void )
{
    u8 com_data;

    if ( UART4->SR & USART_SR_ORE )    // ORE??
    {
        com_data = UART4->DR;
    }
    // ????
    if ( USART_GetITStatus( UART4, USART_IT_RXNE ) )
    {
        USART_ClearITPendingBit( UART4, USART_IT_RXNE );    //??????
        com_data = UART4->DR;
        drvU4GetByte( com_data );
    }

    // ??(????)??
    if ( USART_GetITStatus( UART4, USART_IT_TXE ) )
    {
        UART4->DR = Tx4Buffer[Tx4Counter++];    //?DR??????
        if ( Tx4Counter == count4 )
        {
            UART4->CR1 &= ~USART_CR1_TXEIE;    //??TXE(????)??
        }
    }
}

//====uart5
void DrvUart5Init( u32 br_num )
{
    USART_InitTypeDef USART_InitStructure;
    // USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    USART_StructInit( &USART_InitStructure );

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5, ENABLE );    //??USART2??
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE );

    //???????
    NVIC_InitStructure.NVIC_IRQChannel                   = UART5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART5_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = NVIC_UART5_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    GPIO_PinAFConfig( GPIOC, GPIO_PinSource12, GPIO_AF_UART5 );
    GPIO_PinAFConfig( GPIOD, GPIO_PinSource2, GPIO_AF_UART5 );

    //??PC12??UART5 Tx
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init( GPIOC, &GPIO_InitStructure );
    //??PD2??UART5 Rx
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init( GPIOD, &GPIO_InitStructure );

    //??UART5
    //??????
    USART_InitStructure.USART_BaudRate = br_num;    //????????????
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    // 8???
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //??????1????
    USART_InitStructure.USART_Parity = USART_Parity_No;    //??????
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;    //???????
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    //???????
    USART_Init( UART5, &USART_InitStructure );

    //??UART5????
    USART_ITConfig( UART5, USART_IT_RXNE, ENABLE );
    //??USART5
    USART_Cmd( UART5, ENABLE );
}

u8 Tx5Buffer[256];
u8 Tx5Counter = 0;
u8 count5     = 0;

void DrvUart5SendBuf( unsigned char* DataToSend, u8 data_num )
{
    u8 i;
    for ( i = 0; i < data_num; i++ ) { Tx5Buffer[count5++] = *( DataToSend + i ); }

    if ( !( UART5->CR1 & USART_CR1_TXEIE ) )
    {
        USART_ITConfig( UART5, USART_IT_TXE, ENABLE );    //??????
    }
}

u8 U5RxDataTmp[100];
u8 U5RxInCnt  = 0;
u8 U5RxoutCnt = 0;

void drvU5GetByte( u8 data )
{
    U5RxDataTmp[U5RxInCnt++] = data;
    if ( U5RxInCnt >= 100 )
        U5RxInCnt = 0;
}

void drvU5DataCheck( void )
{
    while ( U5RxInCnt != U5RxoutCnt )
    {
        U5GetOneByte( U5RxDataTmp[U5RxoutCnt++] );
        if ( U5RxoutCnt >= 100 )
            U5RxoutCnt = 0;
    }
}

void Uart5_IRQ( void )
{
    u8 com_data;

    if ( UART5->SR & USART_SR_ORE )    // ORE??
    {
        com_data = UART5->DR;
    }
    //????
    if ( USART_GetITStatus( UART5, USART_IT_RXNE ) )
    {
        USART_ClearITPendingBit( UART5, USART_IT_RXNE );    //??????
        com_data = UART5->DR;
        drvU5GetByte( com_data );
    }
    //??(????)??
    if ( USART_GetITStatus( UART5, USART_IT_TXE ) )
    {
        UART5->DR = Tx5Buffer[Tx5Counter++];    //?DR??????
        if ( Tx5Counter == count5 )
        {
            UART5->CR1 &= ~USART_CR1_TXEIE;    //??TXE(????)??
        }
    }
}


//====uart6
void DrvUart6Init( u32 br_num )
{
    USART_InitTypeDef USART_InitStructure;
    // USART_ClockInitTypeDef USART_ClockInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    USART_StructInit( &USART_InitStructure );

    RCC_APB1PeriphClockCmd( RCC_APB2Periph_USART6, ENABLE );    //??USART2??
    RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE );

    //???????
    NVIC_InitStructure.NVIC_IRQChannel                   = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_UART6_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = NVIC_UART6_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init( &NVIC_InitStructure );

    GPIO_PinAFConfig( GPIOC, GPIO_PinSource6, GPIO_AF_USART6 );
    GPIO_PinAFConfig( GPIOC, GPIO_PinSource7, GPIO_AF_USART6 );

    //??PC6??USART6 Tx
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init( GPIOC, &GPIO_InitStructure );
    //??PC7??USART6 Rx
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init( GPIOC, &GPIO_InitStructure );

    //??UART6
    //??????
    USART_InitStructure.USART_BaudRate = br_num;    //????????????
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;    // 8???
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    //??????1????
    USART_InitStructure.USART_Parity = USART_Parity_No;    //??????
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;    //???????
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;    //???????
    USART_Init( USART6, &USART_InitStructure );

    //??UART5????
    USART_ITConfig( USART6, USART_IT_RXNE, ENABLE );
    //??USART5
    USART_Cmd( USART6, ENABLE );
}

u8 Tx6Buffer[256];
u8 Tx6Counter = 0;
u8 count6     = 0;

void DrvUart6SendBuf( unsigned char* DataToSend, u8 data_num )
{
    u8 i;
    for ( i = 0; i < data_num; i++ ) { Tx6Buffer[count6++] = *( DataToSend + i ); }

    if ( !( USART6->CR1 & USART_CR1_TXEIE ) )
    {
        USART_ITConfig( USART6, USART_IT_TXE, ENABLE );    //??????
    }
}

u8 U6RxDataTmp[100];
u8 U6RxInCnt  = 0;
u8 U6RxoutCnt = 0;

void drvU6GetByte( u8 data )
{
    U6RxDataTmp[U6RxInCnt++] = data;
    if ( U6RxInCnt >= 100 )
        U6RxInCnt = 0;
}

void drvU6DataCheck( void )
{
    while ( U6RxInCnt != U6RxoutCnt )
    {
        U6GetOneByte( U6RxDataTmp[U6RxoutCnt++] );
        if ( U6RxoutCnt >= 100 )
            U6RxoutCnt = 0;
    }
}

void Uart6_IRQ( void )
{
    u8 com_data;

    if ( USART6->SR & USART_SR_ORE )    // ORE??
    {
        com_data = USART6->DR;
    }
    //????
    if ( USART_GetITStatus( USART6, USART_IT_RXNE ) )
    {
        USART_ClearITPendingBit( USART6, USART_IT_RXNE );    //??????
        com_data = USART6->DR;
        drvU6GetByte( com_data );
    }
    //??(????)??
    if ( USART_GetITStatus( USART6, USART_IT_TXE ) )
    {
        USART6->DR = Tx6Buffer[Tx6Counter++];    //?DR??????
        if ( Tx6Counter == count6 )
        {
            USART6->CR1 &= ~USART_CR1_TXEIE;    //??TXE(????)??
        }
    }
}
void DrvUartDataCheck( void )
{
    drvU1DataCheck( );
    drvU2DataCheck( );
    drvU3DataCheck( );
    drvU4DataCheck( );
    drvU5DataCheck( );
    drvU6DataCheck( );
}
/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
