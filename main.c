/*
 * PokemonBattle.c
 *
 * Created: 5/19/2019 8:23:45 PM
 * Author : Edward-Laptop
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "bit.h"
#include "lcd.h"
#include "nokia5110.h"

#include "definitions.h"
#include "pokemon.h"
#include "move.h"

unsigned char EEMEM eeprom_array[10];

// NokiaLCD at scale 1 has 14 columns, 7 rows
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // A input (A4..A0 buttons and A7..A5 SNES buttons)
	DDRB = 0xFF; PORTB = 0x00; // B output (B1B0 = LCD and B6..B2 = Nokia)
	DDRD = 0xFF; PORTD = 0x00; // D output (LCD)
	
	LCD_init();
	NokiaLCD_Init();
	
	Pokemon pokemon1 = Pokemon_Charmander();
	Pokemon pokemon2 = Pokemon_Charmander();
	
	//unsigned char a = eeprom_read_word(&eeprom_array[0]);
	
	NokiaLCD_SetScale(1);
	NokiaLCD_WriteString(pokemon1.name);
	NokiaLCD_WriteString(pokemon2.move1.name);
	NokiaLCD_Render();
	
	LCD_ClearScreen();
	LCD_DisplayString(1, "*MOVE1  *MOVE2  ");
	LCD_Cursor(9);
	
    while (1) 
    {
		
    }
}

