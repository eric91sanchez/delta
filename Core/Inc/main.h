/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

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
#define E_EndStop2_Sup_Pin GPIO_PIN_2
#define E_EndStop2_Sup_GPIO_Port GPIOE
#define E_EndStop2_Sup_EXTI_IRQn EXTI2_IRQn
#define E_EndStop2_Inf_Pin GPIO_PIN_3
#define E_EndStop2_Inf_GPIO_Port GPIOE
#define E_EndStop2_Inf_EXTI_IRQn EXTI3_IRQn
#define E_EndStop3_Sup_Pin GPIO_PIN_4
#define E_EndStop3_Sup_GPIO_Port GPIOE
#define E_EndStop3_Sup_EXTI_IRQn EXTI4_IRQn
#define E_EndStop3_Inf_Pin GPIO_PIN_5
#define E_EndStop3_Inf_GPIO_Port GPIOE
#define E_EndStop3_Inf_EXTI_IRQn EXTI9_5_IRQn
#define BUTTON_Pin GPIO_PIN_13
#define BUTTON_GPIO_Port GPIOC
#define BUTTON_EXTI_IRQn EXTI15_10_IRQn
#define S_PulsoPaP2_Pin GPIO_PIN_8
#define S_PulsoPaP2_GPIO_Port GPIOF
#define S_PulsoPaP3_Pin GPIO_PIN_9
#define S_PulsoPaP3_GPIO_Port GPIOF
#define NucleoF4_RX_Pin GPIO_PIN_2
#define NucleoF4_RX_GPIO_Port GPIOA
#define NucleoF4_TX_Pin GPIO_PIN_3
#define NucleoF4_TX_GPIO_Port GPIOA
#define E_CountPap1_Pin GPIO_PIN_5
#define E_CountPap1_GPIO_Port GPIOA
#define E_CountPaP2_Pin GPIO_PIN_6
#define E_CountPaP2_GPIO_Port GPIOA
#define S_PulsoPaP1_Pin GPIO_PIN_14
#define S_PulsoPaP1_GPIO_Port GPIOB
#define Jetson_RX_Pin GPIO_PIN_15
#define Jetson_RX_GPIO_Port GPIOB
#define STLINK_TX_Pin GPIO_PIN_8
#define STLINK_TX_GPIO_Port GPIOD
#define STLINK_RX_Pin GPIO_PIN_9
#define STLINK_RX_GPIO_Port GPIOD
#define E_CountPaP3_Pin GPIO_PIN_12
#define E_CountPaP3_GPIO_Port GPIOD
#define S_DirPaP1_Pin GPIO_PIN_8
#define S_DirPaP1_GPIO_Port GPIOC
#define S_DirPaP2_Pin GPIO_PIN_9
#define S_DirPaP2_GPIO_Port GPIOC
#define Jetson_TX_Pin GPIO_PIN_9
#define Jetson_TX_GPIO_Port GPIOA
#define S_DirPaP3_Pin GPIO_PIN_10
#define S_DirPaP3_GPIO_Port GPIOC
#define S_Enable_1_Pin GPIO_PIN_4
#define S_Enable_1_GPIO_Port GPIOD
#define S_Enable_2_Pin GPIO_PIN_5
#define S_Enable_2_GPIO_Port GPIOD
#define S_Enable_3_Pin GPIO_PIN_6
#define S_Enable_3_GPIO_Port GPIOD
#define E_EndStop1_Sup_Pin GPIO_PIN_0
#define E_EndStop1_Sup_GPIO_Port GPIOE
#define E_EndStop1_Sup_EXTI_IRQn EXTI0_IRQn
#define E_EndStop1_Inf_Pin GPIO_PIN_1
#define E_EndStop1_Inf_GPIO_Port GPIOE
#define E_EndStop1_Inf_EXTI_IRQn EXTI1_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
