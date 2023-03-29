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

//Estructura para definir el vector posicion

typedef struct Vector3D{
	double x;
	double y;
	double z;
}Vec3D;

typedef enum { false, true } bool;
typedef enum {INIT,READY,WORKING,HOME,FAULT} statesMachine;

typedef struct Motor{

	uint32_t pMotor;
	uint32_t numStep;
	double theta;     		//Posicion angular[degrees]
	double omega;     		//Velocidad angular[rad/s]
	double rpm;				//Velocidad angular[rev/min]
	bool hom;
	bool stepReached;

	double currentAngle;
	double calcStep;
	double remanente;

}Motor;



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
#define relayDrivers_Pin GPIO_PIN_3
#define relayDrivers_GPIO_Port GPIOF
#define S_PulsoPaP2_Pin GPIO_PIN_8
#define S_PulsoPaP2_GPIO_Port GPIOF
#define S_PulsoPaP3_Pin GPIO_PIN_9
#define S_PulsoPaP3_GPIO_Port GPIOF
#define stepFeedback1_Pin GPIO_PIN_0
#define stepFeedback1_GPIO_Port GPIOA
#define NucleoF4_RX_Pin GPIO_PIN_2
#define NucleoF4_RX_GPIO_Port GPIOA
#define NucleoF4_TX_Pin GPIO_PIN_3
#define NucleoF4_TX_GPIO_Port GPIOA
#define stepFeedback2_Pin GPIO_PIN_6
#define stepFeedback2_GPIO_Port GPIOA
#define faultDriver1_Pin GPIO_PIN_7
#define faultDriver1_GPIO_Port GPIOE
#define faultDriver1_EXTI_IRQn EXTI9_5_IRQn
#define S_PulsoPaP1_Pin GPIO_PIN_14
#define S_PulsoPaP1_GPIO_Port GPIOB
#define Jetson_RX_Pin GPIO_PIN_15
#define Jetson_RX_GPIO_Port GPIOB
#define STLINK_TX_Pin GPIO_PIN_8
#define STLINK_TX_GPIO_Port GPIOD
#define STLINK_RX_Pin GPIO_PIN_9
#define STLINK_RX_GPIO_Port GPIOD
#define stepFeedback3_Pin GPIO_PIN_12
#define stepFeedback3_GPIO_Port GPIOD
#define faultDriver2_Pin GPIO_PIN_14
#define faultDriver2_GPIO_Port GPIOD
#define faultDriver2_EXTI_IRQn EXTI15_10_IRQn
#define faultDriver3_Pin GPIO_PIN_15
#define faultDriver3_GPIO_Port GPIOD
#define faultDriver3_EXTI_IRQn EXTI15_10_IRQn
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

//Macros de la lectura de los finales de carrera
#define ES1s_PRESSED !(HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port, E_EndStop1_Sup_Pin))
#define ES2s_PRESSED !(HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port, E_EndStop2_Sup_Pin))
#define ES3s_PRESSED !(HAL_GPIO_ReadPin(E_EndStop3_Sup_GPIO_Port, E_EndStop3_Sup_Pin))

#define ES1s_UNPRESSED (HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port, E_EndStop1_Sup_Pin))
#define ES2s_UNPRESSED (HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port, E_EndStop2_Sup_Pin))
#define ES3s_UNPRESSED (HAL_GPIO_ReadPin(E_EndStop3_Sup_GPIO_Port, E_EndStop3_Sup_Pin))


#define ES1i_PRESSED !(HAL_GPIO_ReadPin(E_EndStop1_Inf_GPIO_Port, E_EndStop1_Inf_Pin))
#define ES2i_PRESSED !(HAL_GPIO_ReadPin(E_EndStop2_Inf_GPIO_Port, E_EndStop2_Inf_Pin))
#define ES3i_PRESSED !(HAL_GPIO_ReadPin(E_EndStop3_Inf_GPIO_Port, E_EndStop3_Inf_Pin))

#define ES1i_UNPRESSED (HAL_GPIO_ReadPin(E_EndStop1_Inf_GPIO_Port, E_EndStop1_Inf_Pin))
#define ES2i_UNPRESSED (HAL_GPIO_ReadPin(E_EndStop2_Inf_GPIO_Port, E_EndStop2_Inf_Pin))
#define ES3i_UNPRESSED (HAL_GPIO_ReadPin(E_EndStop3_Inf_GPIO_Port, E_EndStop3_Inf_Pin))

//Macros para encender o apargar el pwm de los motores

#define Start_PWM_MOTOR_1 (HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1))
#define Start_PWM_MOTOR_2 (HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1))
#define Start_PWM_MOTOR_3 (HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1))

#define Stop_PWM_MOTOR_1 (HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1))
#define Stop_PWM_MOTOR_2 (HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1))
#define Stop_PWM_MOTOR_3 (HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1))

//Macros para definir el sentido de giro de los motores.  POSITIVO SENTIDO HORARIO (VISTA FRONTAL DEL MOTOR)
// 														  NEGATIVO SENTIDO ANTIHORARIO (VISTA FRONTAL DEL MOTOR)

#define positive_Dir_MOTOR_1 (HAL_GPIO_WritePin(S_DirPaP1_GPIO_Port, S_DirPaP1_Pin, GPIO_PIN_RESET))
#define positive_Dir_MOTOR_2 (HAL_GPIO_WritePin(S_DirPaP2_GPIO_Port, S_DirPaP2_Pin, GPIO_PIN_RESET))
#define positive_Dir_MOTOR_3 (HAL_GPIO_WritePin(S_DirPaP3_GPIO_Port, S_DirPaP3_Pin, GPIO_PIN_RESET))

#define negative_Dir_MOTOR_1 (HAL_GPIO_WritePin(S_DirPaP1_GPIO_Port, S_DirPaP1_Pin, GPIO_PIN_SET))
#define negative_Dir_MOTOR_2 (HAL_GPIO_WritePin(S_DirPaP2_GPIO_Port, S_DirPaP2_Pin, GPIO_PIN_SET))
#define negative_Dir_MOTOR_3 (HAL_GPIO_WritePin(S_DirPaP3_GPIO_Port, S_DirPaP3_Pin, GPIO_PIN_SET))

#define relayAbierto (HAL_GPIO_WritePin(relayDrivers_GPIO_Port, relayDrivers_Pin, GPIO_PIN_SET))
#define relayCerrado (HAL_GPIO_WritePin(relayDrivers_GPIO_Port, relayDrivers_Pin, GPIO_PIN_RESET))

#define FCL 64000000.0


#define MICROSTEPRESOLUTION 8000.0			   //Micropasos por revolucion. Corresponde a la configuracion de los Swithces del Driver
#define REDUCTOR 1.0				 		   //Relacion de reduccion (Se la esablece a 1 ya que hemos quitado los reductores del robot)
#define STEPREV MICROSTEPRESOLUTION*REDUCTOR   //Paso por revolucion considerando la reduccion.

#define COUNTERPERIOD(rpm) (uint32_t)((FCL/((double)(TIM12->PSC) + 1.0))*( 60.0 / ((rpm) * STEPREV)) - 1.0)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
