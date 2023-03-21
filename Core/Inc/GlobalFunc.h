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


double dotProduct(double VectA[], double VectB[],int n);
void inv(double A[3][3], double A_inv[3][3]);
void matrixProduct(double A[3][3], double B[3][3], double product[3][3]);
void matrixTranspose(double A[3][3], double result[3][3]);


#endif /* INC_GLOBALFUNC_H_ */
