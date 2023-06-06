#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdio.h>

#define MAX_DATA_SIZE 100
/*Function prototype for delay and UART2 configuration functions */
void UART2_Configuration(void);
void Clock_Configuration(void);
void LED3_Configuration(void);
void BTN_Configuration(void);
void Delay_ms(volatile int time_ms);

UART_HandleTypeDef huart2; /*Create UART_InitTypeDef struct instance */
char RX_Buffer; //"Interrupt input"
char rec_data[MAX_DATA_SIZE]; //"receice data"
char TX_Buffer[50] = "Hello World\r\n";
uint8_t Uart2_Rx_Cnt = 0;
uint8_t loop_cnt = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if((Uart2_Rx_Cnt == sizeof(rec_data)-2) || (RX_Buffer == '\r') || (RX_Buffer == '\n'))
	{
		rec_data[Uart2_Rx_Cnt] = '\r';
		rec_data[Uart2_Rx_Cnt+1] = '\n';

		HAL_UART_Transmit(&huart2, (uint8_t *)&rec_data, sizeof(rec_data),0xFFFF);
		if(Uart2_Rx_Cnt == sizeof(rec_data)-2) HAL_UART_Transmit(&huart2, (uint8_t *)"overflow\r\n", 10,0xFFFF);
		memset(rec_data,0,sizeof(rec_data));
		Uart2_Rx_Cnt = 0;
	}
	else
	{
		rec_data[Uart2_Rx_Cnt++] = RX_Buffer;
	}
	HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_13);
	HAL_UART_Receive_IT(huart, (uint8_t*)&RX_Buffer, 1);
}

int main(void)
{
	HAL_Init(); /* HAL library initialization */
	Clock_Configuration();
	LED3_Configuration();
	BTN_Configuration();
	UART2_Configuration(); /* Call UART2 initialization define below */
	HAL_UART_Transmit(&huart2, (uint8_t*)&TX_Buffer, sizeof(TX_Buffer),0xffff);
	HAL_UART_Receive_IT(&huart2, (uint8_t*)&RX_Buffer, 1);
	while(1)
	{
		if(!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
		{
			memset(TX_Buffer,0,sizeof(TX_Buffer));
			sprintf(TX_Buffer,"Loop Cnt: %d,  Rx Cnt: %d\r\n",loop_cnt, Uart2_Rx_Cnt);
			HAL_UART_Transmit(&huart2, (uint8_t*)&TX_Buffer, sizeof(TX_Buffer),0xFFFF);
			loop_cnt++;
			Delay_ms(250);
		}
	}
}

void BTN_Configuration(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE(); //Enable clock to GPIOA
	GPIO_InitTypeDef PushButton;  // declare a variable of type struct GPIO_InitTypeDef
	PushButton.Mode = GPIO_MODE_INPUT; // set pin mode to input
	PushButton.Pin = GPIO_PIN_0;  // select pin PA0 only
	PushButton.Pull = GPIO_NOPULL; // set no internal pull-up or pull-down resistor
	HAL_GPIO_Init(GPIOA, &PushButton); //  initialize PA0 pins by passing port name and address of PushButton struct
}

void LED3_Configuration(void)
{
    __HAL_RCC_GPIOG_CLK_ENABLE();
    GPIO_InitTypeDef LED3_GPIO_Handler;
    LED3_GPIO_Handler.Pin = GPIO_PIN_13;
    LED3_GPIO_Handler.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOG, &LED3_GPIO_Handler);
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