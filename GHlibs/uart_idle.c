#include "uart_idle.h"


/*** Pringf���� �ض��� ***/
int fputc(int ch, FILE *f)
{
  uint8_t temp[1] = {ch};
  HAL_UART_Transmit(&huart1, temp, 1, 2); /*�ض���UART1 debug�˿� */
  return 1;
}


/*** �������� ***/
volatile uint8_t Uart1_Rx_Buffer[Uart1_Rx_BufferSize] = {0};
volatile uint8_t Uart1_Rcv_Cplt_Flag = 0x00;
volatile uint32_t Uart1_Rcv_Len = 0x00;

volatile uint8_t Uart2_Rx_Buffer[Uart2_Rx_BufferSize] = {0};
volatile uint8_t Uart2_Rcv_Cplt_Flag = 0x00;
volatile uint32_t Uart2_Rcv_Len = 0x00;

volatile uint8_t Uart3_Rx_Buffer[Uart3_Rx_BufferSize] = {0};
volatile uint8_t Uart3_Rcv_Cplt_Flag = 0x00;
volatile uint32_t Uart3_Rcv_Len = 0x00;


/*** ����3 DMA �����ж� ***/
void USART3_IRQHandler(void)
{
	uint32_t tmp_flag = 0x00;
	uint32_t temp;
  /* HAL_UART_IRQHandler(&huart3); */
	tmp_flag = __HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE);        							  	/* ȡ����3�����жϱ�־ */
	
	if (tmp_flag != RESET)                                           									/* ����3�����ж� */
	{
		__HAL_UART_CLEAR_IDLEFLAG(&huart3);                            									/* ����3�����жϱ�־��λ */
		temp = huart3.Instance->SR;                                    									/* ������3״̬�Ĵ��� */
		temp = huart3.Instance->DR;                                    									/* ������3���ݼĴ��� */
		HAL_UART_DMAStop(&huart3);                             				 									/* ֹͣ����3DMA�շ� */
		temp = hdma_usart3_rx.Instance->CNDTR;                     		 									/* ��ȡ����3�ɽ����ַ��� */
		Uart3_Rcv_Len = (uint32_t)Uart3_Rx_BufferSize - temp;          									/* ����3�����жϽ��յ�string���� */
    Uart3_Rcv_Cplt_Flag = 1;                                       									/* ����3�����жϽ�����ɱ�־λ */
		
		#if uart_debug                                                      						/* ����3 debug */
		printf("Uart3_Rx_Buffer:[%s]\r\n", Uart3_Rx_Buffer);           									/* ��ӡ����3���� */
		printf("temp:[%d]\r\n", temp);                                 									/* ��ӡ����3�ɽ����ַ����� */
		printf("Uart3_Rcv_Len:[%d]\r\n", Uart3_Rcv_Len);               									/* ��ӡ����3�����ַ������� */
    /* HAL_UART_Transmit(&huart1, (uint8_t *)Uart3_Rx_Buffer, Uart3_Rcv_Len, 100); */  	/* ��ӡ����3���ճ��ȵ��ַ��� */
		#endif
		at_receive_handle((uint8_t *)Uart3_Rx_Buffer,Uart3_Rcv_Len);
		// memset((uint8_t *)Uart3_Rx_Buffer, 0x00, Uart3_Rcv_Len);                       /* ��մ���3����buffer */
    HAL_UART_Receive_DMA(&huart3, (uint8_t *)Uart3_Rx_Buffer, Uart3_Rx_BufferSize); /*���´򿪴���3 DMA IDLE */
	}
}



