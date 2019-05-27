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
#include <assert.h>

#include "bit.h"
#include "lcd.h"
#include "nokia5110.h"
#include "snescontroller.h"

#include "definitions.h"
#include "pokemon.h"
#include "move.h"
//#include "sprites.h"

#include "tasks.h"
#include "timer.h"

//unsigned char EEMEM eeprom_array[10];

// NokiaLCD at scale 1 has 14 columns, 7 rows
int main(void)
{
	DDRA = 0x06;// PORTA = 0xFF;//PORTA = 0xFF; // A input (A4..A0 buttons and A7..A5 SNES buttons)
	DDRB = 0xFF; PORTB = 0x00; // B output (B1B0 = LCD and B6..B2 = Nokia)
	DDRD = 0xFF; PORTD = 0x00; // D output (LCD)
	
	//unsigned char a = eeprom_read_word(&eeprom_array[0]);

	LCD_Init();
	LCD_ClearScreen();
	NokiaLCD_Init();
	NokiaLCD_SetScale(1);
	TimerSet(tasksPeriod);
	TimerOn();
	
	//Pokemon pokemon1 = Pokemon_Squirtle();
	//Pokemon pokemon2 = Pokemon_Charmander();
	/*
	NokiaLCD_WriteString("Bulbasaur");
	uint8_t enemy_xoffset = 84-1-25;
	uint8_t enemy_yoffset = 0;
	NokiaLCD_CustomBitmap(enemy_charmander_bits, enemy_xoffset, enemy_yoffset, 0);
	NokiaLCD_SetCursor(0,8);
	NokiaLCD_WriteString("HP");
	NokiaLCD_HealthBar(12, 10, 50);

	uint8_t player_xoffset = 0;
	uint8_t player_yoffset = 48-1-24;
	NokiaLCD_CustomBitmap(enemy_bulbasaur_bits, player_xoffset, player_yoffset, 1);
	*/
	
	//NokiaLCD_WriteChar(0x7f);
	//NokiaLCD_WriteString("   POKeMON    ");
	//NokiaLCD_WriteString("   Battle     ");
	//NokiaLCD_WriteChar(0x81);
	//NokiaLCD_WriteString(pokemon2.move1.name);
	
	
	//setScene(SCENE_MAIN_MENU);
	//LCD_Cursor(1);
	
	/*LCD_DisplayMenu4("Catch", "Gym", "Pokemon", "Setting");
	menuLength = 4;
	setMenuIndex(0);
	
	NokiaLCD_Clear();
	NokiaLCD_WriteString("Main menu");
	NokiaLCD_Render();*/
	
	unsigned char i = 0;
	tasks[i].state = SNES_RIGHT_Released;
	tasks[i].period = SNES_Button_Period;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFunction = &SNES_RIGHT_Tick;
	i++;
	tasks[i].state = SNES_LEFT_Released;
	tasks[i].period = SNES_Button_Period;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFunction = &SNES_LEFT_Tick;
	i++;
	tasks[i].state = SNES_B_Released;
	tasks[i].period = SNES_Button_Period;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFunction = &SNES_B_Tick;
	i++;
	tasks[i].state = SNES_X_Released;
	tasks[i].period = SNES_Button_Period;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFunction = &SNES_X_Tick;
	i++;
	tasks[i].state = SCENE_Start;
	tasks[i].period = scenePeriod;
	tasks[i].elapsedTime = tasks[i].period;
	tasks[i].TickFunction = &Scene_Tick;
	i++;
	
    while (1) 
    {
    }
}

