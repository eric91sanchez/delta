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
	break;

	case 'P':												//:Px0.1 y0.1 z-0.5 \0 (Eje, valor, espacio, Eje, valor, espacio, Eje, valor, espacio)
	case 'p':

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
		receptionFlag = true;


		break;


	case 'r':
	case 'R':
		HAL_UART_Transmit(&huart3, (uint8_t *)"Resetting...\n", 14, 100);
		NVIC_SystemReset();

		break;

	default:break;


	}

}
