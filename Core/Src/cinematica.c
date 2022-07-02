
#include <cinematica.h>



//Una constante a diferencia de una macro ocupa un espacio(memoria) durante la ejecución del programa.
//Ya que son valores que nunca van a cambiar en el programa, es mejor definirlos como tal (constantes)

#define C30 0.866025403784439
#define S30 0.500000000000000
#define C150 -0.866025403784439
#define S150 0.500000000000000
#define pi 3.14159265359
#define RAD_TO_DEG 180.0 / pi


//Parametros constructivos del robot

#define fi1 270
#define fi2 30
#define fi3 150
#define L1 0.4
#define L2 0.6
#define R 0.168
#define r 0.070


double A1, B1, C1, XJ1_1, XJ1_2, YJ1_1, YJ1_2, ZJ1_1, ZJ1_2, A11, A12, A13, A14, A15;
double A2, B2, C2, XJ2_1, XJ2_2, YJ2_1, YJ2_2, ZJ2_1, ZJ2_2;
double A3, B3, C3, XJ3_1, XJ3_2, YJ3_1, YJ3_2, ZJ3_1, ZJ3_2;


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
