/*
 * 	startup.c
 *
 */
#include "header.h"

__attribute__((naked)) __attribute__((section (".start_section")) )
void startup ( void )
{
__asm__ volatile(" LDR R0,=0x2001C000\n");		/* set stack */
__asm__ volatile(" MOV SP,R0\n");
__asm__ volatile(" BL main\n");					/* call main */
__asm__ volatile(".L1: B .L1\n");				/* never return */
}


void main(void){
	ascii_init();
	
	char *s;
	char test1[] = "Alfanumerisk";
	char test2[] = "Display - test";
	
	ascii_gotoxy(1,1); // Write to first row
	s = test1;
	while(*s)
		ascii_write_char(*s++);
		
	ascii_gotoxy(1,2); // Write to second row
	s = test2;
	while(*s)
		ascii_write_char(*s++);
}