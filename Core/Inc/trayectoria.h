/*
 * cinematica_inversa.h
 *
 *  Created on: 12 sep. 2020
 *      Author: Santiago
 */

#ifndef INC_TRAYECTORIA_H_
#define INC_TRAYECTORIA_H_

#include "main.h"

#define pi 3.14159265359
#define RAD_TO_DEG 180.0 / pi




extern float q,qd,qdd,qddd;
extern float amax,jmax,vmax,vi,vf;
extern float vmin,jmin,amin;

extern double omega[3];
extern double omega1, omega2, omega3;
extern uint32_t periodoM1, periodoM2, periodoM3;
extern double periodoM[3];
extern double rpm1, rpm2, rpm3;
extern double ErrorPeriodo[3];
extern double ErrorAcumuladoPeriodo[3];

//extern double Fcl;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim15;
extern UART_HandleTypeDef huart3;


void update_ScurveTraj(float _qi ,float _qf, float vi,float vf ,float vmax,float amax,float jmax);
void get_Straj(float t);



void SetPerfilTimers(double omeg1, double omeg2, double omeg3);



#endif /* INC_TRAYECTORIA_H_ */
