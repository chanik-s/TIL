/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

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
#define RED 0
#define GREEN 1
#define BLINK 2

uint8_t pre=1;
uint8_t flag=0;
uint8_t state_=GREEN;
uint8_t swConfig=1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

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
  /* USER CODE BEGIN 2 */

  //init
  //__HAL_RCC_GPIOC_CLK_ENABLE();
  //volatile unsigned int *reg=0x40021018;
  //*reg|=16;
  // __HAL_RCC_GPIOA_CLK_ENABLE();
  //*reg|=4;
 //*reg|=0x14;



  /*Configure GPIO pin Output Level */
  //GPIO_InitTypeDef GPIO_InitStruct = {0}; // 1<<13 , 1<<0 , 1<<1 , 0x3<<0
  //GPIO_InitTypeDef GPIO_InitStruct = {1<<13 , 1<<0 , 1<<1 , 0x3<<0};

  //HAL_GPIO_WritePin(GPIO_LED_GPIO_Port, GPIO_LED_Pin, GPIO_PIN_RESET);
  /*Configure GPIO pin : GPIO_LED_Pin */
  //GPIO_InitStruct.Pin = GPIO_LED_Pin; // ((uint16_t)0x2000)  /* Pin 13 selected   */
  //GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // 0x00000001u
  //GPIO_InitStruct.Pull = GPIO_PULLDOWN; //  0x00000002u
  //GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; //0x3UL <<0
  //HAL_GPIO_Init(GPIO_LED_GPIO_Port, &GPIO_InitStruct);
  //volatile unsigned int *reg2=0x40011004;
  //*reg2=(*reg2& ~(15UL<<20)) | (3UL<<20);

  /*Configure GPIO pin : GPIO_SW_Pin */
    //GPIO_InitStruct.Pin = 1;
    //GPIO_InitStruct.Mode = 0;
    //GPIO_InitStruct.Pull = 1;
    //HAL_GPIO_Init(GPIO_SW_GPIO_Port, &GPIO_InitStruct);
   // volatile unsigned int *regsw=&(GPIOA->CRL); //port A 0
    //*regsw=(*regsw&~(0xf)) | (0x8);
   // GPIOA->ODR=0x1; //pull up

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  volatile unsigned int *regFnd=0x40010810;
  //uint8_t arrFND[16]={ 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f,0x77,0x7c,0x58,0x5e,0x79,0x71};
  uint8_t arrFND[10]={ 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f};
  uint8_t size_FND=sizeof(arrFND)/sizeof(uint8_t);
  int i;
  //uint8_t state=0;
 // bool state=false;

  while (1)
  {

	  //state conversion
	  swConfig=HAL_GPIO_ReadPin(PB0_SW1_GPIO_Port, PB0_SW1_Pin);
	  //HAL_Delay(500);

	  if(pre==1&&swConfig==0){
		 // flag=~flag;

		  //state=!state;
		  switch(state_){
			  case RED:
				  state_=GREEN;
				  break;
			  case GREEN:
				  state_=BLINK;
				  break;
			  default:
				  state_=BLINK;
		  }
	  }
	  pre=swConfig;

	  //state action
	  switch(state_){
	  	 case RED:
	  		//GPIOB->ODR=1<<6;
	  		HAL_GPIO_WritePin(PB6_LED1_GPIO_Port, PB6_LED1_Pin, 0); //red on
	  		HAL_GPIO_WritePin(PB7_LED2_GPIO_Port, PB7_LED2_Pin, 1);//blue off
	  		 	 break;
	  	case GREEN:
	  		//GPIOB->ODR=1<<7;
	  		HAL_GPIO_WritePin(PB7_LED2_GPIO_Port, PB7_LED2_Pin, 0);//blue on
	  		HAL_GPIO_WritePin(PB6_LED1_GPIO_Port, PB6_LED1_Pin, 1); //red off
	  			break;
	  	case BLINK:
	  		state_=RED;
	  		for(i=9;i>=0;i--){

	  			GPIOA->ODR=arrFND[i];
	  			HAL_GPIO_WritePin(PB7_LED2_GPIO_Port, PB7_LED2_Pin, 1);//blue off
	  			HAL_Delay(500);
	  			HAL_GPIO_WritePin(PB7_LED2_GPIO_Port, PB7_LED2_Pin, 0);//blue on
	  			HAL_Delay(500);
	  		}
	  		HAL_GPIO_WritePin(PB7_LED2_GPIO_Port, PB7_LED2_Pin, 1);//blue off
	  		GPIOA->ODR=0;
	  			break;
	  	default:

	  }

	  /*
	  switch(state){

	  	  case false:
	  		HAL_GPIO_WritePin(PB6_LED1_GPIO_Port, PB6_LED1_Pin, 1); //red off
	  		HAL_GPIO_WritePin(PB7_LED2_GPIO_Port, PB7_LED2_Pin, 0); //blue on
	  		//flag=1;
	  		  break;

	  	  case true:
	  		  	  if(flag){


	  				  for(i=9;i>=0;i--){

	  					  GPIOA->ODR=arrFND[i];
	  					  HAL_GPIO_WritePin(PB7_LED2_GPIO_Port, PB7_LED2_Pin, 1);//blue off
	  				  	  HAL_Delay(500);
	  				  	  HAL_GPIO_WritePin(PB7_LED2_GPIO_Port, PB7_LED2_Pin, 0);//blue on
	  				  	  HAL_Delay(500);
	  				  	}
	  				  if(i<0){
	  					  HAL_GPIO_WritePin(PB7_LED2_GPIO_Port, PB7_LED2_Pin, 1);//blue off
	  					  HAL_GPIO_WritePin(PB6_LED1_GPIO_Port, PB6_LED1_Pin, 0); //red on
	  					  GPIOA->ODR=0;
	  					  flag=~flag;
	  				  }
	  			  }

	  			  else{
	  				  HAL_GPIO_WritePin(PB6_LED1_GPIO_Port, PB6_LED1_Pin, 0); //red on
	  				  HAL_GPIO_WritePin(PB7_LED2_GPIO_Port, PB7_LED2_Pin, 1); //blue off
	  			  }
	  		  break;

	  }
	   	   */



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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIO_LED_GPIO_Port, GPIO_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PA0_FNDA_Pin|PA1_FNDB_Pin|PA2_FNDC_Pin|PA3_FNDD_Pin
                          |PA4_FNDE_Pin|PA5_FNDF_Pin|PA6_FNDG_Pin|PA7_FNDP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, PB6_LED1_Pin|PB7_LED2_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : GPIO_LED_Pin */
  GPIO_InitStruct.Pin = GPIO_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIO_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0_FNDA_Pin PA1_FNDB_Pin PA2_FNDC_Pin PA3_FNDD_Pin
                           PA4_FNDE_Pin PA5_FNDF_Pin PA6_FNDG_Pin PA7_FNDP_Pin */
  GPIO_InitStruct.Pin = PA0_FNDA_Pin|PA1_FNDB_Pin|PA2_FNDC_Pin|PA3_FNDD_Pin
                          |PA4_FNDE_Pin|PA5_FNDF_Pin|PA6_FNDG_Pin|PA7_FNDP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0_SW1_Pin */
  GPIO_InitStruct.Pin = PB0_SW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PB0_SW1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PB6_LED1_Pin PB7_LED2_Pin */
  GPIO_InitStruct.Pin = PB6_LED1_Pin|PB7_LED2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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

#ifdef  USE_FULL_ASSERT
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
