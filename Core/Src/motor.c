/*
 * motor.c
 *
 *  Created on: 6 sep. 2020
 *      Author: Santiago
 */
#include "motor.h"

double stepRev = 1600; //Numero de pasos por revolucion (Observar tabla de micropasos del Driver y verificar)
double reductor = 6;							//Relacion de reduccion del reductor

double anguloActual1 = 0;
double anguloActual2 = 0;
double anguloActual3 = 0;
double calculoStep1;
double calculoStep2;
double calculoStep3;
double remanente1;
double remanente2;
double remanente3;
double Periodo;		//periodo de timer PWM
//double ErrorPeriodo;
//double ErrorAcumuladoPeriodo[3];

void configStepMotor1(double angulo) {
	double anguloDif;
	pMotor1 = 0;
	if (angulo >= anguloActual1) {
		if(!(HAL_GPIO_ReadPin(E_EndStop1_Inf_GPIO_Port, E_EndStop1_Inf_Pin))){
			flagErrorEndStop = 1;
		} else {
			anguloDif = angulo - anguloActual1;	//Calculo cuantos grados se tiene que mover.
			anguloActual1 = angulo;
			HAL_GPIO_WritePin(S_DirPaP1_GPIO_Port, S_DirPaP1_Pin, GPIO_PIN_SET); //Antihorario visto de frente
		}
	} else {
		if(!(HAL_GPIO_ReadPin(E_EndStop1_Sup_GPIO_Port, E_EndStop1_Sup_Pin))){
			flagErrorEndStop = 1;
		} else {
			anguloDif = anguloActual1 - angulo;	//Calculo cuantos grados se tiene que mover.
			anguloActual1 = angulo;
			HAL_GPIO_WritePin(S_DirPaP1_GPIO_Port, S_DirPaP1_Pin, GPIO_PIN_RESET); // Horario visto de frente
		}

	}
	// Calculo el error de posicion por casteo a int, y cuando supera la unidad lo compenzo------
	calculoStep1 = (anguloDif * stepRev * reductor) / 360; // Almaceno el remante de los numeros de pasos y
	numStep1 = (uint32_t) calculoStep1;	// cuando pasa la unidad lo sumo al numero de pasos para
	remanente1 = remanente1 + (calculoStep1 - numStep1);// que el error no se amplifique.
	if (remanente1 >= 1) {
		remanente1 = remanente1 - 1;
		numStep1 = numStep1 + 1;
	}
}

void configStepMotor2(double angulo) {
	double anguloDif;
	pMotor2 = 0;
	if (angulo >= anguloActual2) {
		if(!(HAL_GPIO_ReadPin(E_EndStop2_Inf_GPIO_Port, E_EndStop2_Inf_Pin))){
			flagErrorEndStop = 1;
		} else {
			anguloDif = angulo - anguloActual2;	//Calculo cuantos grados se tiene que mover.
			anguloActual2 = angulo;
			HAL_GPIO_WritePin(S_DirPaP2_GPIO_Port, S_DirPaP2_Pin, GPIO_PIN_SET); // VERIFICAR SENTIDO DE GIRO
		}
	} else {
		if(!(HAL_GPIO_ReadPin(E_EndStop2_Sup_GPIO_Port, E_EndStop2_Sup_Pin))){
			flagErrorEndStop = 1;
		} else {
			anguloDif = anguloActual2 - angulo;	//Calculo cuantos grados se tiene que mover.
			anguloActual2 = angulo;
			HAL_GPIO_WritePin(S_DirPaP2_GPIO_Port, S_DirPaP2_Pin, GPIO_PIN_RESET); // VERIFICAR SENTIDO DE GIRO
		}

	}
	// Calculo el error de posicion por casteo a int, y cuando supera la unidad lo compenzo------
	calculoStep2 = (anguloDif * stepRev * reductor) / 360; // Almaceno el remante de los numeros de pasos y
	numStep2 = (uint32_t) calculoStep2;	// cuando pasa la unidad lo sumo al numero de pasos para
	remanente2 = remanente2 + (calculoStep2 - numStep2);// que el error no se amplifique.
	if (remanente2 >= 1) {
		remanente2 = remanente2 - 1;
		numStep2 = numStep2 + 1;
	}
}

void configStepMotor3(double angulo) {
	double anguloDif;
	pMotor3 = 0;
	if (angulo >= anguloActual3) {
		if(!(HAL_GPIO_ReadPin(E_EndStop3_Inf_GPIO_Port, E_EndStop3_Inf_Pin))){
			flagErrorEndStop = 1;
		} else {
			anguloDif = angulo - anguloActual3;	//Calculo cuantos grados se tiene que mover.
			anguloActual3 = angulo;
			HAL_GPIO_WritePin(S_DirPaP3_GPIO_Port, S_DirPaP3_Pin, GPIO_PIN_SET); // Brazo Baja (Antihorario)
		}
	} else {
		if(!(HAL_GPIO_ReadPin(E_EndStop3_Inf_GPIO_Port, E_EndStop3_Inf_Pin))){
					flagErrorEndStop = 1;
		} else {
			anguloDif = anguloActual3 - angulo;	//Calculo cuantos grados se tiene que mover.
			anguloActual3 = angulo;
			HAL_GPIO_WritePin(S_DirPaP3_GPIO_Port, S_DirPaP3_Pin, GPIO_PIN_RESET); // Brazo Sube (Horario)
		}
	}
	// Calculo el error de posicion por casteo a int, y cuando supera la unidad lo compenzo------
	calculoStep3 = (anguloDif * stepRev * reductor) / 360; // Almaceno el remante de los numeros de pasos y
	numStep3 = (uint32_t) calculoStep3;	// cuando pasa la unidad lo sumo al numero de pasos para
	remanente3 = remanente3 + (calculoStep3 - numStep3);// que el error no se amplifique.
	if (remanente3 >= 1) {
		remanente3 = remanente3 - 1;
		numStep3 = numStep3 + 1;
	}
}
