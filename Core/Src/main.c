/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
PetState now = {100, 0, 0, 0, 0};
PetState history[60]={0};

Time_Mode normal=NORMAL;
Time_Mode fast=FAST;

static uint8_t key_state=0;
static uint8_t time_state=0;
static volatile uint8_t timer=0;
volatile uint8_t timer_save=0;

static uint8_t rx_data[5]={0};
static uint8_t ble_data[2]={0};

volatile uint8_t ble_flag=0;
volatile uint8_t write_index=0;
volatile uint8_t full_flag=0;
volatile uint8_t tx_flag=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim==&htim2){
		timer_save++;

		history[write_index].happy=now.happy;
		history[write_index].hungry=now.hungry;
		history[write_index].tired=now.tired;
		history[write_index].humidity=now.humidity;
		history[write_index].temperature=now.temperature;

		write_index=(write_index+1)%60;


		if(time_state==0){
			timer++;
			if(timer==30){
			now.happy-=1;
			now.hungry+=1;
			now.tired+=1;
			timer=0;
			}
		}else{
			now.happy-=1;
			now.hungry+=1;
			now.tired+=1;
		}

		if(timer_save==60){
			full_flag=1;
			tx_flag=1;
		}

	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart==&huart3){
		ble_flag=ble_data[0];
		HAL_UART_Receive_IT(&huart3, ble_data, 1);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  Project_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  OLED_Mode();
	  AHT20_Read(&now.temperature,&now.humidity);
	  key_state=KEY_Scan();
	  switch(key_state){
	  	  case 1:
	  		  Mode(&time_state,normal);
	  		  break;
	  	  case 2:
	  		  Mode(&time_state,fast);
	  		  break;
	  	  case 3:
	  		  Data_curve();
	  		  break;
	  }






    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void Mode(uint8_t *state,Time_Mode mode){
	  __HAL_TIM_SET_AUTORELOAD(&htim3,3000);
	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,3000/5);
	  HAL_Delay(100);
	  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,0);
	  HAL_TIM_Base_Start_IT(&htim2);
	  *state=mode;
	  function();
}

void Project_Init()
{
	  HAL_Delay(20);
	  OLED_Init();
	  AHT20_Init();
	  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	  HAL_UART_Receive_IT(&huart3, ble_data, 1);
	  if(!IsFirstBoot()){
		  EEPROM_ReadBuffer(&hi2c1,ADDRESS_Write,ADDRESS_Start,rx_data,3);
		  now.happy=rx_data[0];
		  now.hungry=rx_data[1];
		  now.tired=rx_data[2];
	  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {

  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
