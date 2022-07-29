/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define STP_ENABLE_Pin GPIO_PIN_0
#define STP_ENABLE_GPIO_Port GPIOA
#define STP_DIR_Pin GPIO_PIN_1
#define STP_DIR_GPIO_Port GPIOA
#define STP_STEP_Pin GPIO_PIN_2
#define STP_STEP_GPIO_Port GPIOA
#define DIAG_Pin GPIO_PIN_3
#define DIAG_GPIO_Port GPIOA
#define PHOE_Pin GPIO_PIN_4
#define PHOE_GPIO_Port GPIOA
#define PAPER_PRESENT_Pin GPIO_PIN_6
#define PAPER_PRESENT_GPIO_Port GPIOA
#define THERMISTOR_Pin GPIO_PIN_7
#define THERMISTOR_GPIO_Port GPIOA
#define PAPER_SENSOR_Pin GPIO_PIN_0
#define PAPER_SENSOR_GPIO_Port GPIOB
#define HEADER_SENSOR_Pin GPIO_PIN_1
#define HEADER_SENSOR_GPIO_Port GPIOB
#define PHSTB3_Pin GPIO_PIN_2
#define PHSTB3_GPIO_Port GPIOB
#define PHSTB2_Pin GPIO_PIN_10
#define PHSTB2_GPIO_Port GPIOB
#define PHSTB1_Pin GPIO_PIN_11
#define PHSTB1_GPIO_Port GPIOB
#define PHLATCH_Pin GPIO_PIN_12
#define PHLATCH_GPIO_Port GPIOB
#define PHCLK_Pin GPIO_PIN_13
#define PHCLK_GPIO_Port GPIOB
#define PHDI_Pin GPIO_PIN_14
#define PHDI_GPIO_Port GPIOB
#define MCD_MODE_Pin GPIO_PIN_3
#define MCD_MODE_GPIO_Port GPIOB
#define MCD_ENABLE_Pin GPIO_PIN_4
#define MCD_ENABLE_GPIO_Port GPIOB
#define MCD_PHASE_Pin GPIO_PIN_5
#define MCD_PHASE_GPIO_Port GPIOB
#define MCD_BRAKE_Pin GPIO_PIN_8
#define MCD_BRAKE_GPIO_Port GPIOB
#define MCD_OE_Pin GPIO_PIN_9
#define MCD_OE_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
