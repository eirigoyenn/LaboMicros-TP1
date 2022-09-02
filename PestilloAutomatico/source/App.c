/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <lectora.h>
#include "board.h"
#include "gpio.h"
#include <stdio.h>  /*borrar despues*/
//#include "hardware.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


char arreglo[40]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	lectora_init();
	char hola = 0;
	hola |=1<<2;
	printf("hola1 = %d ",hola);
	hola |=1<<3;
	printf("hola2 = %d ",hola);

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{

	int stat = readLectoraStatus();
    if(stat==VALUEOK_ || stat == ERRORi_){
    	readLectoraCardInfo(arreglo);
    	printf("entra con el status: %d \n" , stat);
    	printf("%d - %d - %d - %d - %d - %d - %d - %d - %d - %d \n",arreglo[0],arreglo[1],arreglo[2],arreglo[3],arreglo[4],arreglo[5],arreglo[6],arreglo[7],arreglo[8],arreglo[9]);
    	printf("%d - %d - %d - %d - %d - %d - %d - %d - %d - %d \n",arreglo[10],arreglo[11],arreglo[12],arreglo[13],arreglo[14],arreglo[15],arreglo[16],arreglo[17],arreglo[18],arreglo[19]);
    	printf("%d - %d - %d - %d - %d - %d - %d - %d - %d - %d \n",arreglo[20],arreglo[21],arreglo[22],arreglo[23],arreglo[24],arreglo[25],arreglo[26],arreglo[27],arreglo[28],arreglo[29]);
    	printf("%d - %d - %d - %d - %d - %d - %d - %d - %d - %d -%d \n",arreglo[30],arreglo[31],arreglo[32],arreglo[33],arreglo[34],arreglo[35],arreglo[36],arreglo[37],arreglo[38],arreglo[39],arreglo[40]);
    }


}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/






/*******************************************************************************
 ******************************************************************************/



