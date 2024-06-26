/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "7segment.h"
#include "keyboard.h"
#include "calculator.h"
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
#define A_Pin GPIO_PIN_0
#define A_GPIO_Port GPIOA
#define B_Pin GPIO_PIN_1
#define B_GPIO_Port GPIOA
#define C_Pin GPIO_PIN_2
#define C_GPIO_Port GPIOA
#define D_Pin GPIO_PIN_3
#define D_GPIO_Port GPIOA
#define E_Pin GPIO_PIN_4
#define E_GPIO_Port GPIOA
#define F_Pin GPIO_PIN_5
#define F_GPIO_Port GPIOA
#define G_Pin GPIO_PIN_6
#define G_GPIO_Port GPIOA
#define DP_Pin GPIO_PIN_7
#define DP_GPIO_Port GPIOA
#define KEY_R4_Pin GPIO_PIN_0
#define KEY_R4_GPIO_Port GPIOB
#define KEY_R3_Pin GPIO_PIN_1
#define KEY_R3_GPIO_Port GPIOB
#define KEY_R2_Pin GPIO_PIN_10
#define KEY_R2_GPIO_Port GPIOB
#define KEY_R1_Pin GPIO_PIN_11
#define KEY_R1_GPIO_Port GPIOB
#define KEY_C1_Pin GPIO_PIN_12
#define KEY_C1_GPIO_Port GPIOB
#define KEY_C2_Pin GPIO_PIN_13
#define KEY_C2_GPIO_Port GPIOB
#define KEY_C3_Pin GPIO_PIN_14
#define KEY_C3_GPIO_Port GPIOB
#define KEY_C4_Pin GPIO_PIN_15
#define KEY_C4_GPIO_Port GPIOB
#define KEY_LED_R4_Pin GPIO_PIN_8
#define KEY_LED_R4_GPIO_Port GPIOA
#define KEY_LED_R3_Pin GPIO_PIN_9
#define KEY_LED_R3_GPIO_Port GPIOA
#define KEY_LED_R2_Pin GPIO_PIN_10
#define KEY_LED_R2_GPIO_Port GPIOA
#define KEY_LED_R1_Pin GPIO_PIN_11
#define KEY_LED_R1_GPIO_Port GPIOA
#define D1_Pin GPIO_PIN_12
#define D1_GPIO_Port GPIOA
#define D2_Pin GPIO_PIN_15
#define D2_GPIO_Port GPIOA
#define D3_Pin GPIO_PIN_3
#define D3_GPIO_Port GPIOB
#define D4_Pin GPIO_PIN_4
#define D4_GPIO_Port GPIOB
#define D5_Pin GPIO_PIN_5
#define D5_GPIO_Port GPIOB
#define D6_Pin GPIO_PIN_6
#define D6_GPIO_Port GPIOB
#define D7_Pin GPIO_PIN_7
#define D7_GPIO_Port GPIOB
#define D8_Pin GPIO_PIN_8
#define D8_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
