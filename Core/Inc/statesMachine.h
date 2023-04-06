/*
 * statesMachine.h
 *
 *  Created on: Apr 3, 2023
 *      Author: Elias Correa y Eliseo Elorga
 */

#ifndef INC_STATESMACHINE_H_
#define INC_STATESMACHINE_H_

#include "main.h"
#include "trajectory.h"
#include "homing.h"
#include "motor.h"


extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim2,htim3,htim4,htim12,htim13,htim14,htim15,htim5;
extern Motor motor1,motor2,motor3;
extern double time;

void robotInitialization(void);
void statesMachineLoop(void);


#endif /* INC_STATESMACHINE_H_ */
