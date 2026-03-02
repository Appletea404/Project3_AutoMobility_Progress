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
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */






#include "car.h"
#include "direction.h"
#include "speed.h"
#include "delay.h"
#include "stdio.h"
//#include "ledbar.h"
#include "ultrasonic.h"
#include "statemachine.h"






/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */




/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */



#ifdef __GNUC__
/* With GCC small printf (option LD Linker->Libraries->Small printf
 * set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int  __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int  fputc(int ch, FILE *f)
#endif /* __GNUC__*/

/** @brief Retargets the C library printf function to the USART.
 *  @param None
 *  @retval None
 */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART2 and Loop
     until the end of transmission */
  if(ch == '\n')
  {
	  HAL_UART_Transmit(&huart2, (uint8_t*) "\r", 1, 0xFFFF);
  }
   HAL_UART_Transmit(&huart2, (uint8_t*) &ch, 1, 0xFFFF);\
   return ch;
}



/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */



//
//volatile uint8_t rxData[1];
//volatile uint8_t rxFlag = 0;
//volatile uint8_t rxCmd = 0;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */


//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//{
//  Ultrasonic_IC_CaptureCallback(htim);
//}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//    if (huart->Instance == USART1)
//    {
//        rxCmd = rxData[0];
//        rxFlag = 1;
//
//        HAL_UART_Receive_IT(&huart1, (uint8_t *)rxData, 1);
//    }
//}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
//  if (htim->Instance == TIM3 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4)
//    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

  Ultrasonic_IC_CaptureCallback(htim);
}



/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM11_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  STMACHINE_Init();

//  HAL_UART_Receive_IT(&huart1, (uint8_t *)rxData, 1);

  HAL_TIM_Base_Start(&htim11);                  // for delay_us function
  Ultrasonic_Init();



  // 1) speed 모듈에 TIM2 채널 연결
  Speed_Init(&htim2, TIM_CHANNEL_1, &htim2, TIM_CHANNEL_2);

  // 2) PWM Start + 초기 STOP
  Car_Init();




  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */



  while (1)
  {


//	  DC_CONTROL_AUTO();

	  ST_MACHINE();
//      if (rxFlag)
//      {
//          rxFlag = 0;
//
//          HAL_UART_Transmit(&huart1, (uint8_t *)&rxCmd, 1, 10);
//          st_Flag(rxCmd);
//          st_machine(rxCmd);
//      }


//
//	  Ultrasonic_TriggerAll();
//	  HAL_Delay(60);
//
//
//
//	  printf("LEFT : %d cm\r\n CENTER : %d cm\r\n RIGHT : %d cm\r\n",
//	         Ultrasonic_GetDistanceCm(US_LEFT),
//	         Ultrasonic_GetDistanceCm(US_CENTER),
//	         Ultrasonic_GetDistanceCm(US_RIGHT));
//
//	  HAL_Delay(1000);



//	  // 1) 정지
//	  Car_Stop();
//	  HAL_Delay(700);
//
//	  // 2) 전진 (30/50/70/100 순서)
//	  Car_Move(CAR_FRONT, SPD_30);
//	  HAL_Delay(1000);
//	  Car_Move(CAR_FRONT, SPD_50);
//	  HAL_Delay(1000);
//	  Car_Move(CAR_FRONT, SPD_70);
//	  HAL_Delay(1000);
//	  Car_Move(CAR_FRONT, SPD_100);
//	  HAL_Delay(1200);
//
//	  Car_Stop();
//	  HAL_Delay(700);
//
//	  // 3) 후진
//	  Car_Move(CAR_BACK, SPD_50);
//	  HAL_Delay(1500);
//
//	  Car_Stop();
//	  HAL_Delay(700);
//
//	  // 4) 제자리(또는 단순) 좌/우 회전 (요구대로 50/0, 0/50)
//	  Car_Move(CAR_LEFT, SPD_70);   // speed 인자는 무시되지만 시그니처 맞춰 넣음
//	  HAL_Delay(900);
//	  Car_Stop();
//	  HAL_Delay(400);
//
//	  Car_Move(CAR_RIGHT, SPD_70);
//	  HAL_Delay(900);
//	  Car_Stop();
//	  HAL_Delay(700);
//
//	  // 5) 대각 전진
//	  Car_Move(CAR_LEFTFRONT, SPD_70);   // 좌 50 / 우 70
//	  HAL_Delay(1200);
//	  Car_Stop();
//	  HAL_Delay(400);
//
//	  Car_Move(CAR_RIGHTFRONT, SPD_70);  // 좌 70 / 우 50
//	  HAL_Delay(1200);
//	  Car_Stop();
//	  HAL_Delay(700);
//
//	  // 6) 대각 후진
//	  Car_Move(CAR_LEFTBACK, SPD_70);
//	  HAL_Delay(1200);
//	  Car_Stop();
//	  HAL_Delay(400);
//
//	  Car_Move(CAR_RIGHTBACK, SPD_70);
//	  HAL_Delay(1200);
//
//	  // 루프 끝 정지
//	  Car_Stop();
//	  HAL_Delay(1500);










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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
