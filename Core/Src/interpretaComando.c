/*
 * interpretaComando.c
 *
 *  Created on: 12 sep. 2020
 *      Author: Santiago
 */

#include "interpretaComando.h"


volatile double aux;
uint8_t auxString[10];

void interpretaComando(void){

	switch(rx_buffer[0]){

	case 'v':                     //velocidad vi , vf , vmax
		switch(rx_buffer[1]){
		case 'i':
			//HAL_UART_Transmit(&huart3,(uint8_t *)"vi\n", 4, 100);
			aux =  strtod(&rx_buffer[2], NULL);
			vi=aux;
			break;
		case 'f':
			//HAL_UART_Transmit(&huart3,(uint8_t *)"vf\n", 4, 100);
			aux =  strtod(&rx_buffer[2], NULL);
			vf=aux;
			break;
		case 'm':
			//HAL_UART_Transmit(&huart3,(uint8_t *)"vm\n", 4, 100);
			aux =  strtod(&rx_buffer[2], NULL);
			vmax=aux;

			break;
		default:
			HAL_UART_Transmit(&huart3,(uint8_t *)"Error\n", 7, 100);
			break;
		}
		break;

	case 'a': 					//aceleracion amax
		switch(rx_buffer[1]){
		case 'm':
			//HAL_UART_Transmit(&huart3,(uint8_t *)"am\n", 4, 100);
			aux =  strtod(&rx_buffer[2], NULL);
			amax=aux;
			break;

		default:
			HAL_UART_Transmit(&huart3,(uint8_t *)"Error\n", 7, 100);
			break;
		}

		break;

	case 'j': 					//jerk jmax
		switch(rx_buffer[1]){
		case 'm':
			//HAL_UART_Transmit(&huart3,(uint8_t *)"jm\n", 4, 100);
			aux =  strtod(&rx_buffer[2], NULL);
			jmax=aux;
			break;

		default:
			HAL_UART_Transmit(&huart3,(uint8_t *)"Error\n", 7, 100);
			break;
		}
		break;




	case 'M':						//:M(motor) + numero de motor (1. 2. 3) + Angulo de 0 a 90
	case 'm':
		switch(rx_buffer[1]){
		case '1':
			HAL_UART_Transmit(&huart3,(uint8_t *)"Motor1\n\r", 8, 100);
			aux =  strtod(&rx_buffer[2], NULL);
			if(aux>=0 && aux<=90){  // verificacion de limites articulares
				titha1 = aux;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)"ErrorPos\n\r", 10, 100);
			}
			break;
		case '2':
			HAL_UART_Transmit(&huart3,(uint8_t *)"Motor2\n\r", 8, 100);
			if(aux>=0 && aux<=90){  // verificacion de limites articulares
				titha2 = aux;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)"ErrorPos\n\r", 10, 100);
			}
			break;
		case '3':
			HAL_UART_Transmit(&huart3,(uint8_t *)"Motor3\n\r", 8, 100);
			if(aux>=0 && aux<=90){   // verificacion de limites articulares
				titha3 = aux;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)"ErrorPos\n\r", 10, 100);
			}
			break;

		default:
			HAL_UART_Transmit(&huart3,(uint8_t *)"ErrorMotor\n\r", 12, 100);
			break;
		}
		break;



	case 'P':												//:Px0.1 y0.1 z-0.5 \0 (Eje, valor, espacio, Eje, valor, espacio, Eje, valor, espacio)
	case 'p':
		HAL_UART_Transmit(&huart3,(uint8_t *)"Punto\n\r", 7, 100);
		flagErrorEndStop = 0;
		uint8_t i = 1;
		uint8_t j = 0;
		while(rx_buffer[i] != 0){
			if(rx_buffer[i] == 'x'){
				while(rx_buffer[i+1] != ' '){			//Almacenar dato en buffer hasta que se encuentre un espacio
					auxString[j] = rx_buffer[i+1];
					j++;
					i++;
				}
				Pfin.x = strtod(&auxString[0], NULL);
				for (int k = 0; k <= 10; k++) {
					auxString[k] = 0;
				}
				j=0;
				HAL_UART_Transmit(&huart3, (uint8_t *)"PuntoX_ok\n\r", 11, 100);
			}
			else if(rx_buffer[i] == 'y'){
				while(rx_buffer[i+1] != ' '){			//Almacenar dato en buffer hasta que se encuentre un espacio
					auxString[j] = rx_buffer[i+1];
					j++;
					i++;
				}
				Pfin.y = strtod(&auxString[0], NULL);
				for (int k = 0; k <= 10; k++) {
					auxString[k] = 0;
				}
				j=0;
				HAL_UART_Transmit(&huart3,(uint8_t *)"PuntoY_ok\n\r", 11, 100);
			}
			else if(rx_buffer[i] == 'z'){
				while(rx_buffer[i+1] != ' '){			//Almacenar dato en buffer hasta que se encuentre un espacio
					auxString[j] = rx_buffer[i+1];
					j++;
					i++;
				}
				Pfin.z = strtod(&auxString[0], NULL);
				for (int k = 0; k <= 10; k++) {
					auxString[k] = 0;
				}
				j=0;
				HAL_UART_Transmit(&huart3,(uint8_t *)"PuntoZ_ok\n\r", 11, 100);
			}
			i++;
		}
		cinematicaInversa(Pfin);
		break;
	case 'H':
	case 'h':
		HAL_UART_Transmit(&huart3,(uint8_t *)"Homing\n\r", 8, 100);
		//PREGUNTA : se activan los enables en el homming para asegurarse de que el robot empiece a operar en una posicion segura  ?
		HAL_GPIO_WritePin(S_Enable_1_GPIO_Port, S_Enable_1_Pin, GPIO_PIN_RESET);  //activo Enable
		HAL_GPIO_WritePin(S_Enable_2_GPIO_Port, S_Enable_2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(S_Enable_3_GPIO_Port, S_Enable_3_Pin, GPIO_PIN_RESET);
		homingAprox();
		homingArm1();
		homingArm2();
		homingArm3();
		Pini.x=0;
		Pini.y=0;
		Pini.z = -0.334658034417224;
		HAL_UART_Transmit(&huart3,(uint8_t *)"Fin_Homing\n\r", 12, 100);

	break;

	}

}
