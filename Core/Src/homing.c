/*
 * homing.c
 *
 *  Created on: Sep 10, 2020
 *      Author: Santiago
 */
#include "homing.h"


uint8_t homeOk;			//Flag de aproximacion de homing
double rpm;


//#define ES1s_PRESSED !(HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port, E_EndStop1_Sup_Pin))
//#define ES2s_PRESSED !(HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port, E_EndStop3_Sup_Pin))
//#define ES3s_PRESSED !(HAL_GPIO_ReadPin(E_EndStop3_Sup_GPIO_Port, E_EndStop2_Sup_Pin))


void homingAprox(void) {
	homeOk = 0;
	HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);//Apago interrupcion input capture motor 1
	HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);//Apago interrupcion input capture motor 2
	HAL_TIM_IC_Stop_IT(&htim4, TIM_CHANNEL_1);//Apago interrupcion input capture motor 3
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);	//Apago interrupcion EndStop 1 Superior
	HAL_NVIC_DisableIRQ(EXTI1_IRQn);	//Apago interrupcion EndStop 1 Inferior
	HAL_NVIC_DisableIRQ(EXTI2_IRQn);	//Apago interrupcion EndStop 2 Superior
	HAL_NVIC_DisableIRQ(EXTI3_IRQn);	//Apago interrupcion EndStop 2 Inferior
	HAL_NVIC_DisableIRQ(EXTI4_IRQn);	//Apago interrupcion EndStop 3 Superior
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);	//Apago interrupcion EndStop 3 Inferior

	//Las tres piernas se mueven hacia arriba
	HAL_GPIO_WritePin(S_DirPaP1_GPIO_Port, S_DirPaP1_Pin, GPIO_PIN_RESET);//Set direccion con la misma convencion que motor.c
	HAL_GPIO_WritePin(S_DirPaP2_GPIO_Port, S_DirPaP2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(S_DirPaP3_GPIO_Port, S_DirPaP3_Pin, GPIO_PIN_RESET);
	HAL_Delay(20);
	rpm = 0.1;
	uint32_t hom = 0;


	while (homeOk == 0) {
		if(rpm<=4){
			rpm = rpm + 0.001;
		}



		//TIM12->CNT=0;

		periodoM[0]=(uint32_t)(((FCL * 60.0) / (rpm * ((double)(TIM12->PSC) + 1.0) * 9600.0)) - 1.0);
		periodoM[1]=(uint32_t)(((FCL * 60.0) / (rpm * ((double)(TIM13->PSC) + 1.0) * 9600.0)) - 1.0);
		periodoM[2]=(uint32_t)(((FCL * 60.0) / (rpm * ((double)(TIM14->PSC) + 1.0) * 9600.0)) - 1.0);

		//PREGUNTAR
		if (TIM12->CNT > periodoM[0]) {
					TIM12->CNT = periodoM[0] - 1;// Reinicio clock solo si hace falta y a un valor cercano a la interrupcion, para que no haga ese paso de nuevo
		}
		if (TIM13->CNT > periodoM[1]) {
					TIM13->CNT = periodoM[1] - 1;// Reinicio clock solo si hace falta y a un valor cercano a la interrupcion, para que no haga ese paso de nuevo
		}
		if (TIM14->CNT > periodoM[2]) {
					TIM14->CNT = periodoM[2] - 1;// Reinicio clock solo si hace falta y a un valor cercano a la interrupcion, para que no haga ese paso de nuevo
		}

		TIM12->ARR =periodoM[0];
		TIM12->CCR1 = (uint32_t)((double)(TIM12->ARR) / 2.0);
		TIM13->ARR =periodoM[1];
		TIM13->CCR1 = (uint32_t)((double)(TIM13->ARR) / 2.0);
		TIM14->ARR =periodoM[2];
		TIM14->CCR1 = (uint32_t)((double)(TIM14->ARR) / 2.0);


		if (!(HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port, E_EndStop1_Sup_Pin)) || !(HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port, E_EndStop2_Sup_Pin)) || !(HAL_GPIO_ReadPin(E_EndStop3_Sup_GPIO_Port, E_EndStop3_Sup_Pin))) {
			HAL_Delay(30);
			if (!(HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port, E_EndStop1_Sup_Pin)) || !(HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port, E_EndStop2_Sup_Pin)) || !(HAL_GPIO_ReadPin(E_EndStop3_Sup_GPIO_Port, E_EndStop3_Sup_Pin))) {
				HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
				HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);
				HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);
				//HAL_UART_Transmit(&huart3,homeOk, sizeof(homeOk), 100);
				homeOk = 1;
				//HAL_UART_Transmit(&huart3,homeOk, sizeof(homeOk), 100);
			}
		}
		if(hom==0){
			hom=1;
			//HAL_UART_Transmit(&huart3,(uint8_t *)"hola k ace\n\r", 12, 100);
			HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
			HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
		}
		HAL_Delay(1);
	}
	HAL_UART_Transmit(&huart3,(uint8_t *)"F\n", 3, 100);
}

void homingArm1(void) {

	double flagEndStop=0;

	HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);  //puede ser redundante

	rpm = 0.5;
	TIM12->ARR = (uint32_t)(((FCL * 60.0) / (rpm * ((double)(TIM12->PSC) + 1.0) * 9600.0)) - 1.0);
	TIM12->CCR1 = (TIM12->ARR) / 2;

	if (!(HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port, E_EndStop1_Sup_Pin))) {  //puede ser redundante
		while(!(HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port, E_EndStop1_Sup_Pin))){
			HAL_GPIO_WritePin(S_DirPaP1_GPIO_Port, S_DirPaP1_Pin, GPIO_PIN_SET);
			HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
			HAL_Delay(30);
		}
	}
	HAL_GPIO_WritePin(S_DirPaP1_GPIO_Port, S_DirPaP1_Pin, GPIO_PIN_RESET);
	HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
	homeOk = 0;

	while (homeOk == 0) {
		if (flagEndStop==0 && !(HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port, E_EndStop1_Sup_Pin))) {
			HAL_Delay(30);		//Para filtrar transitorio
			if (!(HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port,E_EndStop1_Sup_Pin))) {
				HAL_UART_Transmit(&huart3,(uint8_t *)"ES1_Pressed\n", 13, 100);
				HAL_GPIO_WritePin(S_DirPaP1_GPIO_Port, S_DirPaP1_Pin, GPIO_PIN_SET);
				flagEndStop = 1;
			}
		}
		if (flagEndStop==1 && HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port,E_EndStop1_Sup_Pin)) {
			HAL_Delay(30);		//Para filtrar transitorio
			if (HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port,E_EndStop1_Sup_Pin)) {
				HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
				HAL_UART_Transmit(&huart3,(uint8_t *)"endArm1\n", 9, 100);
				titha1 = 0;
				homeOk = 1;
			}
		}
	}
}

void homingArm2(void) {
	double flagEndStop = 0;
	HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);
	rpm = 0.5;
	TIM13->ARR = (uint32_t)(((FCL * 60.0) / (rpm * ((double)(TIM13->PSC) + 1.0) * 9600.0)) - 1.0);
	TIM13->CCR1 = (TIM13->ARR) / 2;

	if (!(HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port, E_EndStop2_Sup_Pin))) {
		while(!(HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port, E_EndStop2_Sup_Pin))){
			HAL_GPIO_WritePin(S_DirPaP2_GPIO_Port, S_DirPaP2_Pin, GPIO_PIN_SET);
			HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
			HAL_Delay(30);
		}

	}

	HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);
	rpm = 0.5;
	TIM13->ARR = (uint32_t)(((FCL * 60.0) / (rpm * ((double)(TIM13->PSC) + 1.0) * 9600.0)) - 1.0);
	TIM13->CCR1 = (TIM13->ARR) / 2;
	HAL_GPIO_WritePin(S_DirPaP2_GPIO_Port, S_DirPaP2_Pin, GPIO_PIN_RESET);
	HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
	homeOk = 0;
	while (homeOk == 0) {
		if (!(HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port, E_EndStop2_Sup_Pin))) {
			HAL_Delay(30);		//Para filtrar transitorio
			if (!(HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port,E_EndStop2_Sup_Pin))) {
				HAL_GPIO_WritePin(S_DirPaP2_GPIO_Port, S_DirPaP2_Pin, GPIO_PIN_SET);
				flagEndStop = 1;
			}
		}
		if (flagEndStop == 1 && HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port,E_EndStop2_Sup_Pin)) {
			HAL_Delay(30);		//Para filtrar transitorio
			if (HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port,E_EndStop2_Sup_Pin)) {
				HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);
				HAL_UART_Transmit(&huart3,(uint8_t *)"FinArm2\n\r", 9, 100);
				titha2 = 0;
				homeOk = 1;
			}
		}
	}
}

void homingArm3(void) {
	double flagEndStop = 0;
	HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);
	rpm = 0.5;
	TIM14->ARR = (uint32_t)(((FCL * 60.0) / (rpm * ((double)(TIM1->PSC) + 1.0) * 9600.0)) - 1.0);
	TIM14->CCR1 = (TIM14->ARR) / 2;
	if (!(HAL_GPIO_ReadPin(E_EndStop3_Sup_GPIO_Port, E_EndStop3_Sup_Pin))) {
		while(!(HAL_GPIO_ReadPin(E_EndStop3_Sup_GPIO_Port, E_EndStop3_Sup_Pin))){
			HAL_GPIO_WritePin(S_DirPaP3_GPIO_Port, S_DirPaP3_Pin, GPIO_PIN_SET);
			HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
			HAL_Delay(30);
		}
	}
	HAL_GPIO_WritePin(S_DirPaP3_GPIO_Port, S_DirPaP3_Pin, GPIO_PIN_RESET);
	HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
	homeOk = 0;
	while (homeOk == 0) {
		if (!(HAL_GPIO_ReadPin(E_EndStop3_Sup_GPIO_Port, E_EndStop3_Sup_Pin))) {
			HAL_Delay(30);		//Para filtrar transitorio
			if (!(HAL_GPIO_ReadPin(E_EndStop3_Sup_GPIO_Port, E_EndStop3_Sup_Pin))) {
				HAL_GPIO_WritePin(S_DirPaP3_GPIO_Port, S_DirPaP3_Pin, GPIO_PIN_SET);
				flagEndStop = 1;
			}
		}
		if (flagEndStop == 1 && HAL_GPIO_ReadPin(E_EndStop3_Sup_GPIO_Port, E_EndStop3_Sup_Pin)) {
			HAL_Delay(30);		//Para filtrar transitorio
			if (HAL_GPIO_ReadPin(E_EndStop3_Sup_GPIO_Port, E_EndStop3_Sup_Pin)) {
				HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);
				HAL_UART_Transmit(&huart3,(uint8_t *)"FinArm3\n\r", 9, 100);
				titha3 = 0;
				homeOk = 1;
			}
		}
	}


	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);	//Enciendo interrupcion input capture motor 1
	HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);	//Enciendo interrupcion input capture motor 2
	HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);	//Enciendo interrupcion input capture motor 3

	/*
	 *
	Enciendo las interrupciones de los FC

	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	 */
}

