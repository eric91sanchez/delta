/*
 * motor.c
 *
 *  Created on: 6 sep. 2020
 *      Author: Santiago River
 *      Updated by: Elias Correa y Eliseo Elorga
 */
#include "motor.h"

void configMotor (Motor *motor, int i){

    i = i-1; // Esto es ya que el usario ingresara 1,2 o 3

	volatile double diffAngles;

	if (motor->theta >= motor->currentAngle) {
		if(ES1i_PRESSED){
			flagErrorEndStop = 1;
		} else {
			diffAngles = motor->theta - motor->currentAngle;	//Calculo cuantos grados se tiene que mover.
			motor->currentAngle = motor->theta;
			if (i==0) negative_Dir_MOTOR_1; //Antihorario visto de frente
            if (i==1) negative_Dir_MOTOR_2;
            if (i==2) negative_Dir_MOTOR_3;
		}
	} else {
		if(ES1s_PRESSED){
			flagErrorEndStop = 1;
		} else {
			diffAngles = motor->currentAngle - motor->theta;	//Calculo cuantos grados se tiene que mover.
			motor->currentAngle = motor->theta;

			if (i==0) positive_Dir_MOTOR_1; // Horario visto de frente
            if (i==1) positive_Dir_MOTOR_2;
            if (i==2) positive_Dir_MOTOR_3;
		}

	}
	// Calculo el error de posicion por casteo a int, y cuando supera la unidad lo compenzo------
	motor->calcStep = (diffAngles * STEPREV) / 360; // Almaceno el remante de los numeros de pasos y
	motor->numStep = (uint32_t) motor->calcStep;	// cuando pasa la unidad lo sumo al numero de pasos para
	motor->remanente = motor->remanente + (motor->calcStep - motor->numStep);// que el error no se amplifique.
	if (motor->remanente >= 1) {
		motor->remanente = motor->remanente - 1;
		motor->numStep = motor->numStep + 1;
	}

}

