/*
 * buffer.h
 *
 *  Created on: 1 sep. 2022
 *      Author: Salta
 */

#ifndef BUFFER_H_
#define BUFFER_H_


#include <stdbool.h>


//Funciones publicas
void clear(void);
bool enqueue(int dato);//encolo, añado elemento(int provisorio)
int dequeue(void); //libero un elemento(int provisorio)

#endif /* BUFFER_H_ */
