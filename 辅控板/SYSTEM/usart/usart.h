#ifndef _USART_H
#define _USART_H
#include "stm32f4xx.h"

#define NVIC_UART6_P 4    //??6????
#define NVIC_UART6_S 2

#define NVIC_UART5_P 1    //??5????
#define NVIC_UART5_S 0

#define NVIC_UART4_P 3    //??4????
#define NVIC_UART4_S 1

#define NVIC_UART3_P 2
#define NVIC_UART3_S 0

#define NVIC_UART2_P 4    //??2????
#define NVIC_UART2_S 1

#define NVIC_UART1_P 3    //??1???? //gps
#define NVIC_UART1_S 0

void DrvUart1Init( u32 br_num );
void Usart1_IRQ( void );
void DrvUart1SendBuf( unsigned char* DataToSend, u8 data_num );

// extern u8 Rx_Buf[];
void DrvUart2Init( u32 br_num );
void Usart2_IRQ( void );
void DrvUart2SendBuf( unsigned char* DataToSend, u8 data_num );

void DrvUart3Init( u32 br_num );
void Usart3_IRQ( void );
void DrvUart3SendBuf( unsigned char* DataToSend, u8 data_num );

void DrvUart4Init( u32 br_num );
void Uart4_IRQ( void );
void DrvUart4SendBuf( unsigned char* DataToSend, u8 data_num );

void DrvUart5Init( u32 br_num );
void Uart5_IRQ( void );
void DrvUart5SendBuf( unsigned char* DataToSend, u8 data_num );

void DrvUart6Init( u32 br_num );
void Uart6_IRQ( void );
void DrvUart6SendBuf( unsigned char* DataToSend, u8 data_num );

void DrvUartDataCheck( void );
#endif


