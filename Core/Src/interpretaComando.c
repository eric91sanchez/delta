/*
 * interpretaComando.c
 *
 *  Created on: 12 sep. 2020
 *      Author: Santiago River
 *      Updated by: Elias Correa y Eliseo Elorga
 */

#include "interpretaComando.h"


volatile double aux;
uint8_t auxString[10];

void interpretaComando(void){

	switch(rx_buffer[0]){


	case 'H':
	case 'h':

		state = HOME;


		//Ponemos el enable en bajo para habilitar el driver

		HAL_GPIO_WritePin(S_Enable_1_GPIO_Port, S_Enable_1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(S_Enable_2_GPIO_Port, S_Enable_2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(S_Enable_3_GPIO_Port, S_Enable_3_Pin, GPIO_PIN_RESET);

		HAL_Delay(50); //50 ms es el tiempo que la señal ENABLE en cambiar de estado

		homing();



	break;
	/*
	 * Estos comandos nos permiten asignarles valores a las coordenadas articulares activas del robot,
	 * dicha asignacion generaria movimiento a lazo abierto. La dejaremos comentada ya que no representar ninguna
	 * funcionalidad importante.
	 *
	case 'M':						//:M(motor) + numero de motor (1. 2. 3) + Angulo de 0 a 90
	case 'm':
		switch(rx_buffer[1]){
		case '1':
			HAL_UART_Transmit(&huart3,(uint8_t *)"M1\n", 4, 100);
			aux =  strtod(&rx_buffer[2], NULL);
			if(aux>=0 && aux<=90){  // verificacion de limites articulares
				titha1 = aux;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)"ErrorPos\n\r", 10, 100);
			}
			break;
		case '2':
			HAL_UART_Transmit(&huart3,(uint8_t *)"M2\n", 4, 100);
			aux =  strtod(&rx_buffer[2], NULL);
			if(aux>=0 && aux<=90){  // verificacion de limites articulares
				titha2 = aux;
			}
			else{
				HAL_UART_Transmit(&huart3,(uint8_t *)"ErrorPos\n\r", 10, 100);
			}
			break;
		case '3':
			HAL_UART_Transmit(&huart3,(uint8_t *)"M3\n", 4, 100);
			aux =  strtod(&rx_buffer[2], NULL);
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
	 */


	case 'P':												//:Px0.1 y0.1 z-0.5 \0 (Eje, valor, espacio, Eje, valor, espacio, Eje, valor, espacio)
	case 'p':
		receptionFlag = true;
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
				HAL_UART_Transmit(&huart3, (uint8_t *)"Px_ok\n", 7, 100);
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
				HAL_UART_Transmit(&huart3,(uint8_t *)"Py_ok\n", 7, 100);
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
				HAL_UART_Transmit(&huart3,(uint8_t *)"Pz_ok\n", 7, 100);
			}
			i++;
		}


		inverseKinematic(Pfin);


		break;




	/*

	case 'v':
		HAL_UART_Transmit(&huart3, (uint8_t *)"params\n",8, 100);
		uint8_t i = 1;
		uint8_t j = 0;
		while(rx_buffer[i] != 0){
			if(rx_buffer[i] == 'm'){
				while(rx_buffer[i+1] != ' '){			//Almacenar dato en buffer hasta que se encuentre un espacio
					auxString[j] = rx_buffer[i+1];
					j++;
					i++;
				}
				vmax = strtod(&auxString[0], NULL);

				for (int k = 0; k <= 10; k++) {
					auxString[k] = 0;
				}

				j=0;
				HAL_UART_Transmit(&huart3, (uint8_t *)"vm\n",4, 100);
			}
			else if(rx_buffer[i] == 'v'){
				 if(rx_buffer[i+1] == 'f'){
					while(rx_buffer[i+2] != ' '){			//Almacenar dato en buffer hasta que se encuentre un espacio
						auxString[j] = rx_buffer[i+2];
						j++;
						i++;
					}
					vf = strtod(&auxString[0], NULL);

					for (int k = 0; k <= 10; k++) {
						auxString[k] = 0;
					}

					j=0;
					HAL_UART_Transmit(&huart3,(uint8_t *)"vf\n", 4, 100);
				}
			}

			else if(rx_buffer[i] == 'v'){
				 if(rx_buffer[i+1] == 'i'){
					while(rx_buffer[i+2] != ' '){			//Almacenar dato en buffer hasta que se encuentre un espacio
						auxString[j] = rx_buffer[i+2];
						j++;
						i++;
					}
					vi = strtod(&auxString[0], NULL);

					for (int k = 0; k <= 10; k++) {
						auxString[k] = 0;
					}

					j=0;
					HAL_UART_Transmit(&huart3,(uint8_t *)"vi\n", 4, 100);
				}
			}


			else if(rx_buffer[i] == 'a'){
				 if(rx_buffer[i+1] == 'm'){
					while(rx_buffer[i+2] != ' '){			//Almacenar dato en buffer hasta que se encuentre un espacio
						auxString[j] = rx_buffer[i+2];
						j++;
						i++;
					}
					amax = strtod(&auxString[0], NULL);

					for (int k = 0; k <= 10; k++) {
						auxString[k] = 0;
					}

					j=0;
					HAL_UART_Transmit(&huart3,(uint8_t *)"am\n", 4, 100);
				}
			}
			else if(rx_buffer[i] == 'j'){
				 if(rx_buffer[i+1] == 'm'){
					while(rx_buffer[i+2] != ' '){			//Almacenar dato en buffer hasta que se encuentre un espacio
						auxString[j] = rx_buffer[i+2];
						j++;
						i++;
					}
					jmax = strtod(&auxString[0], NULL);

					for (int k = 0; k <= 10; k++) {
						auxString[k] = 0;
					}
					j=0;
					HAL_UART_Transmit(&huart3,(uint8_t *)"jm\n", 4, 100);
				}
			}
			i++;
		}


		break;
	*/



	}

}
