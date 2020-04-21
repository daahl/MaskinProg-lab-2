#include "header.h"
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
	#ifdef SIMULATOR
		us = us/1000;
		us++;
	#else
	for(unsigned int i = 0; i <= us; i++){
		delay_250ns();
		delay_250ns();
		delay_250ns();
		delay_250ns();
	}
	#endif
}

void delay_milli(unsigned int ms){
	#ifdef SIMULATOR
		ms = ms/1000;
		ms++;
	#else
	for(unsigned int i = 0; i < ms; i++)
		delay_micro(1000);
	#endif
}

// x: bits are 1 to set
void ascii_ctrl_bit_set(char x){
	char c;
	c = *GPIO_E_ODR_LOW;
	*GPIO_E_ODR_LOW = B_SELECT | x | c;
}

// x: bits are 1 to clear
void ascii_ctrl_bit_clear(char x){
	char c;
	c = *GPIO_E_ODR_LOW;
	c = c & ~x;
	*GPIO_E_ODR_LOW = B_SELECT | c;
}

// Write to command register
void ascii_write_cmd(unsigned char command){
	ascii_ctrl_bit_clear(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	
	ascii_write_controller(command);
}

// Write to data register
void ascii_write_data(unsigned char data){
	ascii_ctrl_bit_set(B_RS);
	ascii_ctrl_bit_clear(B_RW);
	
	ascii_write_controller(data);
}

unsigned char ascii_read_data(void){
	*GPIO_E_MODER = 0x00005555;	// Set port E_HIGH to in
	ascii_ctrl_bit_set(B_RS);
	ascii_ctrl_bit_set(B_RW);
	
	unsigned char rv;
	rv = ascii_read_controller();
	
	*GPIO_E_MODER = 0x55555555;	// Set port E_HIGH to out
	return rv;
}

unsigned char ascii_read_status(void){
	
	char c;
	
	*GPIO_E_MODER = 0x00005555;
	ascii_ctrl_bit_set(B_RW);
	ascii_ctrl_bit_clear(B_RS);
	c = ascii_read_controller();
	
	*GPIO_E_MODER = 0x55555555;
	return c;
}

void ascii_command(unsigned char command){
	// Wait until the display is ready
	while((ascii_read_status() & 0x80) == 0x80){}
	
	delay_micro(8);	// Latency time
	ascii_write_cmd(command);
	delay_milli(2); // Kommand time
}

void ascii_write_controller(char c){
	ascii_ctrl_bit_set(B_E);
	*GPIO_E_ODR_HIGH = c;
	ascii_ctrl_bit_clear(B_E);
	delay_250ns();
}

unsigned char ascii_read_controller(void){
	char c;
	ascii_ctrl_bit_set(B_E);
	
	delay_250ns();
	delay_250ns();
	
	c = *GPIO_E_IDR_HIGH;
	
	ascii_ctrl_bit_clear(B_E);
	return c;
}

void ascii_init(void){
	// Function set (0x00111000)
	ascii_command(0x38);
	// Display control (0x00001111)
	ascii_command(0xF);
	// Clear display
	ascii_command(1);
	// Entry mode set (0x00000100)
	ascii_command(0x2);
}

void ascii_write_char(char c){
	// Wait until the display is ready
	while((ascii_read_status() & 0x80) == 0x80){}

	delay_micro(8);
	ascii_write_data(c);
	delay_micro(43);
}


void ascii_gotoxy(int x, int y){
	unsigned char offset;
	
	offset = 0x40 * (y - 1) + (x - 1);
	ascii_command(0x80 | offset);
}

void test(void){
	char f = 0;
}