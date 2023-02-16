/*
 * motor.h
 *
 *  Created on: 6 sep. 2020
 *      Author: Santiago River
 *      Updated by: Elias Correa y Eliseo Elorga
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"


extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;
extern UART_HandleTypeDef huart3;
extern uint32_t numStep1;		//Numero de pasos PaP1 consigna
extern uint32_t numStep2;		//Numero de pasos PaP2 consigna
extern uint32_t numStep3;		//Numero de pasos PaP3 consigna
extern uint32_t pMotor1;		//Pasos de motor PaP1 actual
extern uint32_t pMotor2;		//Pasos de motor PaP2 actual
extern uint32_t pMotor3;		//Pasos de motor PaP3 actual
//extern double Fcl;
extern double flagErrorEndStop;




void configStepMotor1(double angulo);
void configStepMotor2(double angulo);
void configStepMotor3(double angulo);


#endif /* INC_MOTOR_H_ */
