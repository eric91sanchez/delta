/*
 *  GlobalFunc.c
 *
 *  Created on: Jun 22, 2022
 *      Author: Elias Correa y Eliseo Elorga
 */

#include <GlobalFunc.h>


double dotProduct(double VectA[], double VectB[],int n) {
	double product = 0;
	for (int i = 0; i < n; i++) {				//Producto punto entre vectores
		product += VectA[i] * VectB[i];
	}
	return product;
}

void matrixTranspose(double A[3][3], double result[3][3]) {
    int i, j;

    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            result[j][i] = A[i][j];
        }
    }
}

void inv(double A[3][3], double A_inv[3][3]) {

	// Calculamos el determinante de la matriz

	double det = 0;
	for (int i = 0; i < 3; i++) {
		det += A[0][i] * (A[1][(i+1)%3]*A[2][(i+2)%3] - A[1][(i+2)%3]*A[2][(i+1)%3]);
	}

	// Verificamos si la matriz es invertible
	if (det == 0) {
		return;
	}

	// Calculamos la matriz adjunta
	double adj[3][3];

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			adj[i][j] = ((A[(j+1)%3][(i+1)%3] * A[(j+2)%3][(i+2)%3]) - (A[(j+1)%3][(i+2)%3] * A[(j+2)%3][(i+1)%3]));
		}
	}

	// Calculamos la matriz inversa
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			A_inv[i][j] = adj[i][j] / det;
		}
	}
}

void matrixProduct(double A[3][3], double B[3][3], double product[3][3]) {
    int i, j, k;

    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
        	product[i][j] = 0;
            for(k = 0; k < 3; k++) {
            	product[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
