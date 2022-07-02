/*
 * interpretaComando.h
 *
 *  Created on: 12 sep. 2020
 *      Author: Santiago
 */

#ifndef INC_INTERPRETACOMANDO_H_
#define INC_INTERPRETACOMANDO_H_

#include "main.h"
#include "stdlib.h"
#include "trayectoria.h"
#include "homing.h"
#include "cinematica.h"

extern Vec3D Pini;
extern Vec3D Pfin;

extern UART_HandleTypeDef huart3;
extern uint8_t rx_buffer[30];
extern uint16_t valor;
extern double titha1;
extern double titha2;
extern double titha3;

/*
extern float_t Pxini;
extern float_t Pyini;
extern float_t Pzini;
extern float_t Pxfin;
extern float_t Pyfin;
extern float_t Pzfin;
*/

extern double flagErrorEndStop;

void interpretaComando(void);


#endif /* INC_INTERPRETACOMANDO_H_ */
