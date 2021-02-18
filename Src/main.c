/*
 * main.c

 *
 *  Created on: 23/01/2021
 *      Author: gomez9656
 *
 *      This program is to use ITM3 output channels 1,2,3 and 4 in the Output
 *      Capture Mode. This, to generate 500, 1000, 2000 and 4000 Hz signals
 */

#include "stm32f4xx.h"
#include "main.h"
#include <string.h>

/*
 * Functions prototypes
 */
void SystemCoreClockConfig(uint8_t clock_freq);
void Error_handler(void);
void GPIO_Init(void);
void TIMER3_Init(void);
void UART2_Init(void);

TIM_HandleTypeDef htimer3;
UART_HandleTypeDef huart2; //Handle of UART 2

/*
 * You can calculate it by doing 1) desired frequency * 2.
 * 2) Timer frequency / step 1
 */
uint32_t pulse1 = 25000; //This gives a value of 500 Hz in the output frequency
uint32_t pulse2 = 12500; //This gives a value of 1000 Hz in the output frequency
uint32_t pulse3 = 6250;	//This gives a value of 20000 Hz in the output frequency
uint32_t pulse4 = 3125;	//This gives a value of 4000 Hz in the output frequency

uint32_t ccr_content;

int main(){

	/* Basic initialization  */
	HAL_Init();

	SystemCoreClockConfig(SYS_CLOCK_FREQ_50_MHZ);

	GPIO_Init();

	UART2_Init();

	TIMER3_Init();


	while(1);

	return 0;
}

/*
 * Initialize Timer 3 in PWM Mode
 */
void TIMER3_Init(void){

	TIM_OC_InitTypeDef tim3PWM_Config;
	//Low level initialization
	htimer3.Instance = TIM3;
	htimer3.Init.Period = 0xFFFFFFFF;
	htimer3.Init.Prescaler = 1; //IF you choose system clock of 50MHz, with a prescaler of 1 it will generate a 25MHz signal

	if (HAL_TIM_PWM_Init(&htimer3) != HAL_OK){
		Error_handler();
	}

	//Low level initalization
	tim3PWM_Config.OCMode = TIM_OCMODE_PWM1;
	tim3PWM_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim3PWM_Config.Pulse = 0;


}


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){

		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, ccr_content + pulse1);
	}

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){

			ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
			__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, ccr_content + pulse2);
	}

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){

			ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
			__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, ccr_content + pulse3);
	}

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4){

			ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
			__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, ccr_content + pulse1);
	}
}

/*
 * You can use it when you need an specific clock
 * By default will be internal clock
 */
void SystemCoreClockConfig(uint8_t clock_freq){

	RCC_OscInitTypeDef Osc_Init;
	RCC_ClkInitTypeDef Clock_Init;

	Osc_Init.OscillatorType = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSE | RCC_OSCILLATORTYPE_HSE;
	Osc_Init.HSIState = RCC_HSI_ON;
	Osc_Init.LSEState = RCC_LSE_ON;
	Osc_Init.HSEState = RCC_HSE_ON;
	Osc_Init.HSICalibrationValue = 16;
	Osc_Init.PLL.PLLState = RCC_PLL_ON;
	Osc_Init.PLL.PLLSource = RCC_PLLSOURCE_HSI;

	switch(clock_freq){
	  case SYS_CLOCK_FREQ_50_MHZ:
			  Osc_Init.PLL.PLLM = 8;
			  Osc_Init.PLL.PLLN = 50;
			  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
			  Osc_Init.PLL.PLLQ = 2;
			  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
		                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
			  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
			  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
		     break;

		  case SYS_CLOCK_FREQ_84_MHZ:
			  Osc_Init.PLL.PLLM = 8;
			  Osc_Init.PLL.PLLN = 84;
			  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
			  Osc_Init.PLL.PLLQ = 2;
			  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
		                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
			  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV2;
			  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV1;
		     break;

		  case SYS_CLOCK_FREQ_120_MHZ:
			  Osc_Init.PLL.PLLM = 8;
			  Osc_Init.PLL.PLLN = 120;
			  Osc_Init.PLL.PLLP = RCC_PLLP_DIV2;
			  Osc_Init.PLL.PLLQ = 2;
			  Clock_Init.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
		                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
			  Clock_Init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
			  Clock_Init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			  Clock_Init.APB1CLKDivider = RCC_HCLK_DIV4;
			  Clock_Init.APB2CLKDivider = RCC_HCLK_DIV2;
		     break;

		  default:
		   return ;
		 }

			if (HAL_RCC_OscConfig(&Osc_Init) != HAL_OK)
		{
				Error_handler();
		}



		if (HAL_RCC_ClockConfig(&Clock_Init, FLASH_LATENCY_2) != HAL_OK)
		{
			Error_handler();
		}


		/*Configure the systick timer interrupt frequency (for every 1 ms) */
		uint32_t hclk_freq = HAL_RCC_GetHCLKFreq();
		HAL_SYSTICK_Config(hclk_freq/1000);

		/**Configure the Systick
		*/
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

		/* SysTick_IRQn interrupt configuration */
		HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

void UART2_Init(void){

	//1. Linking handle struct to base address
	huart2.Instance = USART2;

	//2. Init the handle variable
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;

	//3. Use the API to initialize
	if(HAL_UART_Init(&huart2) != HAL_OK){

		//There is a problem
		Error_handler();
	}
}

/*
 * Initialize LED in GPIO A5
 */
void GPIO_Init(void){

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef ledgpio;
	ledgpio.Pin = GPIO_PIN_5;
	ledgpio.Mode = GPIO_MODE_OUTPUT_PP;
	ledgpio.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &ledgpio);
}

void Error_handler(void){
	while(1);
}
