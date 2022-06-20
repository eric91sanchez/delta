/*
 * cinematica_inversa.h
 *
 *  Created on: 12 sep. 2020
 *      Author: Santiago
 */

#ifndef INC_TRAYECTORIA_H_
#define INC_TRAYECTORIA_H_

#include "main.h"

extern double titha1;
extern double titha2;
extern double titha3;
extern float_t Pxini;
extern float_t Pyini;
extern float_t Pzini;
extern float_t Pxfin;
extern float_t Pyfin;
extern float_t Pzfin;
extern double TiempoTotal;
extern double X;
extern double DX;
extern double DDX;
extern double omega[3];
extern double FlagTrayecto;
extern double omega1, omega2, omega3;
extern uint32_t periodoM1, periodoM2, periodoM3;
extern double periodoM[3];
extern double rpm1, rpm2, rpm3;
extern double ErrorPeriodo[3];
extern double ErrorAcumuladoPeriodo[3];
extern double FlagTrayectoM1, FlagTrayectoM2, FlagTrayectoM3;
extern double Fcl;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim15;
extern UART_HandleTypeDef huart3;

typedef struct{
	double m[3][3];
}array2D;


void inicializarTrayectoria(double XiHip1, double XfHip4, double V0Hip, double Vf, double Vmax, double Amax, double Jerk );
void obtenerVelCurva(double t);
void cinematicaInversa(double Pxaux, double Pyaux, double Pzaux);
array2D transpuesta (double Maux[3][3]);
double productDot(double VectA[], double VectB[]);
double determinante (double Maux[3][3]);
array2D adjunta (double Maux[3][3]);
array2D inversa (double A[3][3]);
array2D productMatriz (double MatrizA[3][3], double MatrizB[3][3]);
void jacobianoInverso(double Vxaux, double Vyaux, double Vzaux, double Pxaux, double Pyaux, double Pzaux);
void SetPerfilTimers(double omeg1, double omeg2, double omeg3);



#endif /* INC_TRAYECTORIA_H_ */
