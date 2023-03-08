/*
 * trajectory.h
 *
 *  Created on: Jun 22, 2022
 *      Authors: Elias Correa y Eliseo Elorga
 */


#ifndef INC_TRAJECTORY_H_
#define INC_TRAJECTORY_H_

#include "main.h"

#define pi 3.14159265359
#define RAD_TO_DEG 180.0 / pi
#define RADs_TO_RPM (60.00/(2.0*pi))

extern float q,qd,qdd,qddd;
extern float amax,jmax,vmax,vi,vf;
extern float vmin,jmin,amin;


extern Motor motor1,motor2,motor3;
extern double omega[3];
extern uint32_t periodoM1, periodoM2, periodoM3;
extern double periodoM[3];
extern double rpm1, rpm2, rpm3;
extern double ErrorPeriodo[3];
extern double ErrorAcumuladoPeriodo[3];


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



void setProfilTimer(void);



#endif /* INC_TRAJECTORY_H_ */
