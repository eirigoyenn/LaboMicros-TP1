/*
 * buffer.c
 *
 *  Created on: 1 sep. 2022
 *      Author: Salta
 */
#include <buffer.h>
#include <stdio.h>//borrar, lo use pa testear
#define MAXSIZE 2

typedef struct {
	//int maxSize;	//Tamaño maximo de la cola
	int front;      //Posicion del elemento front(se desencola)
	int rear;		//posicion del elemento rear(se encola)
	int full;		//si esta lleno ,chilla
	int buffer[MAXSIZE];
}buffer_t;
buffer_t BUFFER;

//funciones privadas
void check_rear(void);
void check_front(void);

//funciones privadas
void check_rear(void) {//cambiar dependiendo el dato
	if (BUFFER.rear == MAXSIZE) {
		BUFFER.rear = 0;
	}
}
void check_front(void){
	if (BUFFER.front == MAXSIZE) {
		BUFFER.front = 0;
	}
}

//Funciones Publicas
void clear(void) {
	BUFFER.full = 0;
	BUFFER.front = 0;
	BUFFER.rear = 0;
}

bool enqueue(int dato) {
	bool salida;
	if ((BUFFER.full==0)||(BUFFER.full % MAXSIZE)) { //siempre que full no sea 100 osea este lleno, es xq hay espacio disponible
		check_rear();
		printf("posicion rear:%d\n",BUFFER.rear);
		BUFFER.buffer[BUFFER.rear] = dato;
		BUFFER.rear++;
		BUFFER.full++;
		salida=true;
	}
	else {		//si dio cero es xq no espacio pa agregar sin pisar.
		salida=false;
	}
	return salida;
}

int dequeue(void) {
	printf("posicion rear:%d\n",BUFFER.rear);
	check_front();
	int salida = BUFFER.buffer[BUFFER.front];
	BUFFER.front++;
	BUFFER.full--;
	return salida;
}
