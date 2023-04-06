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


extern Vec3D Pini,Pfin;
extern UART_HandleTypeDef huart3;
extern uint8_t rx_buffer[BUFFER_SIZE];
extern bool receptionFlag,readFile;
extern statesMachine state;


extern double flagErrorEndStop;

void interpretaComando(void);


#endif /* INC_INTERPRETACOMANDO_H_ */
