/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"
#include "bsp_printer.h"
//#include "printer_bmp.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern void vInterrup_TIM14(void); 
/* USER CODE END PD */
int CountDataSerial = 0;
char FlagSerial0=0;
/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim14;
extern UART_HandleTypeDef UartHandle;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM14 global interrupt.
  */
void TIM14_IRQHandler(void)
{
  /* USER CODE BEGIN TIM14_IRQn 0 */

  /* USER CODE END TIM14_IRQn 0 */
  HAL_TIM_IRQHandler(&htim14);
  /* USER CODE BEGIN TIM14_IRQn 1 */
	
	vInterrup_TIM14();

  /* USER CODE END TIM14_IRQn 1 */
}

extern uint8_t aRxBuffer[];
/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void){
	uint8_t iDataRx = 0;	
	if((__HAL_UART_GET_IT(&UartHandle, UART_IT_RXNE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&UartHandle, UART_IT_RXNE) != RESET)) {
		HAL_UART_Receive(&UartHandle, &iDataRx, 1,0xff);

		BmpBufferRS232.CountBufferRS232++;
		if(BmpBufferRS232.CountBufferRS232>=SizeOfBufer232)
			BmpBufferRS232.CountBufferRS232=0;
		
		BmpBufferRS232.BufferRS232[BmpBufferRS232.CountBufferRS232]=iDataRx;
	}
}
void USAR00T1_IRQHandler00(void){
	uint8_t iDataRx = 0;	
	if((__HAL_UART_GET_IT(&UartHandle, UART_IT_RXNE) != RESET) && (__HAL_UART_GET_IT_SOURCE(&UartHandle, UART_IT_RXNE) != RESET)) {
		HAL_UART_Receive(&UartHandle, &iDataRx, 1, 0xFF);
		
		if(aRxBuffer[0]==0)
		  CountDataSerial=0;					
		//HAL_UART_Transmit(&UartHandle,&iDataRx,1,10);
		
		aRxBuffer[CountDataSerial]= iDataRx;
		
		if(iDataRx=='\0'||CountDataSerial==SizeOfBuferToPrint-1)
			FlagSerial0=1;	             		
		else
		  CountDataSerial++;		
	}
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
