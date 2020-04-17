/*
 * 	startup.c
 *
 */
__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}

/* ---- GPIO registers ---- */
#define GPIO_D			0x40020C00
#define GPIO_D_IDR		((volatile unsigned short*)	(GPIO_D+0x10))	// The entire IDR
#define GPIO_D_IDR_HIGH	((volatile unsigned char*)	(GPIO_D+0x11))	// The HIGH values un the IDR
#define GPIO_D_ODR		((volatile unsigned short*)	(GPIO_D+0x14))	// The entire ODR
#define GPIO_D_ODR_LOW	((volatile unsigned char*)	(GPIO_D+0x14))	// The LOW values in the ODR, D0-7
#define GPIO_D_ODR_HIGH	((volatile unsigned char*)	(GPIO_D+0x15))	// The HIGH values in the ODR, D8-15
#define GPIO_D_MODER	((volatile unsigned int*)	GPIO_D)			// I/O config
#define GPIO_D_OTYPER	((volatile unsigned char*)	(GPIO_D+5))		// Push-pull/open-drain config
#define GPIO_D_PUPDR	((volatile unsigned short*)	(GPIO_D+0x0E))	// Pull-up/pull-down config

/* ---- Global variables ---- */
unsigned char KEYLIST[16] = {1, 2, 3, 0xA, 4, 5, 6, 0xB, 7, 8, 9, 0xC, 0xE, 0, 0xF, 0xD};					   // Keypad key values
unsigned char HEXLIST[16] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71}; // Segment values for 0-F (hex)

/* ---- Init ---- */
void appInit(void)
{
	*GPIO_D_MODER 		= 0x55005555;	// Pin D15-12 outports, D11-0 inports
	*GPIO_D_OTYPER 		= 0x00;			// Push-pull on outpins
	*GPIO_D_PUPDR 		= 0x00AA;		// Pull-down on inpins
	*GPIO_D_ODR_HIGH	= 0;			// Reset outpins to 0 at init
}

/* ---- Helper functions ---- */
/*
 * Returns a segment code based on the given key value.
 */
unsigned char out7seg(unsigned char kv)
{
	return HEXLIST[kv];
}

/*
 *	Activates the given row, r, by editing the ODR.
 */
void kbActivateRow(unsigned char row){
	switch(row){
		case 1: *GPIO_D_ODR_HIGH = 0x10; break;
		case 2: *GPIO_D_ODR_HIGH = 0x20; break;
		case 3: *GPIO_D_ODR_HIGH = 0x40; break;
		case 4: *GPIO_D_ODR_HIGH = 0x80; break;
		default: *GPIO_D_ODR_HIGH = 0;
	}
}

/*
 * Returns which column is activated.
 */
unsigned char kbGetCol(void){
	unsigned char col = *GPIO_D_IDR_HIGH;
	if(col & 8) return 4;
	if(col & 4) return 3;
	if(col & 2) return 2;
	if(col & 1) return 1;
	return 0;
}

/*
 * Activate each row, one at a time,
 * 	read each column,
 * 	return the key value.
 */
unsigned char keyB(void){
	unsigned char keyValue = 0xFF; // Default value if no key is pressed
	
		for(unsigned char row = 1; row <= 4; row++){
			kbActivateRow(row);
			unsigned char col = kbGetCol();
			
			if(col){
				kbActivateRow(0); // Turn off all rows once a key has been pressed
				keyValue = KEYLIST[(4 * (row - 1)) + (col - 1)];
			}
		}
		
	return keyValue;
}

/* ---- Main ---- */
void main(void)
{
	unsigned char dispValue;
	appInit();
	while(1){
		dispValue = out7seg(keyB());	// Read keypad D8-15
		*GPIO_D_ODR_LOW = dispValue;	// Update display D0-7
	}
}

