# Robot paralelo tipo DELTA de tres grados de libertad rotacionales. 

En este proyecto se desarrolla el firmware que controla un robot paralelo tipo DELTA de tres grados de libertad rotacionales. El mismo se ha desarrollado en un microcontrolador de la marca STM modelo: NUCLEO SMT32H743ZI.



**Periféricos utilizados:**

UART3: Corresponde a la uart vinculada con ST-Link de la placa y que permite la comunicacion con una PC para el evidio y recepcion de datos. (asociado a una interrupcion).

Timers 2,3,4 &rarr; Los timers 2,3,4 estan configurados en modo Input Capture Direct Mode. Perimiten leer la entrada de pulsos que recibe el driver (Asociados a interrupciones internas).

Timers 12,13,14 &rarr; Los timers 12,13,14 estan configurados como PWM Generation. Los mismos se encargan de generar señales PWM para el control de cada uno de los motores.

Timer 5 &rarr; El timer 5 se utiliza para contar el tiempo (en segundos) con el objetivo de poder actualizar la curva de velocidad instante a instante.

Timer 15 &rarr; El timer 15 es el encargado de actualizar la curva de velocidad cada 10 ms (esta asociado a una interrupcion interna que actualiza el tiempo).


**Interrupciones externas:** 

En cuanto a las interrupciones debidas a eventos externos al microcontroladores, hemos definido unas 6 interrupciones para los finales de carrara y una interrupcion adicional para el boton de usuario (boton azul de la placa)



**Interpretacion de comandos:**

Por el momento el firmware admite los siguientes comandos:

**:h/:H** &rarr; Se efectua un homing, es decir, que el robot falla a una posicion de referencia (En nuestro caso los eslabones superiores se posicionaran a cero grados)

**:p/:P** &rarr; Recepcion de coordenadas en el espacio cartesiano correspondiente a la posicion que se desea alcanzar. La trama tiene la siguiente forma. Ej: :px0 y0 z-0.6

**:r/:R** &rarr; Permite realizar un reset del micro gracias a una llamada al sistema.

**:d/:D** &rarr; Con este comando ingresamos el estado de demostracion, que leerá un archivo previamente guardado en el árbol del proyecto y efectuará un movimiento continuo recorriendo todo los puntos contenido en dicho archivo.






