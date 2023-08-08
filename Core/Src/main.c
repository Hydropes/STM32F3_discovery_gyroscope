/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define BUFSIZ
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define ITM_Port32(n) (*((volatile unsigned long *)(0xE0000000+4*n)))
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

float x_val=0;
float y_val=0;
float z_val=0;
float buffer[3]={0, 0, 0};
float prev_buffer[3]={0, 0, 0};
float x_result_angle = 0.;
float y_result_angle = 0.;
float z_result_angle = 0.;
uint16_t delta =500;
uint8_t print_time = 0;
float XYZ_results[3];

float sens;//coef
uint16_t time_delay=100; //ms
float time_query =0.;//ms

float x = 0;
float abs_x = 0;
float y = 0;
float abs_y = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void GYRO_READ_ANGLE_III(float* buffer, float sens_coef, float time_delay);
void LED_ON_III(uint8_t led);
void LEDS_OFF_III(void);
float ABS(float num);
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int _write(int file, char *ptr, int len){
	int i = 0;
	for(i=0;i<len;i++){
		ITM_SendChar((*ptr++));
	}
	return len;
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
  // ITM_Port32(31)=1;

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

//  L3GD20_Init(0);
//  uint8_t gyro_id = L3GD20_ReadID();
//  uint8_t gyro_status = L3GD20_GetDataStatus();

  //HAL_TIM_Base_Start(&htim3);
  //HAL_TIM_Base_Start_IT(&htim3);

  //Через вложенности:
  BSP_GYRO_Init();
  BSP_GYRO_ReadID();
  printf("System Init Done \r\n");
  //HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, GPIO_PIN_SET);

  	uint8_t tmpreg = 0;
	GYRO_IO_Read(&tmpreg,L3GD20_CTRL_REG4_ADDR,1);
	printf("This is tmpreg %i\r\n", tmpreg);
	tmpreg =0b00010000;
	GYRO_IO_Write(&tmpreg, L3GD20_CTRL_REG4_ADDR, 1);
	printf("This is tmpreg %i\r\n", tmpreg);

	if (tmpreg==0){
		//200dps
		time_query=1000/200;
		sens =0.00875;
	} else if(tmpreg==32){
		//2000dps
		time_query=1000/2000;
		sens = 0.07;
	} else{
		//500dps
		time_query=1000/500;
		sens = 0.0175;
	}



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  GYRO_READ_ANGLE_III(XYZ_results, sens, ((float)time_delay+time_query)/1000);
	  x_result_angle = XYZ_results[0];
	  y_result_angle = XYZ_results[1];
	  z_result_angle = XYZ_results[2];

	  if(print_time%(10)==0){
		  print_time=0;
		 // HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_14);
		  printf("Angle(x) = %f\r\n", XYZ_results[0]);
		  printf("Angle(y) = %f\r\n", XYZ_results[1]);
		  printf("Angle(z) = %f\r\n", XYZ_results[2]);
		  printf("=============================\r\n");

	  }

	  x = x_result_angle;
	  abs_x = ABS(x_result_angle);
	  y = y_result_angle;
	  abs_y = ABS(y_result_angle);
//	  printf("x = %f\r\n", x);
//	  printf("abs_x = %f\r\n", abs_x);
//	  printf("y = %f\r\n", y);
//	  printf("abs_y = %f\r\n", abs_y);

	  if ((abs_x>10.)||(abs_y>10.)){
		  //diag leds
		  if((abs_x >0.5*abs_y)&&(abs_x <1.5*abs_y)){
			  if(x>0. && y >0.){
				  LED_ON_III(12);
			  }else if (x<0. && y >0.){
				  LED_ON_III(10);
			  }else if (x>0. && y <0.){
				  LED_ON_III(14);
			  }else if(x<0. && y <0.){
				  LED_ON_III(8);
			  }
		  } else{
  			  if(abs_x>abs_y){
  				  if(x>0){
  					  LED_ON_III(13);
  				  }else{
  					  LED_ON_III(9);
  				  }
  			  }else{
  				  if(y>0){
  					  LED_ON_III(11);
  				  }else{
  					  LED_ON_III(15);
  				  }
  			  }
			  //LEDS_OFF_III();
		  }
	  } else{
		  LEDS_OFF_III();
	  }

	  print_time+=1;
	  HAL_Delay(time_delay);


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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE3 PE0 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PE8 PE9 PE10 PE11 
                           PE12 PE13 PE14 PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11 
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){
	if(hspi->Instance ==SPI1){
		printf("Data translate by SPI was done \r\n");
	}

//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
//	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
//	if(htim->Instance==TIM3){
//		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_13);
//	}

//}
}

void GYRO_READ_ANGLE_III(float * buffer, float sens_coef, float time_delay){
	//printf("from func\r\n");
	uint8_t x_val_low, x_val_high;
	uint8_t y_val_low, y_val_high;
	uint8_t z_val_low, z_val_high;
	uint16_t results[3];
	uint8_t signs[3];
	float deriv=0.;

	GYRO_IO_Read(&x_val_low,L3GD20_OUT_X_L_ADDR,1);
	GYRO_IO_Read(&x_val_high,L3GD20_OUT_X_H_ADDR,1);

	GYRO_IO_Read(&y_val_low,L3GD20_OUT_Y_L_ADDR,1);
	GYRO_IO_Read(&y_val_high,L3GD20_OUT_Y_H_ADDR,1);

	GYRO_IO_Read(&z_val_low,L3GD20_OUT_Z_L_ADDR,1);
	GYRO_IO_Read(&z_val_high,L3GD20_OUT_Z_H_ADDR,1);

	results[0] =  x_val_low|(x_val_high<<8);
	results[1] =  y_val_low|(y_val_high<<8);
	results[2] =  z_val_low|(z_val_high<<8);

	for (uint8_t i =0; i<3;i++){
		//if ((results[i]>0b0000000000001111)&&(results[i]<0b1111000000000000)){
		//if (1){
			//printf("result(%i) = %i\r\n", i, results[i]);
			if((results[i]&0b1000000000000000)==0){
				signs[i] = 0;
			}else{
				signs[i]=1;
				results[i]&=0b0111111111111111;
				results[i]=0b1000000000000000-results[i];
			}
			deriv = sens_coef*results[i]*time_delay;
			if (abs(deriv)>0.02){
				if (signs[i]) {buffer[i]-=deriv;}
				else {buffer[i]+=deriv;}
			}


//		}else{
//			results[i]=0;
//		}

	}

}

void LED_ON_III(uint8_t led){
	uint16_t gpios[8]={GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10,
			GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13,
			GPIO_PIN_14,GPIO_PIN_15};
	for (uint8_t k=0; k<8; k++){
		HAL_GPIO_WritePin(GPIOE, gpios[k], GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(GPIOE, gpios[led-8], GPIO_PIN_SET);
}

void LEDS_OFF_III(void){
	uint16_t gpios[8]={GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10,
				GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13,
				GPIO_PIN_14,GPIO_PIN_15};
		for (uint8_t k=0; k<8; k++){
			HAL_GPIO_WritePin(GPIOE, gpios[k], GPIO_PIN_RESET);
		}
}
float ABS(float num){
	return num>0?num:-num;
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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
