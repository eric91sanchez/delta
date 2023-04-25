/*
 * kinematic.h
 *
 *  Created on: 1 jul. 2022
 *      Author: Elias Correa y Eliseo Elorga
 */

#ifndef INC_KINEMATIC_H_
#define INC_KINEMATIC_H_

#include <GlobalFunc.h>
#include <main.h>

extern Motor motor1,motor2,motor3;
void inverseKinematic(Vec3D _Pfin);

#endif /* INC_KINEMATIC_H_ */
