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
#define RADs_TO_RPM (60.0/(2.0*pi))

extern double q,qd,qdd,qddd;
extern double amax,jmax,vmax,vi,vf;
extern double vmin,jmin,amin;


extern Motor motor1,motor2,motor3;
extern double rpm1, rpm2, rpm3;
/*

extern double ErrorPeriodo[3];
extern double ErrorAcumuladoPeriodo[3];
*/

extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;

extern UART_HandleTypeDef huart3;


double* update_ScurveTraj(double _qi ,double _qf, double vi,double vf ,double vmax,double amax,double jmax);
double get_Straj(double t,double _qi, double _qf,double *params);

void setProfilTimer(void);



#endif /* INC_TRAJECTORY_H_ */
