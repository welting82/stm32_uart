#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>

#define MAX_DATA_SIZE 100
/*Function prototype for delay and UART2 configuration functions */
void UART2_Configuration(void);
void Clock_Configuration(void);
void Delay_ms(volatile int time_ms);

UART_HandleTypeDef huart2; /*Create UART_InitTypeDef struct instance */
DMA_HandleTypeDef hdma_usart2_rx;
char RX_Buffer[30]; //"Interrupt input"
char TX_Buffer[50] = "Hello World\r\n";

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t RX_Size)
{
	strcat(RX_Buffer,"\r\n");
	HAL_UART_Transmit(&huart2, (uint8_t*)&RX_Buffer, RX_Size+2,0xFFFF);
	memset(RX_Buffer,0,sizeof(RX_Buffer));
	HAL_UARTEx_ReceiveToIdle_DMA(huart, (uint8_t*)&RX_Buffer, sizeof(RX_Buffer));
}

int main(void)
{
	HAL_Init(); /* HAL library initialization */
	Clock_Configuration();
	UART2_Configuration(); /* Call UART2 initialization define below */
	HAL_UART_Transmit(&huart2, (uint8_t*)&TX_Buffer, sizeof(TX_Buffer),0xffff);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, (uint8_t*)&RX_Buffer, sizeof(RX_Buffer));
	while(1) {}
}

void UART2_Configuration(void)
{
	//UART Configuration
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 921600;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	HAL_UART_Init(&huart2);
}

void Clock_Configuration(void)
{
    __HAL_RCC_USART2_CLK_ENABLE(); /* Enable clock to UART2 module */

    // PLL Configuration
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

}

/*Generate ms */
void Delay_ms(volatile int time_ms)
{
  int j;
  for(j = 0; j < time_ms*4000; j++)
    {}  /* excute NOP for 1ms */
}