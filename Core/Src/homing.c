/*
 * homing.c
 *
 *  Created on: Sep 10, 2020
 *  	  Author: Santiago Rivier
 *        Updated by: Elias Correa y Eliseo Elorga
 *
 */
#include "homing.h"


double rpm = 2.0;  //Valor experimental.
bool homAprox, homStart,homFin;
bool hom1,hom2,hom3;

#define ES1s_PRESSED !(HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port, E_EndStop1_Sup_Pin))
#define ES2s_PRESSED !(HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port, E_EndStop2_Sup_Pin))
#define ES3s_PRESSED !(HAL_GPIO_ReadPin(E_EndStop3_Sup_GPIO_Port, E_EndStop3_Sup_Pin))

#define ES1s_UNPRESSED (HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port, E_EndStop1_Sup_Pin))
#define ES2s_UNPRESSED (HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port, E_EndStop2_Sup_Pin))
#define ES3s_UNPRESSED (HAL_GPIO_ReadPin(E_EndStop3_Sup_GPIO_Port, E_EndStop3_Sup_Pin))


void homing(void) {

	homAprox = true;
	homStart = true;
    homFin = false;
    hom1=false;
    hom2=false;
    hom3=true;

    HAL_GPIO_WritePin(S_DirPaP1_GPIO_Port, S_DirPaP1_Pin, GPIO_PIN_RESET); // Se estable la direccion antihorario por defecto
    HAL_GPIO_WritePin(S_DirPaP2_GPIO_Port, S_DirPaP2_Pin, GPIO_PIN_RESET); // Se estable la direccion antihorario por defecto
    HAL_GPIO_WritePin(S_DirPaP3_GPIO_Port, S_DirPaP3_Pin, GPIO_PIN_RESET); // Se estable la direccion antihorario por defecto

	HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);//Apago interrupcion input capture motor 1
	HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);//Apago interrupcion input capture motor 2
	HAL_TIM_IC_Stop_IT(&htim4, TIM_CHANNEL_1);//Apago interrupcion input capture motor 3

	HAL_NVIC_DisableIRQ(EXTI0_IRQn);	//Apago interrupcion EndStop 1 Superior
	HAL_NVIC_DisableIRQ(EXTI1_IRQn);	//Apago interrupcion EndStop 1 Inferior
	HAL_NVIC_DisableIRQ(EXTI2_IRQn);	//Apago interrupcion EndStop 2 Superior
	HAL_NVIC_DisableIRQ(EXTI3_IRQn);	//Apago interrupcion EndStop 2 Inferior
	HAL_NVIC_DisableIRQ(EXTI4_IRQn);	//Apago interrupcion EndStop 3 Superior
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);	//Apago interrupcion EndStop 3 Inferior

    //Me aseguro que los motores esten detenidos

	HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);

	periodoM[0] = (uint32_t)(((FCL * 60.0) / (rpm * ((double)(TIM12->PSC) + 1.0) * STEPREV)) - 1.0);
	periodoM[1] = (uint32_t)(((FCL * 60.0) / (rpm * ((double)(TIM13->PSC) + 1.0) * STEPREV)) - 1.0);
	periodoM[2] = (uint32_t)(((FCL * 60.0) / (rpm * ((double)(TIM14->PSC) + 1.0) * STEPREV)) - 1.0);

	TIM12->ARR = periodoM[0];
	TIM12->CCR1 = (uint32_t)((double)(TIM12->ARR) / 2.0);
	TIM13->ARR =periodoM[1];
	TIM13->CCR1 = (uint32_t)((double)(TIM13->ARR) / 2.0);
	TIM14->ARR =periodoM[2];
	TIM14->CCR1 = (uint32_t)((double)(TIM14->ARR) / 2.0);


    while(homAprox){

        HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);

        if (ES1s_PRESSED || ES2s_PRESSED || ES3s_PRESSED){
            HAL_Delay(30);
            if (ES1s_PRESSED || ES2s_PRESSED || ES3s_PRESSED){

                HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
                HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);
                HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);

                homAprox = false;
            }
        }
    }

    //-----------------------------------------------------------------------------

    while (!homFin){

        if (ES1s_PRESSED && !hom1) {

            HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);
            HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);

            HAL_Delay(30);//Delay necesario para consultar el estado del pin vinculado al ES1s

            while(ES1s_PRESSED){

                HAL_GPIO_WritePin(S_DirPaP1_GPIO_Port, S_DirPaP1_Pin, GPIO_PIN_SET);
                HAL_Delay(0.5); 							//delay cambio de dir
                HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
                HAL_Delay(500); //Lo dejamos que se mueva medio segundo en la direccion descreciente

            }

            HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
            titha1 = 0;
            hom1=true;
            HAL_UART_Transmit(&huart3,(uint8_t *)"F1\n", 4, 100);
            HAL_Delay(30);

            if (ES2s_UNPRESSED && !hom2)HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
            //if (ES3s_UNPRESSED && !hom3)HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);


        } // ES1s_UNPRESSED : Se dejó de presionar el FC1 sup

        if (ES2s_PRESSED && !hom2){

            HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
            HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);

            HAL_Delay(30);//Delay necesario para consultar el estado del pin vinculado al ES2s

            while(ES2s_PRESSED){

                HAL_GPIO_WritePin(S_DirPaP2_GPIO_Port, S_DirPaP2_Pin, GPIO_PIN_SET);
                HAL_Delay(0.5);
                HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
                HAL_Delay(500);

            }

            HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);
            titha2 = 0;
            hom2=true;
            HAL_UART_Transmit(&huart3,(uint8_t *)"F2\n", 4, 100);
            HAL_Delay(30);

            if (ES1s_UNPRESSED && !hom1)HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
            //if (ES3s_UNPRESSED && !hom3)HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);

        }// ES2s_UNPRESSED : Se dejó de presionar el FC2 sup

        /*
        if (ES3s_PRESSED){

            HAL_TIM_PWM_Stop(&htim12, TIM_CHANNEL_1);
            HAL_TIM_PWM_Stop(&htim13, TIM_CHANNEL_1);

            HAL_Delay(30);//Delay necesario para consultar el estado del pin vinculado al ES3s

            while(ES3s_PRESSED){

                HAL_GPIO_WritePin(S_DirPaP3_GPIO_Port, S_DirPaP3_Pin, GPIO_PIN_SET);
                HAL_Delay(0.5);
                HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
                HAL_Delay(500);
            }

            HAL_TIM_PWM_Stop(&htim14, TIM_CHANNEL_1);
            titha3 = 0;
            hom3=true;
            HAL_UART_Transmit(&huart3,(uint8_t *)"F3\n", 4, 100);
			HAL_Delay(30);
            if (ES1s_UNPRESSED && !hom1)HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
            if (ES2s_UNPRESSED && !hom2)HAL_TIM_PWM_Start(&htim13, TIM_CHANNEL_1);
        }// ES3s_UNPRESSED : Se dejó de presionar el FC3 sup

        */

        if (hom1 &&hom2 && hom3){
        	homFin=true;

        	//HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);//Apago interrupcion input capture motor 1
        	//HAL_TIM_IC_Stop_IT(&htim3, TIM_CHANNEL_1);//Apago interrupcion input capture motor 2
        	//HAL_TIM_IC_Stop_IT(&htim4, TIM_CHANNEL_1);//Apago interrupcion input capture motor 3

        	HAL_NVIC_EnableIRQ(EXTI0_IRQn);		//Enciendo interrupcion EndStop 1 Superior
        	HAL_NVIC_EnableIRQ(EXTI1_IRQn);		//Enciendo interrupcion EndStop 1 Inferior
        	HAL_NVIC_EnableIRQ(EXTI2_IRQn);		//Enciendo interrupcion EndStop 2 Superior
        	HAL_NVIC_EnableIRQ(EXTI3_IRQn);		//Enciendo interrupcion EndStop 2 Inferior
        	HAL_NVIC_EnableIRQ(EXTI4_IRQn);		//Enciendo interrupcion EndStop 3 Superior
        	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);	//Enciendo interrupcion EndStop 3 Inferior


        	HAL_UART_Transmit(&huart3,(uint8_t *)"FH\n", 4, 100);
        }
       }


    }





