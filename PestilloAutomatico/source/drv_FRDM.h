/*
 * drv_FRDM.h
 *
 *  Created on: 2 sep. 2022
 *      Author: Salta
 */

#ifndef DRV_FRDM_H_
#define DRV_FRDM_H_

void DRV_FRDM_Init_LEDs(void);
void TurnOn_LED_FRDM_GREEN(void);
void TurnOn_LED_FRDM_BLUE(void);
void TurnOn_LED_FRDM_RED(void);
void TurnOff_LED_FRDM_GREEN(void);
void TurnOff_LED_FRDM_BLUE(void);
void TurnOff_LED_FRDM_RED(void);
void Toggle_LED_FRDM_RED(void);
void Toggle_LED_FRDM_GREED(void);
void Toggle_LED_FRDM_BLUE(void);
void turnOn_Secs_RED(int secs);
void turnOn_Secs_GREEN(int secs);
void turnOn_Secs_BLUE(int secs);

#endif /* DRV_FRDM_H_ */
