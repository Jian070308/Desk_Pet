/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "oled.h"
#include "KEY.h"
#include "function.h"
#include "EEPROM.h"
#include "AHT20.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct{
	uint8_t happy;
	uint8_t hungry;
	uint8_t tired;
	float temperature;
	float humidity;
}PetState;

typedef enum{
	NORMAL,
	FAST
}Time_Mode;

extern PetState now;
extern PetState history[60];

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern volatile uint8_t write_index;
extern volatile uint8_t full_flag;
extern volatile uint8_t tx_flag;
extern volatile uint8_t ble_flag;
extern volatile uint8_t timer_save;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void Project_Init();
void Mode(uint8_t *state,Time_Mode mode);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY1_Pin GPIO_PIN_10
#define KEY1_GPIO_Port GPIOA
#define KEY2_Pin GPIO_PIN_11
#define KEY2_GPIO_Port GPIOA
#define KEY3_Pin GPIO_PIN_12
#define KEY3_GPIO_Port GPIOA
#define KEY4_Pin GPIO_PIN_15
#define KEY4_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
