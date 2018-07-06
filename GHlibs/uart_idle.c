#include "uart_idle.h"


/*** Pringf函数 重定向 ***/
int fputc(int ch, FILE *f)
{
  uint8_t temp[1] = {ch};
  HAL_UART_Transmit(&huart1, temp, 1, 2); /*重定向到UART1 debug端口 */
  return 1;
}


/*** 变量定义 ***/
volatile uint8_t Uart1_Rx_Buffer[Uart1_Rx_BufferSize] = {0};
volatile uint8_t Uart1_Rcv_Cplt_Flag = 0x00;
volatile uint32_t Uart1_Rcv_Len = 0x00;

volatile uint8_t Uart2_Rx_Buffer[Uart2_Rx_BufferSize] = {0};
volatile uint8_t Uart2_Rcv_Cplt_Flag = 0x00;
volatile uint32_t Uart2_Rcv_Len = 0x00;

volatile uint8_t Uart3_Rx_Buffer[Uart3_Rx_BufferSize] = {0};
volatile uint8_t Uart3_Rcv_Cplt_Flag = 0x00;
volatile uint32_t Uart3_Rcv_Len = 0x00;


/*** 串口3 DMA 空闲中断 ***/
void USART3_IRQHandler(void)
{
	uint32_t tmp_flag = 0x00;
	uint32_t temp;
  /* HAL_UART_IRQHandler(&huart3); */
	tmp_flag = __HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE);        							  	/* 取串口3空闲中断标志 */
	
	if (tmp_flag != RESET)                                           									/* 串口3空闲中断 */
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart3);                            									/* 串口3空闲中断标志复位 */
		temp = huart3.Instance->SR;                                    									/* 读串口3状态寄存器 */
		temp = huart3.Instance->DR;                                    									/* 读串口3数据寄存器 */
		HAL_UART_DMAStop(&huart3);                             				 									/* 停止串口3DMA收发 */
		temp = hdma_usart3_rx.Instance->CNDTR;                     		 									/* 读取串口3可接收字符数 */
		Uart3_Rcv_Len = (uint32_t)Uart3_Rx_BufferSize - temp;          									/* 串口3空闲中断接收的string长度 */
    Uart3_Rcv_Cplt_Flag = 1;                                       									/* 串口3空闲中断接收完成标志位 */
		
		#if uart_debug                                                      						/* 串口3 debug */
		printf("Uart3_Rx_Buffer:[%s]\r\n", Uart3_Rx_Buffer);           									/* 打印串口3内容 */
		printf("temp:[%d]\r\n", temp);                                 									/* 打印串口3可接收字符数量 */
		printf("Uart3_Rcv_Len:[%d]\r\n", Uart3_Rcv_Len);               									/* 打印串口3接收字符串长度 */
    /* HAL_UART_Transmit(&huart1, (uint8_t *)Uart3_Rx_Buffer, Uart3_Rcv_Len, 100); */  	/* 打印串口3接收长度的字符串 */
		#endif
		at_receive_handle((uint8_t *)Uart3_Rx_Buffer,Uart3_Rcv_Len);
		// memset((uint8_t *)Uart3_Rx_Buffer, 0x00, Uart3_Rcv_Len);                       /* 清空串口3接收buffer */
    HAL_UART_Receive_DMA(&huart3, (uint8_t *)Uart3_Rx_Buffer, Uart3_Rx_BufferSize); /*重新打开串口3 DMA IDLE */
	}
}



