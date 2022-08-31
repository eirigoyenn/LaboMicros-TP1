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

/*typedef struct{
	uint8_t A_Down : 1;
	uint8_t B_Down : 1;
	uint8_t A_Up : 1;
	uint8_t B_Up : 1;
	uint8_t unused : 4;
} turn_t;*/

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static void IRQ_RchA(void);
//static void IRQ_RchB(void);
static void IRQ_RSwitch(void);
static void makeTurn(void);
static void btnPressed(void);
/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
//static int (*event_stack[MAX_EVENTS]);
static int* event_stack;

static int B_val;
//static turn_t turning_seq;
static tim_id_t anti_bounce_timer;
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
	//gpioIRQ(PIN_RCHB, GPIO_IRQ_MODE_BOTH_EDGES, IRQ_RchB);
	gpioIRQ(PIN_RSWITCH, GPIO_IRQ_MODE_BOTH_EDGES, IRQ_RSwitch);

	anti_bounce_timer = timerGetId();
	anti_multi_turn_timer = timerGetId();

	//turning_seq.A_Down = 0;
	//turning_seq.B_Down = 0;
	//turning_seq.A_Up = 0;
	//turning_seq.B_Up = 0;
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

	/*if(turning_seq.A_Down==0)
	{
		turning_seq.A_Down = true;
	}
	else
	{
		turning_seq.A_Up = true;
	}

	if(turning_seq.A_Down && turning_seq.A_Up && turning_seq.B_Down && turning_seq.B_Up)
	{
		*event_stack=ENC_TURN_ANTI_CLOCK;//meto en stack turn_anti_clock (xq A fue ultimo)
		//pongo todo en cero
		turning_seq.A_Down=0;
		turning_seq.A_Up=0;
		turning_seq.B_Down=0;
		turning_seq.B_Up=0;
	}*/
}

/*static void IRQ_RchB(void)
{
	bool A_val = gpioRead(PIN_RCHA);
	//timerDelay(TIMER_MS_2_TICKS(10));
	if(A_val)
	{
		*event_stack = ENC_TURN_ANTI_CLOCK;
	}
	else *event_stack = ENC_TURN_CLOCK;
	//timerDelay(TIMER_MS_2_TICKS(0.5));
	if(turning_seq.B_Down==0)
	{
		turning_seq.B_Down = true;
	}
	else
	{
		turning_seq.B_Up = true;
	}

	if(turning_seq.A_Down && turning_seq.A_Up && turning_seq.B_Down && turning_seq.B_Up)
	{
		*event_stack=ENC_TURN_CLOCK;//meto en stack turn_clock (xq B fue ultimo)
		//pongo todo en cero
		turning_seq.A_Down=0;
		turning_seq.A_Up=0;
		turning_seq.B_Down=0;
		turning_seq.B_Up=0;
	}
}*/
 

static void IRQ_RSwitch(void)
{
	//static bool allow=true;
	if(!timerExpired(anti_bounce_timer))
	{
		timerStart(anti_bounce_timer, TIMER_MS_2_TICKS(50), TIM_MODE_SINGLESHOT, btnPressed);
	}

}

static void btnPressed(void)
{
	static int counter = 0;
	printf("a %d\n", counter++);

}
