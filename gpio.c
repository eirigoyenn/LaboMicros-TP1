/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"
#include "MK64F12.h"
#include "Hardware.h"
#include <stdio.h>
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SET_BIT(number, bit)	(number) |= (1<<(bit))
#define CLR_BIT(number, bit)	(number) &= ~(1<<(bit))


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

typedef struct
{
	PORT_Type* port;
	int port_num;
	int pin_num;
	GPIO_Type* gpio;
}MyPortPin;



static void pinMode (PORT_Type* port, GPIO_Type* gpio, int port_num, int pin, uint8_t mode);
static void init_clk(int port);
static MyPortPin getPortPin(pin_t pin);
//Interrupciones (SACAR PA OTRO archvio)
#define PINS_POR_PUERTO 32
#define PUERTOS 5 //A ,B ,C ,D ,E
#define OFFSET_PORTA 32  //LARGO DEL PUERTO A
#define OFFSET_PORTB (1*OFFSET_PORTA)
#define OFFSET_PORTC (2*OFFSET_PORTA)
#define OFFSET_PORTD (3*OFFSET_PORTA)
#define OFFSET_PORTE (4*OFFSET_PORTA)
void IRQs_Handler(int port); //se despacha a cada ISR
int Search_ISF(int port);   //busca pin por pin cual es el que tiene el isf levantao
static punt_func_t CALLBACKS[(PINS_POR_PUERTO*PUERTOS)+1];//creo un arreglo de tamaño  pinesporpuerto*puerto depunteros a funciones(manejo interrupciones)
                                                      //Objetivo: de que todas las entradas tengan una potencial ISR
int convers(int tipo);   				//convierte de un typedef
#define PINS_POR_PUERTO 32
#define PUERTOS 5 //A ,B ,C ,D ,E
#define OFFSET_PORTA 32  //LARGO DEL PUERTO A
#define OFFSET_PORTB (1*OFFSET_PORTA)
#define OFFSET_PORTC (2*OFFSET_PORTA)
#define OFFSET_PORTD (3*OFFSET_PORTA)
#define OFFSET_PORTE (4*OFFSET_PORTA)


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/**
 * @brief Configures the specified pin to behave either as an input or an output
 * @param pin the pin whose mode you wish to set (according PORTNUM2PIN)
 * @param mode INPUT, OUTPUT, INPUT_PULLUP or INPUT_PULLDOWN.
 */
void gpioMode (pin_t pin, uint8_t mode){
	MyPortPin myport=getPortPin(pin);

	init_clk(myport.port_num);

	pinMode(myport.port, myport.gpio, myport.port_num, myport.pin_num, mode);

}

/**
 * @brief Configures how the pin reacts when an IRQ event ocurrs
 * @param pin the pin whose IRQ mode you wish to set (according PORTNUM2PIN)
 * @param irqMode disable, risingEdge, fallingEdge or bothEdges
 * @param irqFun function to call on pin event
 * @return Registration succeed
 */
bool gpioIRQ(pin_t pin, int tipo, punt_func_t callback){
	int port = PIN2PORT(pin);			//se obtiene el puerto y numero de pin
	int num = PIN2NUM(pin);
	static bool NVIC_A=false,NVIC_B=false,NVIC_C=false,NVIC_D=false,NVIC_E=false;
	int modo=convers(tipo); 	//funcion para pasar de un enum , a otro , otra capa de abstracción
	if(modo< GPIO_IRQ_CANT_MODES){
		switch (port) {	//No se controla que antes se haya activa el Nvic. Podemos hacerlo adentro o buscar agluna forma de evitar ese posble error
			case(PA):
				PORTA->PCR[num] |= PORT_PCR_IRQC(modo);
				CALLBACKS[num] = callback;
				if(NVIC_A==false){NVIC_EnableIRQ(PORTA_IRQn);NVIC_A=true;}       //habilito en Nvic , pa quee el PORTa pueda interrumpirbreak;	//le asigno una lugar en el arreglo de callback
				break;
			case(PB):PORTB->PCR[num] |= PORT_PCR_IRQC(modo);
				CALLBACKS[OFFSET_PORTB+num] = callback; 	//31-63
				if(NVIC_B==false){NVIC_EnableIRQ(PORTB_IRQn); NVIC_B=true;}
				break;	     //habilito en Nvic , pa quee el PORTa pueda interrumpir
			case(PC): PORTC->PCR[num] |= PORT_PCR_IRQC(modo);
				CALLBACKS[OFFSET_PORTC+ num] = callback; //63-95
				if(NVIC_C==false){NVIC_EnableIRQ(PORTC_IRQn);NVIC_C=true;}
				 break;      //habilito en Nvic , pa quee el PORTa pueda interrumpir
			case(PD):PORTD->PCR[num] |= PORT_PCR_IRQC(modo);
				CALLBACKS[OFFSET_PORTD + num] = callback; //95-127
				if(NVIC_D==false){NVIC_EnableIRQ(PORTD_IRQn);NVIC_D=true;}
				 break;      //habilito en Nvic , pa quee el PORTa pueda interrumpir /
			case(PE):PORTE->PCR[num] |= PORT_PCR_IRQC(modo);
				CALLBACKS[OFFSET_PORTE + num] = callback;  //127-159
				if(NVIC_E==false){NVIC_EnableIRQ(PORTE_IRQn); NVIC_E=true;}
				break;       //habilito en Nvic , pa quee el PORTa pueda interrumpir
			default:printf("No se pudo configurar correctamente irqc del pcr del pin seleccionado"); return false ;break;
		}
		return true;
	}
	return false;	//MAL DATO DE ENTRADA
}

	/*switch(irqMode)
	{
	case GPIO_IRQ_MODE_DISABLE:
		myport->port->PCR |= PORT_PCR_IRQC(0);
		break;
	case GPIO_IRQ_MODE_RISING_EDGE:
		break;
	case GPIO_IRQ_MODE_FALLING_EDGE:
		break;
	case GPIO_IRQ_MODE_BOTH_EDGES:
		break;
	case GPIO_IRQ_CANT_MODES:
		break;
	}*/


/**
 * @brief Write a HIGH or a LOW value to a digital pin
 * @param pin the pin to write (according PORTNUM2PIN)
 * @param val Desired value (HIGH or LOW)
 */
void gpioWrite (pin_t pin, bool value)
{
	MyPortPin myport=getPortPin(pin);
	if(value)
	{
		myport.gpio->PSOR=(1<<myport.pin_num);
	}
	else
	{
		myport.gpio->PCOR=(1<<myport.pin_num);
	}
}

/**
 * @brief Toggle the value of a digital pin (HIGH<->LOW)
 * @param pin the pin to toggle (according PORTNUM2PIN)
 */
void gpioToggle (pin_t pin)
{
	MyPortPin myport=getPortPin(pin);
	myport.gpio->PTOR = (1<<myport.pin_num);
}

/**
 * @brief Reads the value from a specified digital pin, either HIGH or LOW.
 * @param pin the pin to read (according PORTNUM2PIN)
 * @return HIGH or LOW
 */
bool gpioRead (pin_t pin)
{
	MyPortPin myport=getPortPin(pin);
	bool value = myport.gpio->PDIR & (1<<myport.pin_num);
	return value;
}



/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static void pinMode(PORT_Type* port, GPIO_Type* gpio, int port_num, int pin, uint8_t mode)
{
	//Por las dudas, pongo todo el PCR en cero
	port->PCR[pin] = 0x00000000u;

	//Seteo como GPIO
	port->PCR[pin] |= PORT_PCR_MUX(1);
	//CLR_BIT(port->PCR[pin], 10);
	//CLR_BIT(port->PCR[pin], 9);
	//SET_BIT(port->PCR[pin], 8);

	switch(mode)
	{
		case INPUT:
			CLR_BIT(gpio->PDDR, pin);
			break;
		case OUTPUT:
			SET_BIT(gpio->PDDR, pin);
			break;
		case INPUT_PULLUP:
			CLR_BIT(gpio->PDDR, pin);
			SET_BIT(port->PCR[pin], 1);
			SET_BIT(port->PCR[pin], 0);
			break;
		case INPUT_PULLDOWN:
			CLR_BIT(gpio->PDDR, pin);
			SET_BIT(port->PCR[pin], 1);
			CLR_BIT(port->PCR[pin], 0);
			break;
	}

}

static void init_clk(int port)
{
	SET_BIT(SIM->SCGC5, 9+port);	//Clock Gating
}

static MyPortPin getPortPin(pin_t pin)
{
	MyPortPin result;
	result.port_num = PIN2PORT(pin);
	result.pin_num = PIN2NUM(pin);

	uint32_t offset_port=PORTB_BASE-PORTA_BASE;
	uint32_t offset_gpio=GPIOB_BASE-GPIOA_BASE;

	result.port = (PORT_Type*) (PORTA_BASE + (offset_port*result.port_num));
	result.gpio = (GPIO_Type*) (GPIOA_BASE + (offset_gpio*result.port_num));

	return result;
}
/*Funciones de INTERRUPCIONES,sacar pa otro archivo*/
int convers(int tipo){
    int cont=0;
    for(int i=0; cont<3; i++){			//se elige 3 por la forma de IRQC ver pag 289 del reference manual
        if(i==tipo){return tipo; }
        cont+=1;
    }
    for (;cont<9;cont++){		 //se elige 3 por la forma de IRQC ver pag 289 del reference manual
        if(cont==tipo){return cont+4;}
    }
    return 15;//error
}

void IRQs_Handler(int num_pin) {
	(*CALLBACKS[num_pin])();
}
int Search_ISF(int port){			//TRATAR DE HACERLO CON BIT-BAND ALIAS (clase 4 interrupcones 2.17.)
	//Tengo que protegerla de alguna forma , pa que no me salte un interrupcion en este proceso.
	uint32_t ISFR_Port;
	uint32_t MASK=1; //MASCARA DE 1.
	int num_pin=0;
	int salida=(PINS_POR_PUERTO*PUERTOS);
	switch(port){
		case(PA): ISFR_Port=PORTA->ISFR;//SALVO EL REGISTRO
			for( num_pin=0; (num_pin<32)&&(salida==PINS_POR_PUERTO*PUERTOS);){
				if((ISFR_Port & MASK)==1) {salida=num_pin;}	//la mascara busca anular el resto de los valores y encontrar el primero
				else{num_pin+=1;
					ISFR_Port=ISFR_Port>>1;}}break;		//shifteo 1
		case(PB):ISFR_Port=PORTB->ISFR;
			for(num_pin=0; (num_pin<32)&&(salida==PINS_POR_PUERTO*PUERTOS);){
				if((ISFR_Port & MASK)==1) {salida=num_pin;}
				else{num_pin+=1;
					ISFR_Port=ISFR_Port>>1;}}break;
		case(PC):ISFR_Port=PORTC->ISFR;
			for(num_pin=0; (num_pin<32)&&(salida==PINS_POR_PUERTO*PUERTOS);){
				if((ISFR_Port & MASK)==1) {salida=num_pin;}
				else{num_pin+=1;
					ISFR_Port=ISFR_Port>>1;}}break;
		case(PD):ISFR_Port=PORTD->ISFR;
			for(num_pin=0; (num_pin<32)&&(salida==PINS_POR_PUERTO*PUERTOS);){
				if((ISFR_Port & MASK)==1) {salida=num_pin;}
				else{num_pin+=1;
					ISFR_Port=ISFR_Port>>1;}}break;
		case(PE):ISFR_Port=PORTE->ISFR;
			for(num_pin=0; (num_pin<32)&&(salida==PINS_POR_PUERTO*PUERTOS);){
				if((ISFR_Port & MASK)==1) {salida=num_pin;}
				else{num_pin+=1;
					ISFR_Port=ISFR_Port>>1;}}break;
		default: return salida;break;
	}
	return salida;
}

/*Funciones que se llaman de manera automatica*/
__ISR__ PORTA_IRQHandler(void) {
	int num_pin=Search_ISF(PA);
	PORTA->PCR[num_pin]|=PORT_PCR_ISF_MASK;  //Se baja el flag de ISF
	IRQs_Handler(num_pin); }
__ISR__ PORTB_IRQHandler(void) {
	int num_pin=Search_ISF(PB);
	PORTB->PCR[num_pin]|=PORT_PCR_ISF_MASK;
	IRQs_Handler(num_pin+OFFSET_PORTB); } //offset del portB
__ISR__ PORTC_IRQHandler(void) {
	int num_pin=Search_ISF(PC);
	PORTC->PCR[num_pin]|=PORT_PCR_ISF_MASK;
	IRQs_Handler(num_pin+OFFSET_PORTC); }
__ISR__ PORTD_IRQHandler(void) {
	int num_pin=Search_ISF(PD);
	PORTD->PCR[num_pin]|=PORT_PCR_ISF_MASK;
	IRQs_Handler(num_pin+OFFSET_PORTD); }
__ISR__ PORTE_IRQHandler(void) {
	int num_pin=Search_ISF(PE);
	PORTE->PCR[num_pin]|=PORT_PCR_ISF_MASK;
	IRQs_Handler(num_pin+OFFSET_PORTE); }


/*__ISR__ SysTick_Handler(void){

}*/


/*******************************************************************************
 ******************************************************************************/
