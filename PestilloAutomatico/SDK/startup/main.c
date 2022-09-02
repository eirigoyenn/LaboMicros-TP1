/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   Nicolás Magliola
 ******************************************************************************/

#include "hardware.h"
#include "fsm.h"


void App_Init (void);
void App_Run (void);


int main (void)
{
    hw_Init();
    hw_DisableInterrupts();
    timerInit();
    lectora_init();
    DRV_Init_Display();
    DRV_Init_LEDs();
    DRV_Init_Switch();
    DRV_FRDM_Init_LEDs();
    hw_EnableInterrupts();

    eSystemState eNextState = wait_State;
	user list[4] = {{.id = {1,0,0,0,0,0,0,0}, .password = {1,0,0,0,NAN}},
		{.id = {2,0,0,0,0,0,0,0}, .password = {2,0,0,0,0}},
		//TARJETA ITBA ique
		{.card = {';', '4','5','1','7','6','6','0','1','3','8','9','6','5','7','8','4',
				'=','2','2','0','4','1','2','1','2','3','6','0','0','4','2','1','7','7',
				'1','9','5','?','>'}, .id = {3,8,9,6,5,7,8,4}, .password = {2,1,0,0,0}},

		//SUBE joaco
		{.card = {';', '6','0','6','1','2','6','8','2','6','4','8','5','8','2','2','2',
				'=','2','5','0','6','1','0','1','0','0','0','0','0','?','2','0','0','0',
				'0','0','0','0','0'}, .id = {4,8,9,6,5,7,8,4}, .password = {0,1,0,0,0}}};

	loadUsers(list, 4);
	printDisplay();
	for (;;)
	{
		FSM_Run(&eNextState); /* Program-specific loop  */
	}
}


