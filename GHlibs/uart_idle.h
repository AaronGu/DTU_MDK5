#ifndef _UART_IDLE_
#define _UART_IDLE_
#include "stm32f1xx_hal.h"
#include "stm32f1xx_it.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"


/*** 串口定义 ***/
#define Uart1_Rx_BufferSize                      (256)    /* 串口1接收buffer长度*/
#define Uart2_Rx_BufferSize                      (256)    /* 串口2接收buffer长度*/
#define Uart3_Rx_BufferSize                      (256)    /* 串口3接收buffer长度*/
#define uart_debug                               (0)      /* 串口debug模式 */


/*** 变量声明 ***/
volatile extern uint8_t Uart1_Rx_Buffer[];
volatile extern uint8_t Uart1_Rcv_Cplt_Flag;
volatile extern uint32_t Uart1_Rcv_Len;

volatile extern uint8_t Uart2_Rx_Buffer[];
volatile extern uint8_t Uart2_Rcv_Cplt_Flag;
volatile extern uint32_t Uart2_Rcv_Len;

volatile extern uint8_t Uart3_Rx_Buffer[];
volatile extern uint8_t Uart3_Rcv_Cplt_Flag;
volatile extern uint32_t Uart3_Rcv_Len;

extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;


/*** 串口接收中断函数声明 ***/
//void USART1_IRQHandler(void);
//void USART2_IRQHandler(void);
void USART3_IRQHandler(void);



#endif  // _UART_IDLE_

