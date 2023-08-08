/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "l3gd20.h"
#include "stm32f3_discovery_gyroscope.h"

//#ifndef __MAIN_H
//#define LD_PORT GPIOE
//#define LD3 GPIO_PIN_9 //red1
//#define LD4 GPIO_PIN_8 //blue1
//#define LD5 GPIO_PIN_10 //orange1
//#define LD6 GPIO_PIN_15 //green1
//
//#define LD7 GPIO_PIN_11 //green2
//#define LD8 GPIO_PIN_14 //orange2
//#define LD9 GPIO_PIN_12 //blue2
//#define LD10 GPIO_PIN_13 //red2
//
//#define LD3_ON HAL_GPIO_WritePin(LD_PORT, LD3, GPIO_PIN_SET)
//#define LD4_ON HAL_GPIO_WritePin(LD_PORT, LD4, GPIO_PIN_SET)
//#define LD5_ON HAL_GPIO_WritePin(LD_PORT, LD5, GPIO_PIN_SET)
//#define LD6_ON HAL_GPIO_WritePin(LD_PORT, LD6, GPIO_PIN_SET)
//
//#define LD7_ON HAL_GPIO_WritePin(LD_PORT, LD3, GPIO_PIN_SET)
//#define LD8_ON HAL_GPIO_WritePin(LD_PORT, LD4, GPIO_PIN_SET)
//#define LD9_ON HAL_GPIO_WritePin(LD_PORT, LD5, GPIO_PIN_SET)
//#define LD10_ON HAL_GPIO_WritePin(LD_PORT, LD6, GPIO_PIN_SET)
//
//#define LD3_OFF HAL_GPIO_WritePin(LD_PORT, LD3, GPIO_PIN_RESET)
//#define LD4_OFF HAL_GPIO_WritePin(LD_PORT, LD4, GPIO_PIN_RESET)
//#define LD5_OFF HAL_GPIO_WritePin(LD_PORT, LD5, GPIO_PIN_RESET)
//#define LD6_OFF HAL_GPIO_WritePin(LD_PORT, LD6, GPIO_PIN_RESET)
//
//#define LD7_OFF HAL_GPIO_WritePin(LD_PORT, LD3, GPIO_PIN_RESET)
//#define LD8_OFF HAL_GPIO_WritePin(LD_PORT, LD4, GPIO_PIN_RESET)
//#define LD9_OFF HAL_GPIO_WritePin(LD_PORT, LD5, GPIO_PIN_RESET)
//#define LD10_OFF HAL_GPIO_WritePin(LD_PORT, LD6, GPIO_PIN_RESET)
//#endif


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
/* USER CODE BEGIN Private defines */
#define LD

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
