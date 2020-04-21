#ifndef HEADER_H
#define HEADER_H

/* ---- Registers ---- */
#define GPIO_E			0x40021000
#define GPIO_E_IDR		((volatile unsigned short*)	(GPIO_E+0x10))	// The entire IDR
#define GPIO_E_IDR_LOW	((volatile unsigned char*)	(GPIO_E+0x10))	// The LOW values un the IDR
#define GPIO_E_IDR_HIGH	((volatile unsigned char*)	(GPIO_E+0x11))	// The HIGH values un the IDR

#define GPIO_E_ODR		((volatile unsigned short*)	(GPIO_E+0x14))	// The entire ODR
#define GPIO_E_ODR_LOW	((volatile unsigned char*)	(GPIO_E+0x14))	// The LOW values in the ODR, E0-7
#define GPIO_E_ODR_HIGH	((volatile unsigned char*)	(GPIO_E+0x15))	// The HIGH values in the ODR, E8-15

#define GPIO_E_MODER	((volatile unsigned int*)	GPIO_E)			// I/O config
#define GPIO_E_OTYPER	((volatile unsigned char*)	(GPIO_E+5))		// Push-pull/open-drain config
#define GPIO_E_SPEEDR	((volatile unsigned int*)	(GPIO_E+0x08))	// Output speed
#define GPIO_E_PUPDR	((volatile unsigned short*)	(GPIO_E+0x0E))	// Pull-up/pull-down config

#define STK			 0xE000E010
#define STK_CTRL	 ((volatile unsigned int*) STK)			// (De)activation and status of SysTick
#define STK_CTRL_END ((volatile unsigned char*) (STK + 2))	// (De)activation and status of SysTick
#define STK_LOAD	 ((volatile unsigned int*) (STK + 4))	// Start value
#define STK_VAL		 ((volatile unsigned int*) (STK + 8))	// Current value

/* ---- Program macros ---- */
#define B_E				0x40	// Enable signal, bit 6
#define B_SELECT		4		// Select display, bit 2
#define B_RW			2		// read = 0, write = 1, bit 1
#define B_RS			1		// control = 0, data = 1, bit 0

#define SIMULATOR

/* ---- Helper functions ---- */
void delay_250ns(void);

void delay_micro(unsigned int us);

void delay_milli(unsigned int ms);

void ascii_ctrl_bit_set(char x);

void ascii_ctrl_bit_clear(char x);

void ascii_write_cmd(unsigned char command);

void ascii_write_data(unsigned char data);

unsigned char ascii_read_data(void);

unsigned char ascii_read_status(void);

void ascii_command(unsigned char command);

void ascii_write_controller(char c);

unsigned char ascii_read_controller(void);

void ascii_init(void);

void ascii_write_char(char c);

void test(void);

void ascii_gotoxy(int x, int y);

#endif