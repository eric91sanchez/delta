/*
 * operaciones.h
 *
 *  Created on: Jun 22, 2022
 *      Author: Elias Correa y Eliseo Elorga
 */

#ifndef INC_GLOBALFUNC_H_
#define INC_GLOBALFUNC_H_

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

typedef struct{
	double m[3][3];
}array2D;

array2D transpuesta (double Maux[3][3]);
double productDot(double VectA[], double VectB[]);
double determinante (double Maux[3][3]);
array2D adjunta (double Maux[3][3]);
array2D inversa (double A[3][3]);
array2D productMatriz (double MatrizA[3][3], double MatrizB[3][3]);


#endif /* INC_GLOBALFUNC_H_ */
