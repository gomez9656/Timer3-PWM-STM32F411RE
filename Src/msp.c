/*
 * msp.c
 *
 *  Created on: 23/01/2021
 *      Author: PC
 */

#include "main.h"

void HAL_MspInit(void){
	//Low level initializations

	//1. Set up priority grouping
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	//2. Enable the required system exceptions
	SCB->SHCSR |= 0x7 << 16; //usage fault, memory fault and bus fault system exception

	//3. Configure the priority for the system exception
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim){

	GPIO_InitTypeDef tim3OC_ch_gpio;
	//!. Enable the peripheral clock for the timer 3 and GPIOB
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	//2. Configure GPIOB to behave as TIMER 3 channel 1, 2, 3 and 4
	/*
	PB0 -> tim3_ch3
	PB1	-> tim3_ch4
	PB4	-> tim3_ch1
	pb5	-> tim3_ch2
	*/

	tim3OC_ch_gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5;
	tim3OC_ch_gpio.Mode = GPIO_MODE_AF_PP;
	tim3OC_ch_gpio.Pull = GPIO_NOPULL;
	tim3OC_ch_gpio.Speed = GPIO_SPEED_FREQ_LOW;
	tim3OC_ch_gpio.Alternate = GPIO_AF2_TIM3;

	HAL_GPIO_Init(GPIOB, &tim3OC_ch_gpio);

	//3. NVIC settings
	HAL_NVIC_SetPriority(TIM3_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart){

	GPIO_InitTypeDef gpio_uart;

	//1. Enable the clock for USART2 and GPIOA peripheral
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//2. Pin muxing configuration
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2; //USART2_TX

	HAL_GPIO_Init(GPIOA, &gpio_uart); //Initialize GPIOA2 with gpio_uart configuration

	gpio_uart.Pin = GPIO_PIN_3;	//USART2_RX
	HAL_GPIO_Init(GPIOA, &gpio_uart); //Initialize GPIOA3 with gpio_uart configuration

	//3. Enable the IRQ in the NVIC and set up priority
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);

}

