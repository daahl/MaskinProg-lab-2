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

#define GPIO_D			0x40020C00
#define GPIO_D_ODR		((volatile unsigned short*)	(GPIO_D+0x14))	// The entire ODR
#define GPIO_D_ODR_LOW	((volatile unsigned char*)	(GPIO_D+0x14))	// The LOW values in the ODR, D0-7
#define GPIO_D_ODR_HIGH	((volatile unsigned char*)	(GPIO_D+0x15))	// The HIGH values in the ODR, D8-15
#define GPIO_D_MODER	((volatile unsigned int*)	GPIO_D)			// I/O config
#define GPIO_D_OTYPER	((volatile unsigned char*)	(GPIO_D+5))		// Push-pull/open-drain config
#define GPIO_D_PUPDR	((volatile unsigned short*)	(GPIO_D+0x0E))		// Pull-up/pull-down config

void appInit (void)
{
	*GPIO_D_MODER 		= 0x55000000;	// Port D15-12 inports, D11-8 outports
	*GPIO_D_OTYPER 		= 0x0F;			// Push-pull
	*GPIO_D_PUPDR 		= 0x0055;		// Pull-down on inports
}

unsigned char binToHex(unsigned char bin)
{
	unsigned char hexList[16] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x67,0x77,0x7C,0x39,0x5E,0x79,0x71};
	return hexList[bin];
}

void main(void)
{
	
	unsigned char c;
	appInit();
	while(1){
		*GPIO_D_ODR_HIGH = 0x40;
		c = *GPIO_D_ODR_HIGH;
		/*
		c = (unsigned char) *((unsigned short*) 0x40021010);
		c = binToHex(c);
		*GPIO_D_ODR_LOW = c;
		*/
		
	}
}

