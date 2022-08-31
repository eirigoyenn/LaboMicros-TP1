/***************************************************************************//**
  @file     drv_switch.h
  @brief    Drivers del switch
  @author   Dimas Bosch
 ******************************************************************************/

#ifndef _DRV_SWITCH_H_
#define _DRV_SWITCH_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdio.h>
#include <stdbool.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum{
	ENC_NO_EVENT,
	ENC_TURN_CLOCK,
	ENC_TURN_ANTI_CLOCK,
	ENC_PRESSED
}encoder_event_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief TODO: completar descripcion
 * @param param1 Descripcion parametro 1
 * @param param2 Descripcion parametro 2
 * @return Descripcion valor que devuelve
*/
void DRV_Init_Switch(int* array);

encoder_event_t getLastEvent(void);

// +ej: char lcd_goto (int fil, int col);+


/*******************************************************************************
 ******************************************************************************/

#endif // _TEMPLATE_H_
