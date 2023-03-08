/*
 * homing.c
 *
 *  Created on: Sep 10, 2020
 *  	  Author: Santiago Rivier
 *        Updated by: Elias Correa y Eliseo Elorga
 *
 */
#include "homing.h"


double rpm = 1.0;  //Valor experimental.
bool homAprox, homStart,homFin;


void homing(void) {

	homAprox = true;
	homStart = true;

    homFin = false;
    motor1.hom = false;
    motor2.hom = false;
    motor3.hom = false;

    //Establecemos la direccion en sentido horario (VISTA FRONTAL DEL MOTOR)
    positive_Dir_MOTOR_1;
	positive_Dir_MOTOR_2;
	positive_Dir_MOTOR_3;

	HAL_TIM_IC_Stop_DMA(&htim2, TIM_CHANNEL_1);
	HAL_TIM_IC_Stop_DMA(&htim2, TIM_CHANNEL_2);
	HAL_TIM_IC_Stop_DMA(&htim2, TIM_CHANNEL_3);

	HAL_NVIC_DisableIRQ(EXTI0_IRQn);	//Apago interrupcion EndStop 1 Superior
	HAL_NVIC_DisableIRQ(EXTI1_IRQn);	//Apago interrupcion EndStop 1 Inferior
	HAL_NVIC_DisableIRQ(EXTI2_IRQn);	//Apago interrupcion EndStop 2 Superior
	HAL_NVIC_DisableIRQ(EXTI3_IRQn);	//Apago interrupcion EndStop 2 Inferior
	HAL_NVIC_DisableIRQ(EXTI4_IRQn);	//Apago interrupcion EndStop 3 Superior
	HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);	//Apago interrupcion EndStop 3 Inferior

    //Me aseguro que los motores esten detenidos

	Stop_PWM_MOTOR_1;
	Stop_PWM_MOTOR_2;
	Stop_PWM_MOTOR_3;

	/*
	motor1.counterPeriod =  COUNTERPERIOD(rpm);
	motor2.counterPeriod =  COUNTERPERIOD(rpm);
	motor3.counterPeriod =  COUNTERPERIOD(rpm);
	*/


	__HAL_TIM_SET_AUTORELOAD(&htim12,COUNTERPERIOD(rpm)); //Escritura del registro ARR
	__HAL_TIM_SET_AUTORELOAD(&htim13,COUNTERPERIOD(rpm));
	__HAL_TIM_SET_AUTORELOAD(&htim14,COUNTERPERIOD(rpm));

	__HAL_TIM_SET_AUTORELOAD(&htim2,3000); //Escritura del registro ARR

	TIM12->CCR1 = (uint32_t)((double)(TIM12->ARR) / 2.0);
	TIM13->CCR1 = (uint32_t)((double)(TIM13->ARR) / 2.0);
	TIM14->CCR1 = (uint32_t)((double)(TIM14->ARR) / 2.0);


    while(homAprox){

        Start_PWM_MOTOR_1;
        Start_PWM_MOTOR_2;
        Start_PWM_MOTOR_3;

        if (ES1s_PRESSED || ES2s_PRESSED || ES3s_PRESSED){
            HAL_Delay(30);
            if (ES1s_PRESSED || ES2s_PRESSED || ES3s_PRESSED){

                Stop_PWM_MOTOR_1;
                Stop_PWM_MOTOR_2;
                Stop_PWM_MOTOR_3;

                homAprox = false;
            }
        }
    }

    //-----------------------------------------------------------------------------

    while (!homFin){

        if (ES1s_PRESSED && !motor1.hom) {

            Stop_PWM_MOTOR_2;
            Stop_PWM_MOTOR_3;

            HAL_Delay(30);//Delay necesario para consultar el estado del pin vinculado al ES1s

            while(ES1s_PRESSED){

                negative_Dir_MOTOR_1;
                HAL_Delay(0.5); 							//delay cambio de dir
                Start_PWM_MOTOR_1;
                HAL_Delay(500); //Lo dejamos que se mueva medio segundo en la direccion descreciente

            }

            Stop_PWM_MOTOR_1;
            motor1.theta = 0.0;
            motor1.hom=true;
            HAL_UART_Transmit(&huart3,(uint8_t *)"F1\n", 4, 100);
            HAL_Delay(30);

            if (ES2s_UNPRESSED && !motor2.hom)Start_PWM_MOTOR_2;
            if (ES3s_UNPRESSED && !motor3.hom)Start_PWM_MOTOR_3;


        } // ES1s_UNPRESSED : Se dejó de presionar el FC1 sup

        if (ES2s_PRESSED && !motor2.hom){

            Stop_PWM_MOTOR_1;
            Stop_PWM_MOTOR_3;

            HAL_Delay(30);//Delay necesario para consultar el estado del pin vinculado al ES2s

            while(ES2s_PRESSED){

                negative_Dir_MOTOR_2;
                HAL_Delay(0.5);
                Start_PWM_MOTOR_2;
                HAL_Delay(500);

            }

            Stop_PWM_MOTOR_2;
            motor2.theta = 0.0;
            motor2.hom=true;
            HAL_UART_Transmit(&huart3,(uint8_t *)"F2\n", 4, 100);
            HAL_Delay(30);

            if (ES1s_UNPRESSED && !motor1.hom)Start_PWM_MOTOR_1;
            if (ES3s_UNPRESSED && !motor3.hom)Start_PWM_MOTOR_3;

        }// ES2s_UNPRESSED : Se dejó de presionar el FC2 sup


        if (ES3s_PRESSED){

            Stop_PWM_MOTOR_1;
            Stop_PWM_MOTOR_2;

            HAL_Delay(30);//Delay necesario para consultar el estado del pin vinculado al ES3s

            while(ES3s_PRESSED){

                negative_Dir_MOTOR_3;
                HAL_Delay(0.5);
                Start_PWM_MOTOR_3;
                HAL_Delay(500);
            }

            Stop_PWM_MOTOR_3;
            motor3.theta = 0.0;
            motor3.hom = true;
            HAL_UART_Transmit(&huart3,(uint8_t *)"F3\n", 4, 100);
			HAL_Delay(30);
            if (ES1s_UNPRESSED && !motor1.hom)Start_PWM_MOTOR_1;
            if (ES2s_UNPRESSED && !motor2.hom)Start_PWM_MOTOR_2;
        }// ES3s_UNPRESSED : Se dejó de presionar el FC3 sup



        if (motor1.hom && motor2.hom && motor3.hom){
        	homFin=true;


        	//HAL_TIM_IC_Start_DMA(&htim2, TIM_CHANNEL_1, fallData, numval);
        	//HAL_TIM_IC_Start_DMA(&htim2, TIM_CHANNEL_2, fallData, numval);
        	//HAL_TIM_IC_Start_DMA(&htim2, TIM_CHANNEL_3, fallData, numval);

        	//HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_ALL);	//Enciendo interrupcion input capture motor 1
        	//HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);	//Enciendo interrupcion input capture motor 2
        	//HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);	//Enciendo interrupcion input capture motor 3


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





