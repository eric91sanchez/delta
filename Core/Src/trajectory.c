/*
 * trajectory.c
 *
 *  Created on: Jun 22, 2022
 *      Authors: Elias Correa y Eliseo Elorga
 */

#include "GlobalFunc.h"
#include "trajectory.h"
#include <stdlib.h>

double period[3],accumulatedError[3],periodError[3];
bool flagInv = false;
double alima;
double alimd;
double vlim;
double T,Ta,Td,Tv,Tj1,Tj2,Tj,delta;
double qi,qf;


double get_Straj(double t,double _qi, double _qf ,double *params){

	Tj1 = params[0];
	Tj2 = params[1];
	Tj = params[2];
	Ta = params[3];
	Td = params[4];
	Tv = params[5];
	T = params[6];

	qi=_qi;
	qf=_qf;

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
        qd = vi+alima*(t-Tj1/2);
        qdd = jmax*Tj1;
        qddd = 0;

        if (flagInv){
            q=-q; qd=-qd; qdd=-qdd; qddd=-qddd;
            flagInv = false;
        }

    } else if (t>Ta-Tj1 && t<=Ta){    //c) [Ta-Tj1,Ta]
        //printf("tramo3\n");
        q = qi+(vlim+vi)*Ta/2-vlim*(Ta-t)-jmin*pow(Ta-t,3)/6;
        qd = vlim+jmin*pow(Ta-t,2)/2;
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
        qd = vlim;
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


    return qd;
}


void update_ScurveTraj(double _qi ,double _qf, double vi,double vf ,double vmax,double amax,double jmax, double *params){

	jmin = -jmax;
	amin = -amax;
	vmin = -vmax;
	qi=_qi;
	qf=_qf;

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


	Tv = (qf-qi)/vmax - (Ta/2)*(1+(vi/vmax))-(Td/2)*(1+(vf/vmax));

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

	*(params)=Tj1;
	*(params+1)=Tj2;
	*(params+2)=Tj;
	*(params+3)=Ta;
	*(params+4)=Td;
	*(params+5)=Tv;
	*(params+6)=T;

}


void setProfilTimer(void){

	motor1.rpm = motor1.omega * RADs_TO_RPM;
	motor2.rpm = motor2.omega * RADs_TO_RPM;
	motor3.rpm = motor3.omega * RADs_TO_RPM;


    period[0] = COUNTERPERIOD(motor1.rpm);
    period[1] = COUNTERPERIOD(motor2.rpm);
    period[3] = COUNTERPERIOD(motor3.rpm);

	// Calculo el error por casteo a int, y cuando supero la unidad, lo compenzo

	for (int i = 0; i < 3; i++) {
		periodError[i] = period[i] - (double)((int32_t) period[i]);
		accumulatedError[i] += periodError[i];
		if (accumulatedError[i] > 1) {
			period[i] += 1;
			accumulatedError[i] -=  1;
		}
	}

    //Escritura del registro ARR
    __HAL_TIM_SET_AUTORELOAD(&htim12,period[0]);
	__HAL_TIM_SET_AUTORELOAD(&htim13,period[1]);
	__HAL_TIM_SET_AUTORELOAD(&htim14,period[3]);

	TIM12->CCR1 = (uint32_t)((double)(TIM12->ARR) / 2.0);
	TIM13->CCR1 = (uint32_t)((double)(TIM13->ARR) / 2.0);
	TIM14->CCR1 = (uint32_t)((double)(TIM14->ARR) / 2.0);


}

