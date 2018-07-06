#include "ESP8266_driver.h"




void esp8266_init(void)
{
	HAL_GPIO_WritePin(WIFI_RST_GPIO_Port, WIFI_RST_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(WIFI_GPIO_0_GPIO_Port, WIFI_GPIO_0_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, GPIO_PIN_SET);
	printf("\r\nesp8266 init.\r\n");
	at_init();
	printf("\r\nat init.\r\n");
	Mem_Init();
	printf("\r\nMem init.\r\n");
//	if(HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
//	{
//		printf("TIM3 start error\r\n");
//	}
	
}


void esp8266_processing(void)
{
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart3, (uint8_t *)"at\r\n", strlen("at\r\n"), 100);
	
	
	
	
	
	
	
}

