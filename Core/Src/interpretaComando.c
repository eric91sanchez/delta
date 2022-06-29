/*
 * interpretaComando.c
 *
 *  Created on: 12 sep. 2020
 *      Author: Santiago
 */

#include "interpretaComando.h"


float auxM;
uint8_t auxString[10];

void interpretaComando(void){

	switch(rx_buffer[0]){
	case 'M':						//:M(motor) + numero de motor (1. 2. 3) + Angulo de 0 a 90
	case 'm':
		switch(rx_buffer[1]){
		case '1':
			HAL_UART_Transmit(&huart3, "Motor1\n\r", 8, 100);
			auxM =  strtod(&rx_buffer[2], NULL);
			if(auxM>=0 && auxM<=90){  // verificacion de limites articulares
				titha1 = auxM;
			}
			else{
				HAL_UART_Transmit(&huart3, "ErrorPos\n\r", 10, 100);
			}
			break;
		case '2':
			HAL_UART_Transmit(&huart3, "Motor2\n\r", 8, 100);
			if(auxM>=0 && auxM<=90){  // verificacion de limites articulares
				titha2 = auxM;
			}
			else{
				HAL_UART_Transmit(&huart3, "ErrorPos\n\r", 10, 100);
			}
			break;
		case '3':
			HAL_UART_Transmit(&huart3, "Motor3\n\r", 8, 100);
			if(auxM>=0 && auxM<=90){   // verificacion de limites articulares
				titha3 = auxM;
			}
			else{
				HAL_UART_Transmit(&huart3, "ErrorPos\n\r", 10, 100);
			}
			break;
		default:
			HAL_UART_Transmit(&huart3, "ErrorMotor\n\r", 12, 100);
			break;
		}
		break;
	case 'P':												//:Px0.1 y0.1 z-0.5 \0 (Eje, valor, espacio, Eje, valor, espacio, Eje, valor, espacio)
	case 'p':
		HAL_UART_Transmit(&huart3, "Punto\n\r", 7, 100);
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
				HAL_UART_Transmit(&huart3, "PuntoX_ok\n\r", 11, 100);
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
				HAL_UART_Transmit(&huart3, "PuntoY_ok\n\r", 11, 100);
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
				HAL_UART_Transmit(&huart3, "PuntoZ_ok\n\r", 11, 100);
			}
			i++;
		}
		cinematicaInversa(Pfin.x, Pfin.y, Pfin.z);
		break;
	case 'H':
	case 'h':
		HAL_UART_Transmit(&huart3, "Homing\n\r", 8, 100);
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
		HAL_UART_Transmit(&huart3, "Fin_Homing\n\r", 12, 100);

		break;
	case 'R':			//Set RPM
	case 'r':
		HAL_UART_Transmit(&huart3, "RPM\n\r", 5, 100);
		break;
	default:
		HAL_UART_Transmit(&huart3, "Nada\n\r", 6, 100);
		break;
	}
}
