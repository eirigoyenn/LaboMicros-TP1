/***************************************************************************//**
  @file     drv_switch.c
  @brief    Drivers del switch
  @author   Dimas Bosch
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include "drv_switch.h"
#include "gpio.h"
#include "timer.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define MAX_EVENTS 10


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
enum{
	PIN_RCHA =PORTNUM2PIN(PB, 9),
	PIN_RCHB = PORTNUM2PIN(PA, 1),
	PIN_RSWITCH = PORTNUM2PIN(PB, 23)
};

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void IRQ_RchA(void);
static void IRQ_RSwitch(void);
static void makeTurn(void);
static void btnPressed(void);
static void doublePressTimeout(void);
/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static int* event_stack;

static int B_val;
//static int switch_val;
static tim_id_t press_count_timer;
static tim_id_t double_press_timer;
static tim_id_t anti_multi_turn_timer;
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void DRV_Init_Switch(int* event_stack_param)
{
	gpioMode(PIN_RCHA, INPUT);		//No le pongo pullup porque el encoder ya lo tiene
	gpioMode(PIN_RCHB, INPUT);
	gpioMode(PIN_RSWITCH, INPUT);

	gpioIRQ(PIN_RCHA, GPIO_IRQ_MODE_BOTH_EDGES, IRQ_RchA);
	gpioIRQ(PIN_RSWITCH, GPIO_IRQ_MODE_FALLING_EDGE, IRQ_RSwitch);

	press_count_timer = timerGetId();
	double_press_timer = timerGetId();
	anti_multi_turn_timer = timerGetId();

	event_stack=event_stack_param;
}

encoder_event_t getLastEvent(void)
{
	return *event_stack;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

static void IRQ_RchA(void)
{
	if(!timerExpired(anti_multi_turn_timer))
	{
		B_val = gpioRead(PIN_RCHB);
		timerStart(anti_multi_turn_timer, TIMER_MS_2_TICKS(50), TIM_MODE_SINGLESHOT, makeTurn);
	}
}

static void makeTurn(void)
{
	if(B_val)
	{
		*event_stack = ENC_TURN_CLOCK;
	}
	else *event_stack = ENC_TURN_ANTI_CLOCK;
}
 
static void IRQ_RSwitch(void)
{
	if(!timerExpired(press_count_timer))
	{
		printf("detected\n");
		timerStart(press_count_timer, TIMER_MS_2_TICKS(50), TIM_MODE_PERIODIC, btnPressed);
	}
}

static void btnPressed(void)
{
	static uint32_t counter = 0;
	counter++;
	if(gpioRead(PIN_RSWITCH))
	{
		timerStop(press_count_timer);
		if(counter<=10)
		{

			if(!timerExpired(double_press_timer))
			{
				timerStop(double_press_timer);
				printf("double press\n");
				*event_stack = ENC_DOUBLE_PRESS;
			}
			else
				timerStart(double_press_timer, TIMER_MS_2_TICKS(200), TIM_MODE_SINGLESHOT, doublePressTimeout);
		}
		else
		{
			printf("long press\n");
			*event_stack = ENC_LONG_PRESS;
		}
		counter = 0;
	}
}

static void doublePressTimeout(void)
{
	printf("smol press\n");
	*event_stack = ENC_SMALL_PRESS;
}
