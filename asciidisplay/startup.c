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

/* ---- Main ---- */
void main(void){
	ascii_init();
	ascii_write_char('f');
	ascii_gotoxy(1,2);
	ascii_write_char('f');
	ascii_gotoxy(2,1);
	ascii_write_char('g');
}

