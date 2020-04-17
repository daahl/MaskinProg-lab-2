/*
 * 	startup.c
 *
 */
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void ){
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

/* ---- Registers ---- */
#define GPIO_E 			0x40021000
#define GPIO_E_MODER	((volatile unsigned int*) GPIO_E)			// I/O config
#define GPIO_E_ODR  	((volatile unsigned short*)	(GPIO_E+0x14))	// The entire ODR

#define STK			 0xE000E010
#define STK_CTRL	 ((volatile unsigned int*) STK)			// (De)activation and status of SysTick
#define STK_CTRL_END ((volatile unsigned char*) (STK + 2))	// (De)activation and status of SysTick
#define STK_LOAD	 ((volatile unsigned int*) (STK + 4))	// Start value
#define STK_VAL		 ((volatile unsigned int*) (STK + 8))	// Current value

/* ---- Global variables ---- */

/* ---- Helper functions ---- */
void delay_250ns(void){
	*STK_CTRL = 0;			// Reset SysTick
	*STK_LOAD = 0x00000029; // Set load value, 42 Hz = 250 ns, 42 (dec) = 29 (hex)
	*STK_VAL = 0;			// Reset counter register
	*STK_CTRL = 5;			// Set SysTick and enable counter
	
	// Run until counter == 0
	while(!*STK_CTRL_END){
		break;
	}
}

void delay_micro(unsigned int us){
	for(unsigned int i = 0; i <= us; i++){
		delay_250ns();
		delay_250ns();
		delay_250ns();
		delay_250ns();
	}
}

void delay_milli(unsigned int ms){
	#ifdef SIMULATOR
		ms = ms/1000;
		ms++;
	#endif
	
	for(unsigned int i = 0; i < ms; i++)
		delay_micro(1000);
		
}

/* ---- Init ---- */
void appInit(void){
	*GPIO_E_MODER = 0x00005555; // Port E0-7 as out
}

/* ---- Main ---- */
void main(void){
	appInit();
	unsigned char *Bargraph = GPIO_E_ODR;
	unsigned int i = 0;
	
	while(1){
		*Bargraph = 0;
		delay_milli(2);
		*Bargraph = 0xFF;
		delay_milli(2);
	}
}

