/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "motor.h"
#include "homing.h"
#include "interpretaComando.h"
//#include "inverseJacobian.h"
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


Motor motor1, motor2,motor3;

double rpm1, rpm2, rpm3;
double ErrorPeriodo[3];
double ErrorAcumuladoPeriodo[3];
//double omega[3];
statesMachine state = INIT;

bool homFin = false;
bool startMotors = false;
bool stopMotors = false;

//--------------------------------------------
//Valores para crear el perfil de velocidad
double q=0,qd=0,qdd=0,qddd=0;
double jmax = 1;
double jmin;

double vmax = 0.5;
double vmin;

double vi = 0;
double vf = 0;

double amax = 2;
double amin;
//--------------------------------------------

Vec3D Pini;
Vec3D Pfin;
//double *arrayParams1,*arrayParams2,*arrayParams3;
double arrayParams1[7];
double arrayParams2[7];
double arrayParams3[7];


//float_t euclideanDistance;
//float_t vDirector[3];
//double Recta3D[3];
//double dRecta3D[3];
//double dRecta3DZ=0; // para debugear

double time;

uint8_t rx_index = 0;
uint8_t rx_buffer[30];
uint8_t rx_data;
uint8_t message[] = "Inicializacion en curso...\n";		//Mensaje enviado al iniciar el programa
uint8_t message1[] = "El robot ya se encuentra operacional.\n";

bool receptionFlag=false;

double periodoM[3];
uint32_t periodoM1, periodoM2, periodoM3;


double flagErrorEndStop = 0;
uint8_t cm0;				//Flag start transmit
bool timeFlag;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void robotInitialization(void){

	/* En esta rutina se procederá a inicializar perifericos vinculados al robot asi como la definicion
	* de un estado seguro y no referenciado del robot al momento de energizarlo, esto quiere decir que
	* habilatamos los drivers al momento de lanzar el programa para que los motores se bloqueen. Se procedera
	* a darles una consigna pequeña de posicion en direccion horario para que los eslabones no entren en la
	* singularidad de los 90º */


	HAL_TIM_Base_Start(&htim12);
	HAL_TIM_Base_Start(&htim13);
	HAL_TIM_Base_Start(&htim14);

	HAL_GPIO_WritePin(S_Enable_1_GPIO_Port, S_Enable_1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(S_Enable_2_GPIO_Port, S_Enable_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(S_Enable_3_GPIO_Port, S_Enable_3_Pin, GPIO_PIN_RESET);

	HAL_Delay(50); //50 ms es el tiempo que la señal ENABLE en cambiar de estado

	// Se estable la direccion horario por defecto
	positive_Dir_MOTOR_1;
	positive_Dir_MOTOR_2;
	positive_Dir_MOTOR_3;

	/*

	periodoM[0] = (uint32_t)(((FCL * 60.0) / (rpm * ((double)(TIM12->PSC) + 1.0) * STEPREV)) - 1.0);
	periodoM[1] = (uint32_t)(((FCL * 60.0) / (rpm * ((double)(TIM13->PSC) + 1.0) * STEPREV)) - 1.0);
	periodoM[2] = (uint32_t)(((FCL * 60.0) / (rpm * ((double)(TIM14->PSC) + 1.0) * STEPREV)) - 1.0);

	TIM12->ARR = periodoM[0];
	TIM12->CCR1 = (uint32_t)((double)(TIM12->ARR) / 2.0);  //pulse
	TIM13->ARR =periodoM[1];
	TIM13->CCR1 = (uint32_t)((double)(TIM13->ARR) / 2.0);
	TIM14->ARR =periodoM[2];
	TIM14->CCR1 = (uint32_t)((double)(TIM14->ARR) / 2.0);
	*/

	/*
	Start_PWM_MOTOR_1;
	Start_PWM_MOTOR_2;
	Start_PWM_MOTOR_3;

    HAL_Delay(1000); //offset experimental

    Stop_PWM_MOTOR_1;
    Stop_PWM_MOTOR_2;
    Stop_PWM_MOTOR_3;

    */
	motor1.stepReached = false;
	motor2.stepReached = false;
	motor3.stepReached = false;


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
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  MX_TIM12_Init();
  MX_TIM13_Init();
  MX_TIM14_Init();
  MX_TIM5_Init();
  MX_TIM15_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

		switch (state){

		case INIT:

			  HAL_UART_Transmit(&huart3, message, sizeof(message), 100); //Mensaje de inicializacion en curso.
			  HAL_UART_Receive_IT(&huart3, &rx_data, 1);
			  robotInitialization();
			  HAL_UART_Transmit(&huart3, message1, sizeof(message1), 100); //Mensaje inidicando que el Robot esta listo para su uso

			  state = READY;

			break;

		case HOME:

			receptionFlag = false; //Solo para asegurarse de no saltar al estado ready con esta bandera en true

			//Ponemos el enable en bajo para habilitar el driver

			HAL_GPIO_WritePin(S_Enable_1_GPIO_Port, S_Enable_1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(S_Enable_2_GPIO_Port, S_Enable_2_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(S_Enable_3_GPIO_Port, S_Enable_3_Pin, GPIO_PIN_RESET);

			HAL_Delay(50); //50 ms es el tiempo que la señal ENABLE en cambiar de estado

			homing();

	        if(homFin){

	        	homFin = false;

	        	HAL_NVIC_EnableIRQ(EXTI0_IRQn);		//Enciendo interrupcion EndStop 1 Superior
	        	HAL_NVIC_EnableIRQ(EXTI1_IRQn);		//Enciendo interrupcion EndStop 1 Inferior
	        	HAL_NVIC_EnableIRQ(EXTI2_IRQn);		//Enciendo interrupcion EndStop 2 Superior
	        	HAL_NVIC_EnableIRQ(EXTI3_IRQn);		//Enciendo interrupcion EndStop 2 Inferior
	        	HAL_NVIC_EnableIRQ(EXTI4_IRQn);		//Enciendo interrupcion EndStop 3 Superior
	        	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);	//Enciendo interrupcion EndStop 3 Inferior

				Pini.x=0;
				Pini.y=0;
				Pini.z =-0.5208; //antes era -0.33

				motor1.theta = 0.0;
				motor2.theta = 0.0;
				motor3.theta = 0.0;

				motor1.currentAngle = 0.0;
				motor2.currentAngle = 0.0;
				motor3.currentAngle = 0.0;

				state = READY;

	        }


			break;

		case WORKING:

			receptionFlag = false;

			while (!(motor1.stepReached && motor2.stepReached  && motor3.stepReached)){

				if (motor1.stepReached) {
					Stop_PWM_MOTOR_1;
					HAL_TIM_IC_Stop(&htim2, TIM_CHANNEL_1);
				}else if (motor2.stepReached) {
					Stop_PWM_MOTOR_2;
					HAL_TIM_IC_Stop(&htim3, TIM_CHANNEL_1);
				}else if (motor3.stepReached){
					Stop_PWM_MOTOR_3;
					HAL_TIM_IC_Stop(&htim4, TIM_CHANNEL_1);
				}

				motor1.omega = get_Straj(time,motor1.currentAngle,motor1.theta,arrayParams1);
//
				motor2.omega = get_Straj(time,motor2.currentAngle,motor2.theta,arrayParams2);
//
				motor3.omega = get_Straj(time,motor3.currentAngle,motor3.theta,arrayParams3);

				/*
				Recta3D[0] = Pini.x + q * vDirector[0];
				Recta3D[1] = Pini.y + q * vDirector[1];
				Recta3D[2] = Pini.z + q * vDirector[2];
				dRecta3D[0] = 0 + qd * vDirector[0];
				dRecta3D[1] = 0 + qd * vDirector[1];
				dRecta3D[2] = 0 + qd * vDirector[2];

				inverseJacobian(dRecta3D[0], dRecta3D[1], dRecta3D[2], Recta3D[0], Recta3D[1], Recta3D[2]);
				*/

				setProfilTimer();


				if(startMotors){
					startMotors = false;
					Start_PWM_MOTOR_1;	// Activar generacion de pwm
					Start_PWM_MOTOR_2;	// Activar generacion de pwm
					Start_PWM_MOTOR_3;	// Activar generacion de pwm
				}

				stopMotors = true;

			}// End while


			if (stopMotors){   //If steps goals for each motor were reached, we stop motors

				startMotors = false;

				HAL_TIM_IC_Stop(&htim2, TIM_CHANNEL_1);
				HAL_TIM_IC_Stop(&htim3, TIM_CHANNEL_1);
				HAL_TIM_IC_Stop(&htim4, TIM_CHANNEL_1);

				if (motor1.stepReached) Stop_PWM_MOTOR_1;
				if (motor2.stepReached) Stop_PWM_MOTOR_2;
				if (motor3.stepReached)	Stop_PWM_MOTOR_3;
			}

			//Update coordinantes
			Pini.x = Pfin.x;
			Pini.y = Pfin.y;
			Pini.z = Pfin.z;

			/*
			//TODO: Ver si esta bien liberar la memoria en este punto
			free(arrayParams1);
			free(arrayParams2);
			free(arrayParams3);
			*/

			HAL_TIM_Base_Stop_IT(&htim15);
			HAL_TIM_Base_Stop(&htim5);


			state = READY;

			break;

		case READY:

			if (receptionFlag){

				receptionFlag = false;

				startMotors = true;

				HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
				HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
				HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);

				/*
				euclideanDistance = sqrt(pow(Pfin.x - Pini.x, 2) + pow(Pfin.y - Pini.y, 2) + pow(Pfin.z - Pini.z, 2));
				vDirector[0] = (Pfin.x - Pini.x) / euclideanDistance;	//Vector director en X
				vDirector[1] = (Pfin.y - Pini.y) / euclideanDistance;	//Vector director en Y
				vDirector[2] = (Pfin.z - Pini.z) / euclideanDistance;	//Vector director en Z
				*/

				inverseKinematic(Pfin);

				configMotor(&motor1,1);
				configMotor(&motor2,2);
				configMotor(&motor3,3);

				motor1.currentAngle = 0;
				motor2.currentAngle = 0;
				motor3.currentAngle = 0;

				update_ScurveTraj(motor1.currentAngle, motor1.theta, vi, vf, vmax, amax, jmax, arrayParams1);
				update_ScurveTraj(motor2.currentAngle, motor2.theta, vi, vf, vmax, amax, jmax, arrayParams2);
				update_ScurveTraj(motor3.currentAngle, motor3.theta, vi, vf, vmax, amax, jmax, arrayParams3);

				timeFlag = false;

				motor1.pMotor = 0;
				motor2.pMotor = 0;
				motor3.pMotor = 0;

				motor1.stepReached = false;
				motor2.stepReached = false;
				motor3.stepReached = false;

				motor1.rpm = 0;
				motor2.rpm = 0;
				motor3.rpm = 0;

				HAL_TIM_Base_Start(&htim5);
				HAL_TIM_Base_Start_IT(&htim15);


				state = WORKING;
			}
			break;

		case FAULT:


			break;

		default:break;
		}



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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {


		//PREGUNTAR: COMO SERIA LA LOGICA DE INTERRUPCION CUANDO UNA PATA TOCA UN FINAL DE CARRERA

		/*case E_EndStop1_Inf_Pin:
		 HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);	//Apago el PWM del motor 1
		 HAL_UART_Transmit(&huart3, "EndStop1Inf\n\r", 13, 100);
		 break;
		 case E_EndStop1_Sup_Pin:
		 HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);	//Apago el PWM del motor 1
		 HAL_UART_Transmit(&huart3, "EndStop1Sup\n\r", 13, 100);
		 break;
		 case E_EndStop2_Inf_Pin:
		 HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);	//Apago el PWM del motor 2
		 HAL_UART_Transmit(&huart3, "EndStop2Inf\n\r", 13, 100);
		 break;
		 case E_EndStop2_Sup_Pin:
		 HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);	//Apago el PWM del motor 2
		 HAL_UART_Transmit(&huart3, "EndStop2Sup\n\r", 13, 100);
		 break;
		 case E_EndStop3_Inf_Pin:
		 HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);	//Apago el PWM del motor 3
		 HAL_UART_Transmit(&huart3, "EndStop3Inf\n\r", 13, 100);
		 break;
		 case E_EndStop3_Sup_Pin:
		 HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);	//Apago el PWM del motor 3
		 HAL_UART_Transmit(&huart3, "EndStop3Sup\n\r", 13, 100);
		 break;*/


}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	if (huart->Instance == USART3) {
		char dato;
		dato = rx_data;
		switch (dato) {
		case ':':
			rx_index = 0;
			cm0 = 1;
			break;
		case 8:
			if (rx_index > 0) {
				rx_index--;
			}
			break;
		case '\r':

			while (!(rx_index==30)){
				rx_buffer[rx_index]= (uint8_t) 0;
				rx_index++;
			}

			if (cm0 == 1) {
				rx_buffer[rx_index] = 0;
				interpretaComando();
				cm0 = 0;

			}
			break;
		default:
			if (rx_index < 30) {
				rx_buffer[rx_index++] = dato;
			}
			break;
		}
	}
	HAL_UART_Receive_IT(&huart3, &rx_data, 1); //Receive data (one character only)
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	__NOP();
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {

	if (htim->Instance == TIM2) {
		if (motor1.pMotor == motor1.numStep) {
			motor1.stepReached = true;
		} else {
			motor1.pMotor++;
		}
	}else if (htim->Instance == TIM3){
		if (motor2.pMotor == motor2.numStep) {
			motor2.stepReached = true;
		} else {
			motor2.pMotor++;
		}

	}else if (htim->Instance == TIM4){
		if (motor3.pMotor == motor3.numStep) {
			motor3.stepReached = true;
		} else {
			motor3.pMotor++;
		}

	}



	/* IMPLEMENTACION CON DMA Y UN SOLO TIMER
	if (htim->Instance == TIM2) {
		if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){

			if (motor1.pMotor == motor1.numStep) {
				motor1.stepReached = true;
			} else {
				motor1.pMotor++;
			}
		}

		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){
			if (motor2.pMotor == motor2.numStep) {
				motor2.stepReached = true;
			} else {
				motor2.pMotor++;
			}
		}
		else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
			if (motor3.pMotor == motor3.numStep) {
				motor3.stepReached = true;
			} else {
				motor3.pMotor++;
			}
		}
		else{}
	}
	*/
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim == &htim15) {  //Timer that update velocity'curve

		if (!timeFlag) {
			timeFlag = true;
			TIM5->CNT = 0;	//We start counting from here
		}

		time = (((double) (TIM5->CNT)) * ((double)(TIM5->PSC + 1) / FCL));
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
