/*
 * statesMachine.c
 *
 *  Created on: Apr 3, 2023
 *      Author: Elias Correa y Eliseo Elorga
 */

#include "statesMachine.h"



statesMachine state = INIT;

bool homFin = false;
bool startMotors = false;
bool stopMotors = false;
bool endStopAlarmSup = false;
bool endStopAlarmInf = false;


bool continuar = false;
bool faultDrivers = false;

//--------------------------------------------
//Valores para crear el perfil de velocidad
//'q' es una variable fisica que puedo interpretarse como posicon cartesiana o articular. qd,qdd,qddd son sus respectivas derivadas

double q=0,qd=0,qdd=0,qddd=0;
double jmax = 1;        //Jerk maximo
double jmin;			//Jerk minimo

double vmax = 0.5;		//Velocidad maxima
double vmin;			//Velocidad minima

double vi = 0.3;		//Velocidad inicial
double vf = 0;			//Velocidad final

double amax = 2;		//Aceleracion maxima
double amin;			//Aceleracion minima
//--------------------------------------------

Vec3D Pini,Pfin;       //Punto inicial y final  (coordenadas cartesianas)

double arrayParams1[7];
double arrayParams2[7];
double arrayParams3[7];

double rpm_fault = 1;

bool timeFlag;
uint8_t rx_index = 0;
uint8_t rx_buffer[BUFFER_SIZE];
uint8_t rx_data;
uint8_t message[] = "Inicializacion en curso...\n";		//Mensaje enviado al iniciar el programa
uint8_t message1[] = "El robot ya se encuentra operacional.\n";

bool receptionFlag=false;
bool readFile=false;
bool startDemo = false;

//--------------------------------------------
//Lectura de archivo para demo

FILE *file;
char *filename = "archivo.txt";
char buffer[BUFFER_SIZE];
//--------------------------------------------

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

	motor1.stepReached = false;
	motor2.stepReached = false;
	motor3.stepReached = false;


}


void statesMachineLoop(void){



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

		homing();

        if(homFin){

        	homFin = false;
        	HAL_Delay(1);

        	HAL_NVIC_EnableIRQ(EXTI0_IRQn);		//Enciendo interrupcion EndStop 1 Superior
        	HAL_NVIC_EnableIRQ(EXTI1_IRQn);		//Enciendo interrupcion EndStop 1 Inferior
        	HAL_NVIC_EnableIRQ(EXTI2_IRQn);		//Enciendo interrupcion EndStop 2 Superior
        	HAL_NVIC_EnableIRQ(EXTI3_IRQn);		//Enciendo interrupcion EndStop 2 Inferior
        	HAL_NVIC_EnableIRQ(EXTI4_IRQn);		//Enciendo interrupcion EndStop 3 Superior
        	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);	//Enciendo interrupcion EndStop 3 Inferior
        	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); //Enciendo interrupcion faultDriver

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

			if (state==FAULT)break;

			if (motor1.stepReached) {
				Stop_PWM_MOTOR_1;
				HAL_TIM_IC_Stop(&htim2, TIM_CHANNEL_1);
			}else if (motor2.stepReached) {
				Stop_PWM_MOTOR_2;
				HAL_TIM_IC_Stop(&htim2, TIM_CHANNEL_2);
			}else if (motor3.stepReached){
				Stop_PWM_MOTOR_3;
				HAL_TIM_IC_Stop(&htim2, TIM_CHANNEL_3);
			}

			motor1.omega = get_Straj(time,motor1.currentAngle,motor1.theta,arrayParams1);
			motor2.omega = get_Straj(time,motor2.currentAngle,motor2.theta,arrayParams2);
			motor3.omega = get_Straj(time,motor3.currentAngle,motor3.theta,arrayParams3);

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

		HAL_TIM_Base_Stop_IT(&htim15);
		HAL_TIM_Base_Stop(&htim5);

		if (startDemo){state=DEMO;}
		else{state = READY;}


		break;

	case READY:

		if (receptionFlag){

			receptionFlag = false;

			startMotors = true;

			HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
			HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
			HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);

			inverseKinematic(Pfin);

			update_ScurveTraj(motor1.currentAngle, motor1.theta, vi, vf, vmax, amax, jmax, arrayParams1);
			update_ScurveTraj(motor2.currentAngle, motor2.theta, vi, vf, vmax, amax, jmax, arrayParams2);
			update_ScurveTraj(motor3.currentAngle, motor3.theta, vi, vf, vmax, amax, jmax, arrayParams3);

			configMotor(&motor1,1);
			configMotor(&motor2,2);
			configMotor(&motor3,3);

			timeFlag = false;

			motor1.pMotor = 0;
			motor2.pMotor = 0;
			motor3.pMotor = 0;

			motor1.stepReached = false;
			motor2.stepReached = false;
			motor3.stepReached = false;

			HAL_TIM_Base_Start(&htim5);
			HAL_TIM_Base_Start_IT(&htim15);

			state = WORKING;
		}
		break;

	case FAULT:

		__HAL_TIM_SET_AUTORELOAD(&htim12,COUNTERPERIOD(rpm_fault)); //Escritura del registro ARR
		__HAL_TIM_SET_AUTORELOAD(&htim13,COUNTERPERIOD(rpm_fault));
		__HAL_TIM_SET_AUTORELOAD(&htim14,COUNTERPERIOD(rpm_fault));

		TIM12->CCR1 = (uint32_t)((double)(TIM12->ARR) / 2.0);
		TIM13->CCR1 = (uint32_t)((double)(TIM13->ARR) / 2.0);
		TIM14->CCR1 = (uint32_t)((double)(TIM14->ARR) / 2.0);

		while((endStopAlarmSup || endStopAlarmInf) && continuar){

			 //HAL_UART_Transmit(&huart3,(uint8_t*)"EndStopAlarm\r\n", 16, 100);

			 if (ES1i_PRESSED){
				 HAL_Delay(10);
				 if (ES1i_PRESSED){
					 positive_Dir_MOTOR_1;
					 HAL_Delay(0.5); 							//delay cambio de dir
					 Start_PWM_MOTOR_1;
					 HAL_Delay(200);
					 Stop_PWM_MOTOR_1;
				 }
			 }
			 if (ES1s_PRESSED){
				 HAL_Delay(10);
				 if (ES1s_PRESSED){
					 negative_Dir_MOTOR_1;
					 HAL_Delay(0.5); 							//delay cambio de dir
					 Start_PWM_MOTOR_1;
					 HAL_Delay(200);
					 Stop_PWM_MOTOR_1;
				 }
			 }


			 if (ES2i_PRESSED){
				 HAL_Delay(30);
				 if (ES2i_PRESSED){
					 positive_Dir_MOTOR_2;
					 HAL_Delay(0.5); 							//delay cambio de dir
					 Start_PWM_MOTOR_2;
					 HAL_Delay(200);
					 Stop_PWM_MOTOR_2;
				 }
			 }
			 if (ES2s_PRESSED){
				 HAL_Delay(10);
				 if (ES2s_PRESSED){
					 negative_Dir_MOTOR_2;
					 HAL_Delay(0.5); 							//delay cambio de dir
					 Start_PWM_MOTOR_2;
					 HAL_Delay(200);
					 Stop_PWM_MOTOR_2;
				 }
			 }
			 if (ES3i_PRESSED){
				 HAL_Delay(10);
				 if (ES3i_PRESSED){
					 positive_Dir_MOTOR_3;
					 HAL_Delay(0.5); 							//delay cambio de dir
					 Start_PWM_MOTOR_3;
					 HAL_Delay(200);
					 Stop_PWM_MOTOR_3;
				 }
			 }
			 if (ES3s_PRESSED){
				 HAL_Delay(10);
				 if (ES3s_PRESSED){
					 negative_Dir_MOTOR_3;
					 HAL_Delay(0.5); 							//delay cambio de dir
					 Start_PWM_MOTOR_3;
					 HAL_Delay(200);
					 Stop_PWM_MOTOR_3;
				 }
			 }


			 if(ES1s_UNPRESSED && ES2s_UNPRESSED && ES3s_UNPRESSED && ES1i_UNPRESSED && ES2i_UNPRESSED && ES3i_UNPRESSED){
				 HAL_Delay(10);
				 if(ES1s_UNPRESSED && ES2s_UNPRESSED && ES3s_UNPRESSED && ES1i_UNPRESSED && ES2i_UNPRESSED && ES3i_UNPRESSED){

					 endStopAlarmSup = false;
					 endStopAlarmInf = false;
					 continuar = false;
					 HAL_UART_Transmit(&huart3,(uint8_t*)"Fin_FAULT\r\n", 13, 100);
					 state = READY;

				 }

			 }


		}//End while

		while(faultDrivers && continuar){


			//relayAbierto;
			//HAL_Delay(100);
			//relayCerrado;

			faultDrivers = false;
			continuar = false;

			HAL_UART_Transmit(&huart3,(uint8_t*)"Fin_FALL\r\n", 13, 100);
			state = READY;

		}//End while


		break;

	case DEMO:

	    // Abre el archivo para lectura (una sola vez siempre y cuando readFile sea verdadero )
		if (readFile){
			file = fopen(filename, "r");

			// Verifica si el archivo se ha abierto correctamente
			if (file == NULL) {
				HAL_UART_Transmit(&huart3,(uint8_t*)"No se pudo abrir el archivo.\n", 30, 100);
				break;
			}
			readFile = false;
			startDemo = true;
		}

	    // Lee cada línea del archivo y la guarda en el buffer
	    if (fgets(buffer, BUFFER_SIZE, file) != NULL) {

	        // Copia la línea al buffer de tipo uint8_t
	        //uint8_t rx_buffer[strlen(buffer)];
	        memcpy(rx_buffer, buffer, strlen(buffer));

	        interpretaComando();

	        state = READY;
	    }
	    else{
			// Cierra el archivo
			startDemo = false;
			fclose(file);
			HAL_UART_Transmit(&huart3,(uint8_t*)"Fin demo\n", 10, 100);
	    }

	default:break;
	}
}
