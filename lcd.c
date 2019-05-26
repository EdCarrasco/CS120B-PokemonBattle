/*
 * lcd.c
 *
 * Created: 5/19/2019 8:28:03 PM
 *  Author: Edward-Laptop
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcd.h"

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

/*-------------------------------------------------------------------------*/

#define DATA_BUS PORTD		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTB	// port connected to pins 4 and 6 of LCD disp.
#define RS 0			// pin number of uC connected to pin 4 of LCD disp.
#define E 1			// pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/

void LCD_ClearScreen(void) {
	LCD_WriteCommand(0x01);
}

void LCD_Init(void) {
	delay_ms(100);
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x0f);
	LCD_WriteCommand(0x01);
	delay_ms(10);
}

void LCD_WriteCommand (unsigned char command) {
	CLR_BIT(CONTROL_BUS,RS);
	DATA_BUS = command;
	SET_BIT(CONTROL_BUS,E);
	asm("nop");
	CLR_BIT(CONTROL_BUS,E);
	delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteData(unsigned char data) {
	SET_BIT(CONTROL_BUS,RS);
	DATA_BUS = data;
	SET_BIT(CONTROL_BUS,E);
	asm("nop");
	CLR_BIT(CONTROL_BUS,E);
	delay_ms(1);
}

void LCD_DisplayString( unsigned char column, /*const unsigned*/ char* string) {
	unsigned char c = column;
	while(*string) {
		LCD_Cursor(c++);
		LCD_WriteData(*string++);
	}
}

void LCD_Cursor(unsigned char column) {
	if ( column < 17 ) { // 16x1 LCD: column < 9
		// 16x2 LCD: column < 17
		LCD_WriteCommand(0x80 + column - 1);
	} else {
		LCD_WriteCommand(0xB8 + column - 9);	// 16x1 LCD: column - 1
		// 16x2 LCD: column - 9
	}
}

void LCD_WriteCustomData(unsigned char column, unsigned char *data_array) {
	int i;
	LCD_WriteCommand(0x40 + (column*8)); // Command 0x40 for GRAM
	for (i = 0; i < 8; i++) {
		LCD_WriteData(data_array[i]);
	}
	LCD_WriteCommand(0x80);
}

void LCD_WriteCustomString(unsigned char column, unsigned char **data_2Darray) {
	unsigned char col = column;
	while (**data_2Darray) {
		LCD_Cursor(col++);
		LCD_WriteCustomData(col, *data_2Darray++);
	}
}

void LCD_DisplayMenu4(char *item1, char *item2, char *item3, char *item4) {
	LCD_ClearScreen();
	LCD_Cursor(1);
	LCD_DisplayString(1, item1);
	LCD_DisplayString(9, item2);
	LCD_DisplayString(17, item3);
	LCD_DisplayString(25, item4);
}

void LCD_DisplayMenu2(char *item1, char *item2) {
	LCD_ClearScreen();
	LCD_Cursor(1);
	LCD_DisplayString(1, item1);
	LCD_DisplayString(9, item2);
}

void delay_ms(int miliSec) //for 8 Mhz crystal
{
	int i, j;
	for (i=0; i < miliSec; i++) {
		for (j = 0; j < 775; j++) {
    		asm("nop");
		}
	}
}