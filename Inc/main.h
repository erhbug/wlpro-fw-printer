/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
	#include "string.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "printer_bmp.h"
#include "bsp_printer.h"
extern char FlagSerial0;
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define SizeOfBuferToPrint 512//121//64

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);
void LimpiarBuferRecepcion(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOTOR_I10_OUT_Pin GPIO_PIN_0
#define MOTOR_I10_OUT_GPIO_Port GPIOA
#define MOTOR_I11_OUT_Pin GPIO_PIN_1
#define MOTOR_I11_OUT_GPIO_Port GPIOA
#define MOTOR_PHASE1_OUT_Pin GPIO_PIN_2
#define MOTOR_PHASE1_OUT_GPIO_Port GPIOA
#define MOTOR_I20_OUT_Pin GPIO_PIN_3
#define MOTOR_I20_OUT_GPIO_Port GPIOA
#define MOTOR_I21_OUT_Pin GPIO_PIN_4
#define MOTOR_I21_OUT_GPIO_Port GPIOA
#define MOTOR_PHASE2_OUT_Pin GPIO_PIN_5
#define MOTOR_PHASE2_OUT_GPIO_Port GPIOA
#define MOTOR_INHIBIT_OUT_Pin GPIO_PIN_6
#define MOTOR_INHIBIT_OUT_GPIO_Port GPIOA
#define VoltageADC_Pin GPIO_PIN_7
#define VoltageADC_GPIO_Port GPIOA
#define ThermalADC_Pin GPIO_PIN_0
#define ThermalADC_GPIO_Port GPIOB
#define PhotoADC_Pin GPIO_PIN_1
#define PhotoADC_GPIO_Port GPIOB
#define VT2_OE_Pin GPIO_PIN_2
#define VT2_OE_GPIO_Port GPIOB
#define PH_DI_Pin GPIO_PIN_10
#define PH_DI_GPIO_Port GPIOB
#define PH_CLK_Pin GPIO_PIN_11
#define PH_CLK_GPIO_Port GPIOB
#define PH_LATCH_Pin GPIO_PIN_12
#define PH_LATCH_GPIO_Port GPIOB
#define PH_STB1_Pin GPIO_PIN_13
#define PH_STB1_GPIO_Port GPIOB
#define PH_STB2_Pin GPIO_PIN_14
#define PH_STB2_GPIO_Port GPIOB
#define PH_STB3_Pin GPIO_PIN_15
#define PH_STB3_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_8
#define LED_GPIO_Port GPIOA
#define VT1_OE_Pin GPIO_PIN_15
#define VT1_OE_GPIO_Port GPIOA
#define Feed_Pin GPIO_PIN_8
#define Feed_GPIO_Port GPIOB
#define PRINT_HEAD_GPIO_Port GPIOB
#define MOTOR_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

#define DEF_SET 1
#define DEF_CLR 0

#define  DEF_BIT_00                                     0x01u
#define  DEF_BIT_01                                     0x02u
#define  DEF_BIT_02                                     0x04u
#define  DEF_BIT_03                                     0x08u
#define  DEF_BIT_04                                     0x10u
#define  DEF_BIT_05                                     0x20u
#define  DEF_BIT_06                                     0x40u
#define  DEF_BIT_07                                     0x80u

#define  DEF_BIT_08                                   0x0100u
#define  DEF_BIT_09                                   0x0200u
#define  DEF_BIT_10                                   0x0400u
#define  DEF_BIT_11                                   0x0800u
#define  DEF_BIT_12                                   0x1000u
#define  DEF_BIT_13                                   0x2000u
#define  DEF_BIT_14                                   0x4000u
#define  DEF_BIT_15                                   0x8000u

/* USER CODE END Private defines */

/* User can use this section to tailor USARTx/UARTx instance used and associated 
   resources */
/* Definition for USARTx clock resources */
#define USARTx                           USART1
#define USARTx_CLK_ENABLE()              __HAL_RCC_USART1_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __HAL_RCC_USART1_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_AF                     GPIO_AF1_USART1
#define USARTx_RX_PIN                    GPIO_PIN_10
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_AF                     GPIO_AF1_USART1

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler

/* Size of Trasmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE
  
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */

extern uint8_t iDataSerial;
#define IWDG_KEY_REFRESH	0x0000AAAA

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
