/*
 * cinematica_inversa.c
 *
 *  Created on: 12 sep. 2020
 *      Author: Santiago Rivier
 *      Updated by: Elias Correa y Eliseo Elorga
 */

#include <GlobalFunc.h>
#include "trayectoria.h"


bool flagInv = false;
float alima;
float alimd;
float vlim;
float T,Ta,Td,Tv,Tj1,Tj2,Tj,delta;
float qi,qf;

void get_Straj(float t){

    if (qf < qi){

    flagInv = true;

    qi = -qi;
    qf = -qf;
    vi = -vi;
    vf = -vf;

    vmax = -vmin;
    vmin = -vmax;
    amax = -amin;
    amin = -amax;
    jmax = -jmin;
    jmin = -jmax;

    alima = jmax*Tj1;
    alimd = -jmax*Tj2;
    vlim = vi+(Ta-Tj1)*alima;

    }else{
        jmin = -jmax;
        amin = -amax;
        vmin = -vmax;

        alima = jmax*Tj1;
        alimd = -jmax*Tj2;
        vlim = vi+(Ta-Tj1)*alima;

    }


    //#Acceleration phase

    if (t>=0 && t<=Tj1){               //a) [0,Tj1]
        //printf("tramo1\n");
        q = qi+vi*t+jmax*pow(t,3)/6;
        qd = vi+jmax*pow(t,2)/2;
        qdd = jmax*t;
        qddd = jmax;
        if (flagInv){
            q=-q; qd=-qd; qdd=-qdd; qddd=-qddd;
            flagInv = false;
        }

    } else if (t>Tj1 && t<=Ta-Tj1){   //b) [Tj1,Ta-Tj1]
        //printf("tramo2\n");
        q = qi+vi*t+(alima/6)*(3*pow(t,2)-3*Tj1*t+pow(Tj1,2));
        qd = vi+amax*(t-Tj1/2);
        qdd = jmax*Tj1;
        qddd = 0;

        if (flagInv){
            q=-q; qd=-qd; qdd=-qdd; qddd=-qddd;
            flagInv = false;
        }

    } else if (t>Ta-Tj1 && t<=Ta){    //c) [Ta-Tj1,Ta]
        //printf("tramo3\n");
        q = qi+(vlim+vi)*Ta/2-vlim*(Ta-t)-jmin*pow(Ta-t,3)/6;
        qd = vmax+jmin*pow(Ta-t,2)/2;
        qdd = -jmin*(Ta-t);
        qddd = jmin;
        if (flagInv){
            q=-q; qd=-qd; qdd=-qdd; qddd=-qddd;
            flagInv = false;
        }
    }

    /*-------------Constant phase ----------------*/
    else if (t>Ta && t<=Ta+Tv){
        //printf("tramo4\n");
        q = qi+(vlim+vi)*Ta/2+vlim*(t-Ta);
        qd = vmax;
        qdd = 0;
        qddd = 0;
        if (flagInv){
            q=-q; qd=-qd; qdd=-qdd; qddd=-qddd;
            flagInv = false;
        }
    }
    /*-------------Dese phase ----------------*/

    else if (t>=T-Td && t<=T-Td+Tj2){
        //printf("tramo5\n");
        q=qf-(vlim+vf)*Td/2+vlim*(t-T+Td)-jmax*(pow(t-T+Td,3)/6);
        qd=vlim-jmax*(pow(t-T+Td,2)/2);
        qdd=-jmax*(t-T+Td);
        qddd=jmin;
        if (flagInv){
            q=-q; qd=-qd; qdd=-qdd; qddd=-qddd;
            flagInv = false;
        }
    } else if (t>T-Td+Tj2 && t<=T-Tj2){
        //printf("tramo6\n");
        q=qf-(vlim+vf)*Td/2+vlim*(t-T+Td)+(alimd/6)*(3*pow(t-T+Td,2)-3*Tj2*(t-T+Td)+pow(Tj2,2));
        qd=vlim+alimd*(t-T+Td-Tj2/2);
        qdd = -jmax*Tj2;
        qddd = 0;
        if (flagInv){
            q=-q; qd=-qd; qdd=-qdd; qddd=-qddd;
            flagInv = false;
        }

    } else if (t>T-Tj2 && t<=T){
        //printf("tramo7\n");
        q = qf-vf*(T-t)-jmax*(pow(T-t,3)/6);
        qd = vf+jmax*(pow(T-t,2))/2;
        qdd = -jmax*(T-t);
        qddd = jmax;
        if (flagInv){
            //printf("inv tramo7\n");
            q=-q; qd=-qd; qdd=-qdd; qddd=-qddd;
            flagInv = false;
        }
    }


}


void update_ScurveTraj(float _qi ,float _qf, float vi,float vf ,float vmax,float amax,float jmax){

jmin = -jmax;
amin = -amax;
vmin = -vmax;
qi=qi;
qf=qf;

if (qf < qi){

    flagInv = true;

    qi = -qi;
    qf = -qf;
    vi = -vi;
    vf = -vf;

    vmax = -vmin;
    vmin = -vmax;
    amax = -amin;
    amin = -amax;
    jmax = -jmin;
    jmin = -jmax;

}

volatile float Tjaux = MIN(sqrt(fabs(vf-vi)/jmax),amax/jmax);


if (Tjaux<amax/jmax){
    //if (qf-qi > Tjaux*(vi+vf)) {printf("the trajectory is feasible \n");}
    //else {printf("the trajectory is NOT \n");}
}
else if (Tjaux == amax/jmax){
    //if (qf-qi > 0.5*(vi+vf)*(Tjaux+fabs(vi+vf)/amax)) {printf("the trajectory is feasible\n");}
    //else {printf("the trajectory is NOT feasible\n");}
}
//Phase 1: acceleration
if ((vmax-vi)*jmax < pow(amax,2)){
    //printf("amax is NOT reached\n");

    Tj1=sqrt(fabs(vmax-vi)/jmax);
    Ta=Tj1*2;
}
else{
    //printf("amax is reached\n");
    Tj1=amax/jmax;
    Ta=Tj1+(vmax-vi)/amax;
}

//Phase 3: Desacceleration


if ((vmax-vf)*jmax < pow(amax,2)){
    //printf("amin is NOT reached\n");

    Tj2=sqrt(fabs(vmax-vf)/jmax);
    Td=Tj2*2;
}
else{
    //printf("amin is reached\n");
    Tj2=amax/jmax;
    Td=Tj2+(vmax-vf)/amax;
}


Tv = (qf-qi)/vmax - Ta/2*(1+vi/vmax)-Td/2*(1+vf/vmax);

if (Tv>0){
    //printf("the max velocity is reached\n");
}
else{
    //printf("CASE 2\n");
    //printf("In this case vmax is NOT reached, so Tv=0\n");
    Tj1=amax/jmax;
    Tj2=Tj1;
    Tj=Tj1;
    delta = (pow(amax,4)/pow(jmax,2))+2*(pow(vi,2)+pow(vf,2))+amax*(4*(qf-qi)-2*(amax/jmax)*(vi+vf));
    Ta=((pow(amax,2)/jmax)-2*vi+sqrt(delta))/(2*amax);
    Td=((pow(amax,2)/jmax)-2*vf+sqrt(delta))/(2*amax);
    Tv=0;


    if (Ta<2*Tj || Td<2*Tj){
        //printf("entre\n");
        while (!(Ta>2*Tj && Td>2*Tj)){

            amax=amax*0.99;
            Tj=amax/jmax;
            delta = (pow(amax,4)/pow(jmax,2))+2*(pow(vi,2)+pow(vf,2))+amax*(4*(qf-qi)-2*(amax/jmax)*(vi+vf));
            Ta=((pow(amax,2)/jmax)-2*vi+sqrt(delta))/(2*amax);
            Td=((pow(amax,2)/jmax)-2*vf+sqrt(delta))/(2*amax);

            //print(f'{i}',amax)


            if (Ta<0){
                Ta=0;
                Tj1=0;
                Td=2*((qf-qi)/(vf+vi));
                Tj2=(jmax*(qf-qi)-sqrt(jmax*(jmax*(pow(qf-qi,2))+pow(vf+vi,2)*(vf-vi))))/(jmax*(vf+vi));
                break;
            }
            if (Td<0){
                Td=0;
                Ta=2*((qf-qi)/(vf+vi));
                Tj1=(jmax*(qf-qi)-sqrt(jmax*(jmax*(pow(qf-qi,2))-pow(vf+vi,2)*(vf-vi))))/(jmax*(vf+vi));
                Tj2=0;
                break;
            }
        }
    }

}

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


		periodoM[0] = (((FCL * 60.00) / ((double)rpm1 * ((double)(TIM12->PSC) + 1.00) * STEPREV)) - 1.00);	//Fpwm = 64M / ((ARR+1)*(PSC+1)
		periodoM[1] = (((FCL * 60.00) / ((double)rpm2 * ((double)(TIM13->PSC) + 1.00) * STEPREV)) - 1.00);
		periodoM[2] = (((FCL * 60.00) / ((double)rpm3 * ((double)(TIM14->PSC) + 1.00) * STEPREV)) - 1.00);


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

			periodoM[0]=(uint32_t)(((FCL * 60.0) / (rpm1 * ((double)(TIM12->PSC) + 1.0) * STEPREV)) - 1.0);
			periodoM[1]=(uint32_t)(((FCL * 60.0) / (rpm2 * ((double)(TIM13->PSC) + 1.0) * STEPREV)) - 1.0);
			periodoM[2]=(uint32_t)(((FCL * 60.0) / (rpm3 * ((double)(TIM14->PSC) + 1.0) * STEPREV)) - 1.0);


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

