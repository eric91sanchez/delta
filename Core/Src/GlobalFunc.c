/*
 * operaciones.c
 *
 *  Created on: Jun 22, 2022
 *      Author: EliasC
 */

#include <GlobalFunc.h>

array2D transpuesta(double Maux[3][3]) {
	array2D MTaux;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			MTaux.m[j][i] = Maux[i][j];	//transpuesta de la matriz l2 JP=[l21';l22';l23'];
		}
	}
	return MTaux;
}

double productDot(double VectA[], double VectB[]) {
	double dotaux = 0;
	for (int i = 0; i < 3; ++i) {				//Producto punto entre vectores
		dotaux = dotaux + VectA[i] * VectB[i];
	}
	return dotaux;
}

double determinante(double Maux[3][3]) {
	double det;
	det = Maux[0][0] * Maux[1][1] * Maux[2][2]; // + Maux[0][1] * Maux[1][2] * Maux[2][0] + Maux[1][0] * Maux[2][1] * Maux[0][2] - Maux[2][0] * Maux[1][1] * Maux[0][2] - Maux[1][0] * Maux[0][1] * Maux[2][2] - Maux[2][1] * Maux[1][2] * Maux[0][0];
	return det;
}

array2D adjunta(double Maux[3][3]) {
	array2D adj;
	adj.m[0][0] = Maux[1][1] * Maux[2][2] - Maux[2][1] * Maux[1][2];
	adj.m[0][1] = Maux[1][0] * Maux[2][2] - Maux[2][0] * Maux[1][2];
	adj.m[0][2] = Maux[1][0] * Maux[2][1] - Maux[2][0] * Maux[1][1];
	adj.m[1][0] = Maux[0][1] * Maux[2][2] - Maux[2][1] * Maux[0][2];
	adj.m[1][1] = Maux[0][0] * Maux[2][2] - Maux[2][0] * Maux[0][2];
	adj.m[1][2] = Maux[0][0] * Maux[2][1] - Maux[2][0] * Maux[0][1];
	adj.m[2][0] = Maux[0][1] * Maux[1][2] - Maux[1][1] * Maux[0][2];
	adj.m[2][1] = Maux[0][0] * Maux[1][2] - Maux[1][0] * Maux[0][2];
	adj.m[2][2] = Maux[0][0] * Maux[1][1] - Maux[1][0] * Maux[0][1];
	return adj;
}

array2D inversa(double A[3][3]) {
	array2D Minv;
	array2D Maux;
	double det;
	Maux = adjunta(A);
	Maux = transpuesta(Maux.m);
	det = determinante(A);
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			Minv.m[i][j] = (1 / det) * Maux.m[i][j];
		}
	}
	return Minv;
}

array2D productMatriz(double MatrizA[3][3], double MatrizB[3][3]) {
	array2D producto;

	for (int k = 0; k < 3; ++k) {
		for (int i = 0; i < 3; ++i) {
			double suma = 0;
			for (int j = 0; j < 3; ++j) {
				suma += MatrizA[i][j] * MatrizB[j][k];
			}
			producto.m[i][k] = suma;
		}
	}
	return producto;
}


