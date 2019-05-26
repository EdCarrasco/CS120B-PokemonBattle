/*
 * statemachines.h
 *
 * Created: 5/20/2019 4:31:30 AM
 *  Author: Edward-Laptop
 */ 


#ifndef TASKS_H_
#define TASKS_H_

#include "sprites.h"

#define TASKS_SIZE	5
#define LCD_MENU_SELECTOR '*'

unsigned long scenePeriod = 200; // ms
unsigned long buttonPeriod = 100; // ms
unsigned long tasksPeriod = 100; // ms
unsigned long SNES_Button_Period = 100; // ms

typedef struct _Task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFunction)(int);
} Task;

Task tasks[TASKS_SIZE];

enum SNES_LEFT_States {SNES_LEFT_Released, SNES_LEFT_Pressed} SNES_LEFT_State;
enum SNES_RIGHT_States {SNES_RIGHT_Released, SNES_RIGHT_Pressed} SNES_RIGHT_State;
enum Scene_States {SCENE_Start, SCENE_MainMenu, SCENE_Battle, SCENE_Pokemon, SCENE_Settings,
	SCENE_BattleMoves	
} sceneState;
enum MENU_ITEMS {MI_TopLeft, MI_TopRight, MI_BotLeft, MI_BotRight};

int8_t menuIndex = MI_TopLeft;
int8_t menuLength = 4;
uint8_t sceneIndex = SCENE_MainMenu;
uint8_t pressedB = 0; // TODO: Combine into one variable or queue
uint8_t pressedX = 0;

Pokemon playerPokemon;
Pokemon enemyPokemon;

void clearMenuSelector() {
	uint8_t i;
	for (i = 0; i < menuLength; i++) {
		uint8_t cursor = (i * 8) + 1;
		LCD_Cursor(cursor);
		LCD_WriteData(' ');
	}
}

void setMenuIndex(int8_t index) {
	if (index < 0) index = menuLength - 1;
	menuIndex = index % menuLength;
	uint8_t cursor = (menuIndex * 8) + 1;
	//clearMenuSelector();
	LCD_Cursor(cursor);
	//LCD_WriteData(LCD_MENU_SELECTOR);
	//LCD_Cursor(cursor);
}

void hoverMenuItem() {
	switch (sceneIndex) {
		case SCENE_MainMenu:
			switch (menuIndex) {
				case MI_TopLeft:
					NokiaLCD_SetLine(4, 1);
					NokiaLCD_WriteString("Find Pokemon  in the tall   grass! ");
					break;
				case MI_TopRight:
					NokiaLCD_SetLine(4, 1);
					NokiaLCD_WriteString("Battle other  trainers!            ");
					break;
				case MI_BotLeft:
					NokiaLCD_SetLine(4, 1);
					NokiaLCD_WriteString("View your     Pokemon!             ");
					break;
				case MI_BotRight:
					NokiaLCD_SetLine(4, 1);
					NokiaLCD_WriteString("Set difficultyand other     things!");
					break;
			}
			break;
		case SCENE_Battle:
			// no changes in NokiaLCD from menu items
			break;
		case SCENE_BattleMoves:
			NokiaLCD_Clear();
			NokiaLCD_WriteString(playerPokemon.moves[menuIndex].name);
			NokiaLCD_SetLine(2, 0);
			NokiaLCD_WriteString(playerPokemon.moves[menuIndex].description);
			break;
		case SCENE_Pokemon:
			// TODO
			break;
		case SCENE_Settings:
			// TODO
			break;
	}
}

void setMenu(uint8_t index) {
	LCD_ClearScreen();
	switch (index) {
		case SCENE_MainMenu:
			LCD_DisplayMenu4("Catch", "Gym", "Pokemon", "Setting");
			menuLength = 4;
			break;
		case SCENE_Battle:
			LCD_DisplayMenu4("Fight", "Pokedex", "Switch", "Escape");
			menuLength = 4;
			break;
		case SCENE_BattleMoves:
			LCD_DisplayMenu4(playerPokemon.moves[0].name, playerPokemon.moves[1].name, playerPokemon.moves[2].name, playerPokemon.moves[3].name);
			menuLength = 4;
			break;
		case SCENE_Pokemon:
			LCD_DisplayMenu2("Main", "Back");
			menuLength = 2;
			break;
		case SCENE_Settings:
			LCD_DisplayMenu2("Diff", "Sound");
			menuLength = 2;
			break;
	}
	setMenuIndex(0);
}

void setScene(uint8_t index) {
	sceneIndex = index;
	NokiaLCD_Clear();
	switch (index) {
		case SCENE_MainMenu:
			NokiaLCD_WriteString("  Main Menu   ");
			break;
		case SCENE_Battle:
			enemyPokemon = new_Pokemon(ID_CHARMANDER);
			playerPokemon = new_Pokemon(ID_BULBASAUR);
			
			NokiaLCD_SetCursor(0,0);
			NokiaLCD_WriteString(enemyPokemon.name);
			uint8_t enemy_xoffset = 84-1-25;
			uint8_t enemy_yoffset = 0;
			NokiaLCD_CustomBitmap(bitmaps[enemyPokemon.spriteFrontIndex], enemy_xoffset, enemy_yoffset, 0);
			NokiaLCD_SetCursor(0,8);
			NokiaLCD_WriteString("HP");
			NokiaLCD_HealthBar(12, 10, 50);
			
			NokiaLCD_SetCursor(31,8*4);
			NokiaLCD_WriteString(playerPokemon.name);
			uint8_t player_xoffset = 0;
			uint8_t player_yoffset = 48-1-24;
			NokiaLCD_CustomBitmap(bitmaps[playerPokemon.spriteBackIndex], player_xoffset, player_yoffset, 1);
			NokiaLCD_SetCursor(24+49, 8*5);
			NokiaLCD_WriteString("HP");
			NokiaLCD_HealthBar(32, 42, 50);
			
			break;
		case SCENE_BattleMoves: {
			
			break; 
		}
		case SCENE_Pokemon:
			NokiaLCD_WriteString("    Pokemon   ");
			break;
		case SCENE_Settings:
			NokiaLCD_WriteString("   Settings   ");
			break;
		default:
			break;
	}
	hoverMenuItem();
}

int Scene_Tick(int state) {
	switch (state) {
		case SCENE_Start:
			state = SCENE_MainMenu;
			setMenu(state);
			setScene(state);
			break;
		case SCENE_MainMenu:
			if (pressedB) {
				pressedB = 0;
				switch(menuIndex) {
					case MI_TopLeft:
						state = SCENE_Battle;
						setMenu(state);
						setScene(state);
						break;
					case MI_TopRight:
						state = SCENE_Battle;
						setMenu(state);
						setScene(state);
						break;
					case MI_BotLeft:
						state = SCENE_Pokemon;
						setMenu(state);
						setScene(state);
						break;
					case MI_BotRight:
						state = SCENE_Settings;
						setMenu(state);
						setScene(state);
						break;
				}
			} else {
				state = SCENE_MainMenu;
			}
			break;
		case SCENE_Battle:
			if (pressedB) {
				pressedB = 0;
				switch(menuIndex) {
					case MI_TopLeft:
						state = SCENE_BattleMoves;
						setMenu(state);
						setScene(state);
						break;
					case MI_TopRight: // Pokedex
						// TODO: Pokedex
						break;
					case MI_BotLeft: // Switch
						// TODO: Switch
						break;
					case MI_BotRight: // Escape
						state = SCENE_MainMenu;
						setMenu(state);
						setScene(state);
						break;
				}
			} else if (pressedX) {
				pressedX = 0;
				state = SCENE_MainMenu;
				setMenu(state);
				setScene(state);
			} else {
				state = SCENE_Battle;
			}
			break;
		case SCENE_BattleMoves:
			if (pressedB) {
				pressedB = 0;
				switch (menuIndex) {
					case MI_TopLeft:
						break;
					case MI_TopRight:
						break;
					case MI_BotLeft:
						break;
					case MI_BotRight:
						break;
				}
			} else if (pressedX) {
				pressedX = 0;
				state = SCENE_Battle;
				setMenu(state);
				setScene(state);
			}
			break;
		case SCENE_Pokemon:
			if (pressedB) {
				pressedB = 0;
				switch (menuIndex) {
					case MI_TopLeft:
						// TODO
						break;
					case MI_TopRight:
						// TODO
						break;
					case MI_BotLeft:
						// TODO
						break;
					case MI_BotRight:
						// TODO
						break;
				}
			}
			break;
		case SCENE_Settings:
			break;
		default:
			state = SCENE_Start;
			break;
	}
	switch (state) {
		case SCENE_Start:
			break;
	}
	return state;
};

enum SNES_B_States {SNES_B_Released, SNES_B_Pressed} SNES_B_State;
int SNES_B_Tick(int state) {
	switch(state) {
		case SNES_B_Released:
			if (SNES_B) {
				state = SNES_B_Pressed;
				pressedB = 1;
			} else {
				state = SNES_B_Released;
			}
			break;
		case SNES_B_Pressed:
			state = (SNES_B) ? SNES_B_Pressed : SNES_B_Released;
			break;
	}
	return state;
}

enum SNES_X_States {SNES_X_Released, SNES_X_Pressed} SNES_X_State;
int SNES_X_Tick(int state) {
	switch(state) {
		case SNES_X_Released:
			if (SNES_X) {
				state = SNES_X_Pressed;
				pressedX = 1;
			} else {
				state = SNES_X_Released;
			}
			break;
		case SNES_X_Pressed:
			state = (SNES_X) ? SNES_X_Pressed : SNES_X_Released;
			break;
	}
	return state;
}

int SNES_LEFT_Tick(int state) {
	switch(state) {
		case SNES_LEFT_Released:
			if (SNES_LEFT) {
				state = SNES_LEFT_Pressed;
				setMenuIndex(menuIndex-1);				
				hoverMenuItem();
			} else {
				state = SNES_LEFT_Released;
			}
			break;
		case SNES_LEFT_Pressed:
			state = (SNES_LEFT) ? SNES_LEFT_Pressed : SNES_LEFT_Released;
			break;
	}
	return state;
}


int SNES_RIGHT_Tick(int state) {
	switch(state) {
		case SNES_RIGHT_Released:
			if (SNES_RIGHT) {
				state = SNES_RIGHT_Pressed;
				setMenuIndex(menuIndex+1);
				hoverMenuItem();
			} else {
				state = SNES_RIGHT_Released;
			}
			break;
		case SNES_RIGHT_Pressed:
			state = (SNES_RIGHT) ? SNES_RIGHT_Pressed : SNES_RIGHT_Released;
			break;
	}
	return state;
};

#endif /* TASKS_H_ */