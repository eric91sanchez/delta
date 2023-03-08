/*
 * interpretaComando.h
 *
 *  Created on: 12 sep. 2020
 *      Author: Santiago Rivier
 *      Updated by: Elias Correa y Eliseo Elorga
 */

#ifndef INC_INTERPRETACOMANDO_H_
#define INC_INTERPRETACOMANDO_H_

#include <kinematic.h>
#include "main.h"
#include "stdlib.h"
#include "trajectory.h"
#include "homing.h"

extern Vec3D Pini;
extern Vec3D Pfin;

extern UART_HandleTypeDef huart3;
extern uint8_t rx_buffer[30];

extern double titha1;
extern double titha2;
extern double titha3;

extern float vmax,amax,jmax,vi,vf;


extern double flagErrorEndStop;

void interpretaComando(void);


#endif /* INC_INTERPRETACOMANDO_H_ */
