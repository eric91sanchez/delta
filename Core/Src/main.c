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
#include "trayectoria.h"
#include "cinematica.h"
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
								//Frecuencia clock
uint32_t pMotor1;		//Pasos de motor PaP1 actual
uint32_t pMotor2;		//Pasos de motor PaP2 actual
uint32_t pMotor3;		//Pasos de motor PaP3 actual
uint32_t numStep1;		//Numero de pasos PaP1 consigna
uint32_t numStep2;		//Numero de pasos PaP2 consigna
uint32_t numStep3;		//Numero de pasos PaP3 consigna
double titha1;
double titha2;
double titha3;


//Valores para crear el perfil de velocidad
double vmax;
double amax;
double jmax;
double vi;
double vf;


//double Fcl = 64000000;
/*
float_t Pxini;
float_t Pyini;
float_t Pzini;

float_t Pxfin;
float_t Pyfin;
float_t Pzfin;
*/

Vec3D Pini;
Vec3D Pfin;


float_t distancia;
float_t vDirector[3];
double Recta3D[3];
double dRecta3D[3];
double omega[3];
double TiempoTotal;
uint8_t rx_index = 0;
uint8_t rx_buffer[30];
uint8_t rx_data;
uint8_t data[] = "Start\n";		//Dato enviado al iniciar el programa
uint16_t valor = 0;
uint8_t cm0;				//Flag start transmit
uint8_t FlagTiempo;

double Tiempo;

double X = 0;
double DX = 0;
double DDX = 0;

double FlagTrayectoM1, FlagTrayectoM2, FlagTrayectoM3 = 1;
double omega1, omega2, omega3;
uint32_t periodoM1, periodoM2, periodoM3;
double periodoM[3];

int FlagButton = 0;

double flagErrorEndStop = 0;
double rpm1, rpm2, rpm3;
double ErrorPeriodo[3];
double ErrorAcumuladoPeriodo[3];
uint8_t Start=0;
double  mandalemecha = 0;
double dRecta3DZ=0; // para debugear


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
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
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART3_UART_Init();
  MX_TIM12_Init();
  MX_TIM13_Init();
  MX_TIM14_Init();
  MX_TIM5_Init();
  MX_TIM15_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Transmit(&huart3, data, sizeof(data), 100); //Enviamos el mensaje: Start
	HAL_UART_Receive_IT(&huart3, &rx_data, 1);


	HAL_TIM_Base_Start(&htim12);
	HAL_TIM_Base_Start(&htim13);
	HAL_TIM_Base_Start(&htim14);

	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);	//Enciendo interrupcion input capture motor 1
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);	//Enciendo interrupcion input capture motor 2
	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);	//Enciendo interrupcion input capture motor 3
	HAL_UART_Transmit(&huart3, (uint8_t *)"Ej:px0 y0 z-0.8 \n", 18, 100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if (FlagButton == 1) {
			FlagButton = 0;
			distancia = sqrt(pow(Pfin.x - Pini.x, 2) + pow(Pfin.y - Pini.y, 2) + pow(Pfin.z - Pini.z, 2));
			vDirector[0] = (Pfin.x - Pini.x) / distancia;	//Vector director en X
			vDirector[1] = (Pfin.y - Pini.y) / distancia;	//Vector director en Y
			vDirector[2] = (Pfin.z - Pini.z) / distancia;	//Vector director en Z
			configStepMotor1(titha1);
			configStepMotor2(titha2);
			configStepMotor3(titha3);
			inicializarTrayectoria(0, distancia, 0, 0, 0.4, 4, 1); //(Posinicio , Posfin , Vinicio , Vfin , Vmaxima, Amaxima, jerk)
			FlagTiempo = 0;
			FlagTrayectoM1 = 0;
			FlagTrayectoM2 = 0;
			FlagTrayectoM3 = 0;
			Start=1;
			rpm1 = 0;
			rpm2 = 0;
			rpm3 = 0;
			mandalemecha = 1;
			HAL_TIM_Base_Start(&htim5);
			HAL_TIM_Base_Start_IT(&htim15);
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
	switch (GPIO_Pin) {

	case BUTTON_Pin:
		FlagButton = 1;
		break;

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

	default:

		break;
	}

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
		if (pMotor1 == numStep1) {
			HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1); //Apago el PWM del motor 1
			FlagTrayectoM1 = 1;
		} else {
			pMotor1++;
		}

	} else if (htim->Instance == TIM3) {
		if (pMotor2 == numStep2) {
			HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);//Apago el PWM del motor 1
			FlagTrayectoM2 = 1;
		} else {
			pMotor2++;
		}

	} else if (htim->Instance == TIM4) {
		if (pMotor3 == numStep3) {
			HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);//Apago el PWM del motor 1
			FlagTrayectoM3 = 1;
		} else {
			pMotor3++;
		}

	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim == &htim15) {  //Timer que actualiza curva de velocidad
		if (FlagTrayectoM1 == 1 && FlagTrayectoM2 == 1 && FlagTrayectoM3 == 1) {
			HAL_TIM_Base_Stop_IT(&htim15);
			HAL_TIM_Base_Stop(&htim5);
			Pini.x = Pfin.x;
			Pini.y = Pfin.y;
			Pini.z = Pfin.z;
			mandalemecha = 0;
		} else {
			if (!FlagTiempo) {
				FlagTiempo = 1;
				TIM5->CNT = 0;	// comienzo a contabilizar el tiempo recien acá
			}
			Tiempo = (((double) (TIM5->CNT)) * ((double)(TIM5->PSC + 1) / 64000000.0));
			obtenerVelCurva(Tiempo);
			Recta3D[0] = Pini.x + X * vDirector[0];
			Recta3D[1] = Pini.y + X * vDirector[1];
			Recta3D[2] = Pini.z + X * vDirector[2];
			dRecta3D[0] = 0 + DX * vDirector[0];
			dRecta3D[1] = 0 + DX * vDirector[1];
			dRecta3D[2] = 0 + DX * vDirector[2];
			dRecta3DZ=dRecta3D[2]; // NO HACE NADA ?

			jacobianoInverso(dRecta3D[0], dRecta3D[1], dRecta3D[2], Recta3D[0], Recta3D[1], Recta3D[2]);
			SetPerfilTimers(omega[0], omega[1], omega[2]);
			if(Start==1){
				Start=0;
				HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);	// Activar generacion de pwm
				HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);	// Activar generacion de pwm
				HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);	// Activar generacion de pwm
			}
		}
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
