/*
 * homing.h
 *
 *  Created on: Sep 10, 2020
 *      Author: Santiago Rivier
 *      Updated by: Elias Correa y Eliseo Elorga
 */

#ifndef INC_HOMING_H_
#define INC_HOMING_H_

#include "main.h"
#include "interpretaComando.h"


extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;

extern Motor motor1,motor2,motor3;
extern bool homFin;
void homing(void);



#endif /* INC_HOMING_H_ */
