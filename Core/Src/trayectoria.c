/*
 * cinematica_inversa.c
 *
 *  Created on: 12 sep. 2020
 *      Author: Santiago
 */

#include <operaciones.h>
#include "trayectoria.h"

//Una constante a diferencia de una macro ocupa un espacio(memoria) durante la ejecución del programa.
//Ya que son valores que nunca van a cambiar en el programa, es mejor definirlos como tal (constantes)

#define C30 0.866025403784439
#define S30 0.500000000000000
#define C150 -0.866025403784439
#define S150 0.500000000000000
#define pi 3.14159265359
#define RAD_TO_DEG 180.0 / pi



#define fi1 270
#define fi2 30
#define fi3 150
#define L1 0.4
#define L2 0.6
#define R 0.168
#define r 0.070


// Para tihta = 0, Pxaux=0 Pyaux=0 y Pzaux=0.334658034417224

//double fi1 = 270;
//double fi2 = 30;
//double fi3 = 150;
//double L1 = 0.4;
//double L2 = 0.6;
//double R = 0.168;
//double r = 0.070;


double A1, B1, C1, XJ1_1, XJ1_2, YJ1_1, YJ1_2, ZJ1_1, ZJ1_2, A11, A12, A13, A14, A15;
double A2, B2, C2, XJ2_1, XJ2_2, YJ2_1, YJ2_2, ZJ2_1, ZJ2_2;
double A3, B3, C3, XJ3_1, XJ3_2, YJ3_1, YJ3_2, ZJ3_1, ZJ3_2;
double F1[3] = { 0, -0.1680, 0 };
double F2[3] = { 0.145492267835786, 0.084, 0 };
double F3[3] = { -0.145492267835786, 0.084, 0 };
double SA1[3] = { 1, 0, 0 };
double SA2[3] = { -0.5, 0.866025403784439, 0 };
double SA3[3] = { -0.5, -0.866025403784439, 0 };
double E1[3];
double E2[3];
double E3[3];
double l1[3][3];
double l2[3][3];
array2D Jcd;
double Jci[3][3];
array2D Jci_inv;
array2D Jinv;
double cross_P1[3];
double cross_P2[3];
double cross_P3[3];
double dot[3];

//double C30 = 0.866025403784439;
//double S30 = 0.500000000000000;
//double C150 = -0.866025403784439;
//double S150 = 0.500000000000000;
//double pi = 3.14159265359;

double Ai, Af, a0Hip1, a1Hip1, a2Hip1, a3Hip1, a0Hip2, a1Hip2, a2Hip2, a3Hip2,
		a0Hip3, a1Hip3, a2Hip3, a3Hip3, a0Hip4, a1Hip4, a2Hip4, a3Hip4;
double XfHip1, XfHip2, XfHip3, XiHip3;
double Vm, VfHip1, VfHip3;
double tfHip1, tiHip2, tfHip2, tiHip3, tfHip3, tiHip4, TiempoTotX, tfHip4;
double DVHip, Aaux1, ViHip2, XiHip2, Aaux, XiHip3, ViHip4, XiHip4;

//(Posinicio , Posfin , Vinicio , Vfin , Vmaxima, Amaxima, jerk)

void inicializarTrayectoria(double XiHip1, double XfHip4, double V0Hip, double Vf, double Vmax, double Amax, double Jerk) {

	if (XfHip4 < XiHip1) {
		Ai = -Amax;
		Af = +Amax;
		Vm = -Vmax;
		Jerk = -Jerk;
	} else if (XfHip4 > XiHip1) {
		Ai = Amax;
		Af = -Amax;
		Vm = Vmax;
	}

	if (V0Hip > Vmax) {
		Ai = -Amax;
		Af = Amax;
	}
	if (Vf > Vmax) {
		Ai = Amax;
		Af = -Amax;
	}

	for (uint8_t Iteracion = 0; Iteracion < 100; Iteracion++) { // Itero 40 veces, solo en caso de que no se alcance la vel maxima, sino salgo
		DVHip = (Ai * Ai) / (2 * Jerk) * 2;

		if (fabs(DVHip) > fabs(Vm - V0Hip)) { // Para cuando la velocidad inicial es muy cercana a la maxima
			if (fabs(Vm - V0Hip) != 0) {
				Aaux1 = ((Ai) / fabs(Ai)) * sqrt(fabs(Vm - V0Hip) * fabs(Jerk));
				Ai = Aaux1;
			}
		}

		if (fabs(Vm - V0Hip) == 0) { // Para cuando la velocidad inicial es igual a la maxima
			a3Hip1 = 0;
			a2Hip1 = 0;
			a1Hip1 = 0;
			a0Hip1 = 0;
			tfHip1 = 0;
			tiHip2 = 0;
			tfHip2 = 0;
			XfHip2 = XiHip1;
		} else {
			//  Polinomio de 3° orden (Jerk cte, parabola en V, hiperbola en X)         HIPERBOLA 1
			a3Hip1 = (Jerk / 6);
			a2Hip1 = 0;
			tfHip1 = ((Ai / (6 * a3Hip1)));
			a1Hip1 = V0Hip;
			VfHip1 = a1Hip1 + (2 * a2Hip1 * tfHip1)
					+ 3 * a3Hip1 * pow(tfHip1, 2);
			a0Hip1 = XiHip1;
			XfHip1 = a0Hip1 + (a1Hip1 * tfHip1) + a2Hip1 * pow(tfHip1, 2)
					+ a3Hip1 * pow(tfHip1, 3);
			// Polinomio de 2° orden (Acel cte, trapecio en V, parabola en X)           PARABOLA 1
			a3Hip2 = (-Jerk / 6);
			a2Hip2 = Ai / 2;
			ViHip2 = Vm - 2.0 * a2Hip2 * (Ai / Jerk)
					- 3 * a3Hip2 * (Ai / Jerk) * (Ai / Jerk);
			tiHip2 = ((ViHip2 - VfHip1) / Ai) + tfHip1;
			XiHip2 = XfHip1 + (Ai / 2) * pow(((ViHip2 - VfHip1) / Ai), 2)
					+ VfHip1 * ((ViHip2 - VfHip1) / Ai);    //x=(Cte/2)*t^2+Vi*t
					//  Polinomio de 3° orden (Jerk cte, parabola en V, hiperbola en X)         HIPERBOLA 2
			tfHip2 = fabs(Ai / (-Jerk)) + tiHip2;
			a1Hip2 = ViHip2;
			a0Hip2 = XiHip2;
			XfHip2 = a0Hip2 + (a1Hip2 * (tfHip2 - tiHip2))
					+ a2Hip2 * pow((tfHip2 - tiHip2), 2.0)
					+ a3Hip2 * pow((tfHip2 - tiHip2), 3.0);
		}

		// Condiciones borde:    Se calcula de adelante hacia atras
		DVHip = pow(Af, 2) / (2 * Jerk) * 2;
		if (fabs(DVHip) > fabs(Vm - Vf)) { // Para cuando la velocidad final es muy cercana a la maxima
			if (fabs(Vm - Vf) != 0) {
				Aaux = ((Af) / fabs(Af)) * sqrt(fabs(Vm - Vf) * fabs(Jerk));
				Af = Aaux;
			}
		}
		if (fabs(Vm - Vf) == 0) { // Para cuando la velocidad final es igual a la maxima
			VfHip3 = Vf;
			XiHip3 = XfHip4;
			tiHip3 = fabs((XiHip3 - XfHip2) / Vm) + tfHip2;
			TiempoTotX = tiHip3;
		} else {
			ViHip4 = pow(Af, 2) / (2 * Jerk) + Vf;
			XiHip4 =
					XfHip4
							- (fabs(pow(Af, 3) / (6 * pow(Jerk, 2)))
									* ((ViHip4) / fabs(ViHip4))
									+ Vf * (fabs(Af / Jerk)));
			VfHip3 = Vm - pow(Af, 2) / (2 * (Jerk));
			XfHip3 = XiHip4
					- (((ViHip4) / fabs(ViHip4)) * (fabs(Af) / 2)
							* pow(((ViHip4 - VfHip3) / Af), 2)
							+ ViHip4 * ((ViHip4 - VfHip3) / Af));
			XiHip3 = XfHip3 - Vm * (Af / (-Jerk))
					- (pow(Af, 3) / (6 * (pow(-Jerk, 2))));
			tiHip3 = fabs((XiHip3 - XfHip2) / Vm) + tfHip2;
			//  Polinomio de 3° orden (Jerk cte, parabola en V, hiperbola en X)         HIPERBOLA 3
			a3Hip3 = (-Jerk / 6);
			a2Hip3 = 0;
			tfHip3 = (Af / (-Jerk)) + tiHip3;
			a1Hip3 = Vm;
			a0Hip3 = XiHip3;
			// Polinomio de 2° orden (Acel cte, trapecio en V, parabola en X)           PARABOLA 2
			tiHip4 = ((ViHip4 - VfHip3) / Af) + tfHip3;
			//  Polinomio de 3° orden (Jerk cte, parabola en V, hiperbola en X)         HIPERBOLA 4
			a3Hip4 = Jerk / 6;
			a2Hip4 = Af / 2;
			tfHip4 = fabs(Af / Jerk) + tiHip4;
			a1Hip4 = ViHip4;
			a0Hip4 = XiHip4;
			TiempoTotX = tfHip4;
		}
		if (XiHip1 < XfHip4) {      // Por si va en un sentido o en el otro
			if (XiHip3 < XfHip2) { // la superposicion de la hip 3 con la 2, acusa de que no se alcanzó la vel maxima
				Vm = Vm - (Vmax * 0.01) * (Vm / fabs(Vm));
			} else {
				break;
			}
		} else {
			if (XiHip3 > XfHip2) { // la superposicion de la hip 3 con la 2, acusa de que no se alcanzó la vel maxima
				Vm = Vm - (Vmax * 0.01) * (Vm / fabs(Vm));
			} else {
				break;
			}
		}
	}
	TiempoTotal = tfHip4;
}

void obtenerVelCurva(double t) {
	/*   -----------------------------------------------------------------------
	 * 	 Funcion que devuelve la velocidad de la trayectoria en un instante dado
	 *	 Entrada: tiempo actual,
	 *	 Salida:  velocidad lineal en m/s en el instante de tiempo t
	 -----------------------------------------------------------------------*/
	if (t <= tfHip1) {                                      // Hiperbola 1
		X = a0Hip1 + a1Hip1 * t + a2Hip1 * pow(t, 2) + a3Hip1 * pow(t, 3);
		DX = a1Hip1 + 2.0 * a2Hip1 * t + 3 * a3Hip1 * pow(t, 2);
		DDX = 2.0 * a2Hip1 + 6 * a3Hip1 * t;
	} else if (t <= (tiHip2)) {                              // Parabola 1
		X = (Ai / 2) * pow((t - tfHip1), 2) + VfHip1 * (t - tfHip1) + XfHip1;
		DX = Ai * (t - tfHip1) + VfHip1;
		DDX = Ai;
	} else if (t <= (tfHip2)) {                            // Hiperbola 2
		X = a0Hip2 + a1Hip2 * (t - tiHip2) + a2Hip2 * pow((t - tiHip2), 2)
				+ a3Hip2 * pow((t - tiHip2), 3);
		DX = a1Hip2 + 2.0 * a2Hip2 * (t - tiHip2)
				+ 3 * a3Hip2 * pow((t - tiHip2), 2);
		DDX = 2.0 * a2Hip2 + 6 * a3Hip2 * (t - tiHip2);
	} else if (t <= (tiHip3)) {                              // Recta
		X = Vm * (t - tfHip2) + XfHip2;
		DX = Vm;
		DDX = 0;
	} else if (t <= (tfHip3)) {                            // Hiperbola 3
		X = a0Hip3 + a1Hip3 * (t - tiHip3) + a2Hip3 * pow((t - tiHip3), 2)
				+ a3Hip3 * pow((t - tiHip3), 3);
		DX = a1Hip3 + 2.0 * a2Hip3 * (t - tiHip3)
				+ 3 * a3Hip3 * pow((t - tiHip3), 2);
		DDX = 2.0 * a2Hip3 + 6 * a3Hip3 * (t - tiHip3);
	} else if (t <= (tiHip4)) {                            // Parabola 2
		X = (Af / 2) * pow((t - tfHip3), 2) + VfHip3 * (t - tfHip3) + XfHip3;
		DX = Af * (t - tfHip3) + VfHip3;
		DDX = Af;
	} else if (t <= TiempoTotX) {                               // Hiperbola 4
		X = a0Hip4 + a1Hip4 * (t - tiHip4) + a2Hip4 * pow((t - tiHip4), 2)
				+ a3Hip4 * pow((t - tiHip4), 3);
		DX = a1Hip4 + 2.0 * a2Hip4 * (t - tiHip4)
				+ 3 * a3Hip4 * pow((t - tiHip4), 2);
		DDX = 2.0 * a2Hip4 + 6.0 * a3Hip4 * (t - tiHip4);
	} else if (t > TiempoTotX) {                               // Tiempo extra
		DX = 0;
		DDX = 0;
	} else {												// Tiempo erroneo
		X = 0;
		DX = 0;
		DDX = 0;
	}

}

void cinematicaInversa(Vec3D _Pfin) {

	A1 = 1.0 + pow(((-2.0  * (R + (_Pfin.y - r))) / (2 * _Pfin.z)), 2);
	A2 = 4.0 + pow(((2.0 * sqrt(3.0) * R * C30 + 2.0 * R * S30 - 2.0 * sqrt(3) * (_Pfin.x + r * C30) - 2.0 * (_Pfin.y + r * S30)) / (2.0 * _Pfin.z)), 2.0);
	A3 = 4.0 + pow(((2.0 * sqrt(3.0) * (_Pfin.x + r * C150) - 2.0 * (_Pfin.y + r * S150) - 2.0 * sqrt(3) * R * C150 + 2.0 * R * S150) / (2.0 * _Pfin.z)), 2.0);

	B1 = (2.0 * R) + (2.0 * ((-2.0  * (R + (_Pfin.y - r))) / (2.0 * _Pfin.z)) * ((-pow(R, 2.0) + pow(L1, 2.0) + pow((_Pfin.y - r), 2.0) + pow(_Pfin.z, 2.0) - pow(L2, 2.0) + pow(_Pfin.x, 2)) / (2 * _Pfin.z)));
	B2 = -2.0 * sqrt(3) * R * C30 - 2.0 * R * S30 + 2.0 * (((2.0 * sqrt(3) * R * C30 + 2.0 * R * S30)	+ (-2 * sqrt(3) * (_Pfin.x + r * C30)	- 2.0 * (_Pfin.y + r * S30))) / (2 * _Pfin.z)) * ((-pow(R, 2) + pow(L1, 2) + pow((_Pfin.x + r * C30), 2) + pow((_Pfin.y + r * S30), 2) + pow(_Pfin.z, 2) - pow(L2, 2)) / (2 * _Pfin.z));
	B3 = 2.0 * sqrt(3) * R * C150 - 2.0 * R * S150 + 2.0 * ((2.0 * sqrt(3) * (_Pfin.x + r * C150) - 2.0 * (_Pfin.y + r * S150) - 2.0 * sqrt(3) * R * C150 + 2.0 * R * S150) / (2 * _Pfin.z)) * ((pow((_Pfin.x + r * C150), 2) + pow((_Pfin.y + r * S150), 2) + pow(_Pfin.z, 2) - pow(L2, 2) - pow(R, 2) + pow(L1, 2)) / (2 * _Pfin.z));

	C1 = (pow(((pow((_Pfin.y - r), 2) + pow(_Pfin.z, 2) - pow(L2, 2) + pow(_Pfin.x, 2) - pow(R, 2) + pow(L1, 2)) / (2 * _Pfin.z)), 2) - pow(L1, 2) + pow(R, 2));
	C2 = pow(R, 2) - pow(L1, 2) + pow(((-pow(R, 2) + pow(L1, 2) + pow((_Pfin.x + r * C30), 2) + pow((_Pfin.y + r * S30), 2) + pow(_Pfin.z, 2) - pow(L2, 2)) / (2.0 * _Pfin.z)), 2.0);
	C3 = pow(R, 2) - pow(L1, 2) + pow(((pow((_Pfin.x + r * C150), 2) + pow((_Pfin.y + r * S150), 2) + pow(_Pfin.z, 2) - pow(L2, 2) - pow(R, 2) + pow(L1, 2)) / (2.0 * _Pfin.z)), 2.0);

	YJ1_1 = (-B1 - sqrt(pow(B1, 2) - 4 * A1 * C1)) / (2 * A1);
	YJ1_2 = (-B1 + sqrt(pow(B1, 2) - 4 * A1 * C1)) / (2 * A1);
	YJ2_1 = (-B2 + sqrt(pow(B2, 2) - 4 * A2 * C2)) / (2 * A2);
	YJ2_2 = (-B2 - sqrt(pow(B2, 2) - 4 * A2 * C2)) / (2 * A2);
	YJ3_1 = (-B3 + sqrt(pow(B3, 2) - 4 * A3 * C3)) / (2 * A3);
	YJ3_2 = (-B3 - sqrt(pow(B3, 2) - 4 * A3 * C3)) / (2 * A3);

	XJ1_1 = 0;
	XJ1_2 = 0;
	XJ2_1 = sqrt(3) * YJ2_1;
	XJ2_2 = sqrt(3) * YJ2_2;
	XJ3_1 = -sqrt(3) * YJ3_1;
	XJ3_2 = -sqrt(3) * YJ3_2;

	ZJ1_1 = ((YJ1_1 * ((-2 * (R + (_Pfin.y - r))) / (2 * _Pfin.z))) + ((pow((_Pfin.y - r), 2) + pow(_Pfin.z, 2) - pow(L2, 2) + pow(_Pfin.x, 2) - pow(R, 2) + pow(L1, 2)) / (2 * _Pfin.z)));
	ZJ1_2 = ((YJ1_2 * ((-2 * (R + (_Pfin.y - r))) / (2 * _Pfin.z))) + ((pow((_Pfin.y - r), 2) + pow(_Pfin.z, 2) - pow(L2, 2) + pow(_Pfin.x, 2) - pow(R, 2) + pow(L1, 2)) / (2 * _Pfin.z)));
	ZJ2_1 = YJ2_1 * (((2 * sqrt(3.0) * R * C30 + 2.0 * R * S30) + (-2 * sqrt(3) * (_Pfin.x + r * C30) - 2.0 * (_Pfin.y + r * S30))) / (2 * _Pfin.z)) + ((-pow(R, 2) + pow(L1, 2) + pow((_Pfin.x + r * C30), 2) + pow((_Pfin.y + r * S30), 2) + pow(_Pfin.z, 2) - pow(L2, 2)) / (2 * _Pfin.z));
	ZJ2_2 = YJ2_2 * (((2 * sqrt(3.0) * R * C30 + 2.0 * R * S30) + (-2 * sqrt(3) * (_Pfin.x + r * C30) - 2.0 * (_Pfin.y + r * S30))) / (2 * _Pfin.z)) + ((-pow(R, 2) + pow(L1, 2) + pow((_Pfin.x + r * C30), 2) + pow((_Pfin.y + r * S30), 2) + pow(_Pfin.z, 2) - pow(L2, 2)) / (2 * _Pfin.z));
	ZJ3_1 = YJ3_1 * ((2 * sqrt(3.0) * (_Pfin.x + r * C150) - 2.0 * (_Pfin.y + r * S150) - 2.0 * sqrt(3) * R * C150 + 2.0 * R * S150) / (2.0 * _Pfin.z)) + ((pow((_Pfin.x + r * C150), 2.0) + pow((_Pfin.y + r * S150), 2) + pow(_Pfin.z, 2.0) - pow(L2, 2) - pow(R, 2) + pow(L1, 2)) / (2 * _Pfin.z));
	ZJ3_2 = YJ3_2 * ((2 * sqrt(3.0) * (_Pfin.x + r * C150) - 2.0 * (_Pfin.y + r * S150) - 2.0 * sqrt(3) * R * C150 + 2.0 * R * S150) / (2.0 * _Pfin.z)) + ((pow((_Pfin.x + r * C150), 2.0) + pow((_Pfin.y + r * S150), 2) + pow(_Pfin.z, 2) - pow(L2, 2) - pow(R, 2) + pow(L1, 2)) / (2 * _Pfin.z));

	titha1 = -1.0 * asin(ZJ1_1 / L1);
	titha1 = titha1 * RAD_TO_DEG;
	titha2 = -1.0 * asin(ZJ2_1 / L1);
	titha2 = titha2 * RAD_TO_DEG;
	titha3 = -1.0 * asin(ZJ3_1 / L1);
	titha3 = titha3 * RAD_TO_DEG;
}


void jacobianoInverso(double Vxaux, double Vyaux, double Vzaux, double Pxaux, double Pyaux, double Pzaux) {
	/*   -----------------------------------------------------------------------
	 * 	 Funcion que devuelve la velocidad angular de cada motor
	 *	 Entrada: Velocidad en (x,y,z) y Posicion en (x,y,z)
	 *	 Salida:  Velocidad angular en rad/s
	 	 -----------------------------------------------------------------------*/

	double Vaux[3] = { Vxaux, Vyaux, Vzaux };

	E1[0] = Pxaux + 0;
	E1[1] = Pyaux - 0.07;
	E1[2] = Pzaux;
	E2[0] = Pxaux + 0.060621778264911;
	E2[1] = Pyaux + 0.035;
	E2[2] = Pzaux;
	E3[0] = Pxaux - 0.060621778264911;
	E3[1] = Pyaux + 0.035;
	E3[2] = Pzaux;

	l1[0][0] = XJ1_1 - F1[0];//l11=[(XJ1(1)-F1(1,1));(YJ1(1)-F1(1,2));(ZJ1(1)-F1(1,3))];
	l1[0][1] = YJ1_1 - F1[1];
	l1[0][2] = ZJ1_1 - F1[2];
	l1[1][0] = XJ2_1 - F2[0];//l12=[(XJ2(1)-F2(1,1));(YJ2(1)-F2(1,2));(ZJ2(1)-F2(1,3))];
	l1[1][1] = YJ2_1 - F2[1];
	l1[1][2] = ZJ2_1 - F2[2];
	l1[2][0] = XJ3_1 - F3[0];//l13=[(XJ3(1)-F3(1,1));(YJ3(1)-F3(1,2));(ZJ3(1)-F3(1,3))];
	l1[2][1] = YJ3_1 - F3[1];
	l1[2][2] = ZJ3_1 - F3[2];

	l2[0][0] = E1[0] - XJ1_1;//l21=[(E1(1,1)-XJ1(1));(E1(1,2)-YJ1(1));(E1(1,3)-ZJ1(1))];
	l2[1][0] = E1[1] - YJ1_1;
	l2[2][0] = E1[2] - ZJ1_1;
	l2[0][1] = E2[0] - XJ2_1;//l22=[(E2(1,1)-XJ2(1));(E2(1,2)-YJ2(1));(E2(1,3)-ZJ2(1))];
	l2[1][1] = E2[1] - YJ2_1;
	l2[2][1] = E2[2] - ZJ2_1;
	l2[0][2] = E3[0] - XJ3_1;//l23=[(E3(1,1)-XJ3(1));(E3(1,2)-YJ3(1));(E3(1,3)-ZJ3(1))];
	l2[1][2] = E3[1] - YJ3_1;
	l2[2][2] = E3[2] - ZJ3_1;

	Jcd = transpuesta(l2);				//Jacobiano de la cinematica directa

	cross_P1[0] = SA1[1] * l1[0][2] - SA1[2] * l1[0][1];//Producto Cruz entre vectores
	cross_P1[1] = SA1[2] * l1[0][0] - SA1[0] * l1[0][2];
	cross_P1[2] = SA1[0] * l1[0][1] - SA1[1] * l1[0][0];

	cross_P2[0] = SA2[1] * l1[1][2] - SA2[2] * l1[1][1];//vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1]
	cross_P2[1] = SA2[2] * l1[1][0] - SA2[0] * l1[1][2];//vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2]
	cross_P2[2] = SA2[0] * l1[1][1] - SA2[1] * l1[1][0];//vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0]

	cross_P3[0] = SA3[1] * l1[2][2] - SA3[2] * l1[2][1];
	cross_P3[1] = SA3[2] * l1[2][0] - SA3[0] * l1[2][2];
	cross_P3[2] = SA3[0] * l1[2][1] - SA3[1] * l1[2][0];

	dot[0] = productDot(cross_P1, Jcd.m[0]);			//Producto Punto
	dot[1] = productDot(cross_P2, Jcd.m[1]);
	dot[2] = productDot(cross_P3, Jcd.m[2]);

	for (int i = 0; i < 3; ++i) {			//Jacobiano de la cinematica inversa
		for (int j = 0; j < 3; ++j) {
			if (i == j) {
				Jci[i][j] = dot[i];
			} else {
				Jci[i][j] = 0;
			}
		}
	}

	Jci_inv = inversa(Jci);
	Jinv = productMatriz(Jcd.m,Jci_inv.m);	//Jacobiano Inverso

	for (int i = 0; i < 3; ++i) {
		double sum = 0;
		for (int j = 0; j < 3; ++j) {
			sum += sum + Jinv.m[i][j] * Vaux[j];
		}
		omega[i] = sum;
	}
	omega1 = omega[0];
	omega2 = omega[1];
	omega3 = omega[2];

}

void SetPerfilTimers(double omeg1, double omeg2, double omeg3) {	// velAng en rpm
	/*   -----------------------------------------------------------------------
	 *   Funcion que configura el PWM del timer 12,13 14, para que el motor vaya a la
	 *   velocidad deseada.
	 *   Entrada: velocidad Angular de cada eslabon, en rad/s.
	 ----------------------------------------------------------------------- */

		rpm1 = (omeg1*((60.00)/(2.00*pi)));
		rpm2 = (omeg2*((60.00)/(2.00*pi)));
		rpm3 = (omeg3*((60.00)/(2.00*pi)));
		if(rpm1<=0.1){
			rpm1=0.1;
		}
		if(rpm2<=0.1){
			rpm2=0.1;
		}
		if(rpm3<=0.1){
			rpm3=0.1;
		}


		periodoM[0] = (((FCL * 60.00) / ((double)rpm1 * ((double)(TIM12->PSC) + 1.00) * 9600.00)) - 1.00);	//Fpwm = 64M / ((ARR+1)*(PSC+1)
		periodoM[1] = (((FCL * 60.00) / ((double)rpm2 * ((double)(TIM13->PSC) + 1.00) * 9600.00)) - 1.00);
		periodoM[2] = (((FCL * 60.00) / ((double)rpm3 * ((double)(TIM14->PSC) + 1.00) * 9600.00)) - 1.00);


		// Calculo el error por casteo a int, y cuando supero la unidad, lo compenzo --------------
		for (int i = 0; i < 3; ++i) {
			ErrorPeriodo[i] = periodoM[i] - (double) ((int32_t) periodoM[i]);
			ErrorAcumuladoPeriodo[i] = ErrorAcumuladoPeriodo[i] + ErrorPeriodo[i];
			if (ErrorAcumuladoPeriodo[i] > 1) {
				periodoM[i] = periodoM[i] + 1;
				ErrorAcumuladoPeriodo[i] = ErrorAcumuladoPeriodo[i] - 1;
			}
			if (periodoM[i] < 2) {
				periodoM[i] = 0; // velocidad lineal de 10.000mm/s !!!
			}

			else if (periodoM[i] > pow(2, 16)) { 		// desborde de timer 32 bits
				periodoM[i] = pow(2, 16);
			}
		}

			periodoM[0]=(uint32_t)(((FCL * 60.0) / (rpm1 * ((double)(TIM12->PSC) + 1.0) * 9600.0)) - 1.0);
			periodoM[1]=(uint32_t)(((FCL * 60.0) / (rpm2 * ((double)(TIM13->PSC) + 1.0) * 9600.0)) - 1.0);
			periodoM[2]=(uint32_t)(((FCL * 60.0) / (rpm3 * ((double)(TIM14->PSC) + 1.0) * 9600.0)) - 1.0);


			if (TIM12->CNT > periodoM[0]) {
						TIM12->CNT = periodoM[0] - 1;// Reinicio clock solo si hace falta y a un valor cercano a la interrupcion, para que no haga ese paso de nuevo
			}
			if (TIM13->CNT > periodoM[1]) {
						TIM13->CNT = periodoM[1] - 1;// Reinicio clock solo si hace falta y a un valor cercano a la interrupcion, para que no haga ese paso de nuevo
			}
			if (TIM14->CNT > periodoM[2]) {
						TIM14->CNT = periodoM[2] - 1;// Reinicio clock solo si hace falta y a un valor cercano a la interrupcion, para que no haga ese paso de nuevo
			}

			TIM12->ARR =periodoM[0];
			TIM12->CCR1 = (uint32_t)((double)(TIM12->ARR) / 2.0);
			TIM13->ARR =periodoM[1];
			TIM13->CCR1 = (uint32_t)((double)(TIM13->ARR) / 2.0);
			TIM14->ARR =periodoM[2];
			TIM14->CCR1 = (uint32_t)((double)(TIM14->ARR) / 2.0);
		//------------------------------------------------------------------------------------------

		//TIM12->ARR = periodoM1;
		//TIM12->CCR1 = (TIM12->ARR) / 2;							//Duty Cycle %50
		//TIM13->ARR = periodoM2;
		//TIM13->CCR1 = (TIM13->ARR) / 2;
		//TIM14->ARR = periodoM3;
		//TIM14->CCR1 = (TIM14->ARR) / 2;

		/*if (TIM12->CNT > Periodo[0]) {
			TIM12->CNT = Periodo[0] - 1;// Reinicio clock solo si hace falta y a un valor cercano a la interrupcion, para que no haga ese paso de nuevo
		}
		if (TIM13->CNT > Periodo[1]) {
			TIM13->CNT = Periodo[1] - 1;// Reinicio clock solo si hace falta y a un valor cercano a la interrupcion, para que no haga ese paso de nuevo
		}
		if (TIM14->CNT > Periodo[2]) {
			TIM14->CNT = Periodo[2] - 1;// Reinicio clock solo si hace falta y a un valor cercano a la interrupcion, para que no haga ese paso de nuevo
		}*/
}

