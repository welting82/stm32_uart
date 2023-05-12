// ref: https://microcontrollerslab.com/uart-usart-communication-stm32f4-discovery-board-hal-uart-driver/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_usart.h"
#include "stm32f4xx_hal_gpio.h"
#include <string.h>

/*Function prototype for delay and UART2 configuration functions */
void UART2_Configuration(void);
void Delay_ms(volatile int time_ms);

USART_HandleTypeDef UART_Handler; /*Create UART_InitTypeDef struct instance */
char Message[] = "Welcome to Microcontrollers Lab\r\n"; /* Message to be transmitted through UART */

int main(void)
{
	HAL_Init(); /* HAL library initialization */
	UART2_Configuration(); /* Call UART2 initialization define below */
	while(1)
	{
		HAL_USART_Transmit(&UART_Handler, (uint8_t *)Message, strlen(Message), 10);
		Delay_ms(100);
	}
}

void UART2_Configuration(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE(); /* Enable clock to PORTA - UART2 pins PA2 and PA3 */
	__HAL_RCC_USART2_CLK_ENABLE(); /* Enable clock to UART2 module */
	
	GPIO_InitTypeDef UART2_GPIO_Handler; /*Create GPIO_InitTypeDef struct instance */
	UART2_GPIO_Handler.Pin = GPIO_PIN_2 | GPIO_PIN_3; 
	UART2_GPIO_Handler.Mode = GPIO_MODE_AF_PP;
	UART2_GPIO_Handler.Pull = GPIO_PULLUP;
	UART2_GPIO_Handler.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	UART2_GPIO_Handler.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &UART2_GPIO_Handler);
	//UART Configuration
	UART_Handler.Instance = USART2;
	UART_Handler.Init.BaudRate = 115200;
	UART_Handler.Init.Mode = UART_MODE_TX_RX;
	UART_Handler.Init.WordLength = UART_WORDLENGTH_8B;
	UART_Handler.Init.StopBits = UART_STOPBITS_1;
	HAL_USART_Init(&UART_Handler);	
}

/*Generate ms */
void Delay_ms(volatile int time_ms)
{
	      int j;
        for(j = 0; j < time_ms*4000; j++)
            {}  /* excute NOP for 1ms */
}