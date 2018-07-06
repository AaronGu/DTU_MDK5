#ifndef _UART_IDLE_
#define _UART_IDLE_
#include "stm32f1xx_hal.h"
#include "stm32f1xx_it.h"
#include "stdio.h"
#include "usart.h"
#include "string.h"


/*** ���ڶ��� ***/
#define Uart1_Rx_BufferSize                      (256)    /* ����1����buffer����*/
#define Uart2_Rx_BufferSize                      (256)    /* ����2����buffer����*/
#define Uart3_Rx_BufferSize                      (256)    /* ����3����buffer����*/
#define uart_debug                               (0)      /* ����debugģʽ */


/*** �������� ***/
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


/*** ���ڽ����жϺ������� ***/
//void USART1_IRQHandler(void);
//void USART2_IRQHandler(void);
void USART3_IRQHandler(void);



#endif  // _UART_IDLE_

