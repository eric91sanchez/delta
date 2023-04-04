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

		diffAngles = motor->theta - motor->currentAngle;	//Calculo cuantos grados se tiene que mover.
		motor->currentAngle = motor->theta;
		if (i==0) negative_Dir_MOTOR_1; //Antihorario visto de frente
		if (i==1) negative_Dir_MOTOR_2;
		if (i==2) negative_Dir_MOTOR_3;

	} else {

		diffAngles = motor->currentAngle - motor->theta;	//Calculo cuantos grados se tiene que mover.
		motor->currentAngle = motor->theta;

		if (i==0) positive_Dir_MOTOR_1; // Horario visto de frente
		if (i==1) positive_Dir_MOTOR_2;
		if (i==2) positive_Dir_MOTOR_3;
	}

	/*Calculamos el error de posicion por caseteo a "int" y cuando supera la unidad la compenzamos, para ello
	 * almacenamos el remanente de cada operacion. Cuando este remanente supera la unidad, le sumamos 1 a el numero
	 * de pasos que debe efectuar el motor */

	motor->calcStep = (diffAngles * STEPREV) / 360;
	motor->numStep = (uint32_t) motor->calcStep;
	motor->remainder += (motor->calcStep - motor->numStep);
	if (motor->remainder >= 1) {
		motor->remainder -= 1;
		motor->numStep += 1;
	}

}

