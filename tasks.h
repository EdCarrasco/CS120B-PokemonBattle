/*
 * statemachines.h
 *
 * Created: 5/20/2019 4:31:30 AM
 *  Author: Edward-Laptop
 */ 


#ifndef TASKS_H_
#define TASKS_H_

#define TASKS_SIZE	4
#define LCD_MENU_SELECTOR '*'

#define INPUT_UP (~PINA & 0x01)
#define INPUT_RIGHT ((~PINA & 0x02) >> 1)
#define INPUT_DOWN ((~PINA & 0x04) >> 2)
#define INPUT_LEFT ((~PINA & 0x08) >> 3)

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

enum ButtonUpStates {BTN_Up_Release, BTN_Up_Pressed} buttonUpState;
enum ButtonDownStates {BTN_Down_Release, BTN_Down_Pressed} buttonDownState;
enum ButtonRightStates {BTN_Right_Release, BTN_Right_Pressed} buttonRightState;
enum ButtonLeftStates {BTN_Left_Release, BTN_Left_Pressed} buttonLeftState;

unsigned char buttonUp = 0;
unsigned char buttonDown = 0;
unsigned char buttonRight = 0;
unsigned char buttonLeft = 0;

enum SCENES {SCENE_MAIN_MENU, SCENE_BATTLE, SCENE_POKEMON_LIST, SCENE_SETTINGS_MENU};
enum MAIN_MENU_ITEMS {MI_CATCH, MI_GYM, MI_POKEMON, MI_SETTINGS};

int8_t menuIndex = MI_CATCH;
int8_t menuLength = 4;
uint8_t sceneIndex = SCENE_MAIN_MENU;

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



/*void selectMenuItem() {
	switch(sceneIndex) {
		case SCENE_MAIN_MENU:
			switch(menuIndex) {
				case MI_CATCH:
					setScene(SCENE_BATTLE);
					break;
				case MI_GYM:
					setScene(SCENE_BATTLE);
					break;
				case MI_POKEMON:
					setScene(SCENE_POKEMON_LIST);
					break;
				case MI_SETTINGS:
					setScene(SCENE_SETTINGS_MENU);
					break;
			}
			break;
		case SCENE_BATTLE:
			break;
		case SCENE_POKEMON_LIST:
			break;
		case SCENE_SETTINGS_MENU:
			break;
	}
}*/

void hoverMenuItem() {
	switch (sceneIndex) {
		case SCENE_MAIN_MENU:
			switch (menuIndex) {
				case MI_CATCH:
					NokiaLCD_SetLine(3);
					NokiaLCD_WriteString("CATCH         ");
					NokiaLCD_SetLine(4);
					NokiaLCD_WriteString("Find Pokemon  in the tall   grass! ");
					break;
				case MI_GYM:
					NokiaLCD_SetLine(3);
					NokiaLCD_WriteString("GYM           ");
					NokiaLCD_SetLine(4);
					NokiaLCD_WriteString("Battle other  trainers!            ");
					break;
				case MI_POKEMON:
					NokiaLCD_SetLine(3);
					NokiaLCD_WriteString("POKEMON           ");
					NokiaLCD_SetLine(4);
					NokiaLCD_WriteString("View your     Pokemon!             ");
					break;
				case MI_SETTINGS:
					NokiaLCD_SetLine(3);
					NokiaLCD_WriteString("SETTINGS           ");
					NokiaLCD_SetLine(4);
					NokiaLCD_WriteString("Set difficultyand other     things!");
					break;
			}
			break;
		case SCENE_BATTLE:
			break;
		case SCENE_POKEMON_LIST:
			break;
		case SCENE_SETTINGS_MENU:
			break;
	}
}

void setScene(uint8_t index) {
	NokiaLCD_Clear();
	switch (index) {
		case SCENE_MAIN_MENU:
			NokiaLCD_WriteString("  Main Menu   ");
			LCD_DisplayMenu4("Catch", "Gym", "Pokemon", "Setting");
			menuLength = 4;
			break;
		case SCENE_BATTLE:
			NokiaLCD_WriteString("    Battle 2  ");
			LCD_DisplayMenu4("Fight", "View", "Swap", "Escape");
			menuLength = 4;
			break;
		case SCENE_POKEMON_LIST:
			NokiaLCD_WriteString("    Pokemon   ");
			LCD_DisplayMenu2("Main", "Back");
			menuLength = 2;
			break;
		case SCENE_SETTINGS_MENU:
			NokiaLCD_WriteString("   Settings   ");
			LCD_DisplayMenu2("Diff", "Sound");
			menuLength = 2;
			break;
		default:
			break;
	}
	setMenuIndex(0);
	hoverMenuItem();
}

uint8_t pressedB = 0;

enum Scene_States {SCENE_Start, SCENE_MainMenu, SCENE_Battle, SCENE_Pokemon, SCENE_Settings} sceneState;
int Scene_Tick(int state) {
	switch (state) {
		case SCENE_Start:
			state = SCENE_MainMenu;
			setScene(SCENE_MAIN_MENU);
			break;
		case SCENE_MainMenu:
			if (pressedB) {
				pressedB = 0;
				switch(menuIndex) {
					case MI_CATCH:
						state = SCENE_Battle;
						setScene(SCENE_BATTLE);
						break;
					case MI_GYM:
						state = SCENE_Battle;
						setScene(SCENE_BATTLE);
						break;
					case MI_POKEMON:
						state = SCENE_Pokemon;
						setScene(SCENE_POKEMON_LIST);
						break;
					case MI_SETTINGS:
						state = SCENE_Settings;
						setScene(SCENE_SETTINGS_MENU);
						break;
				}
			}
			break;
		case SCENE_Battle:
			break;
		case SCENE_Pokemon:
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

enum SNES_LEFT_States {SNES_LEFT_Released, SNES_LEFT_Pressed} SNES_LEFT_State;
int SNES_LEFT_Tick(int state) {
	switch(state) {
		case SNES_LEFT_Released:
			if (SNES_LEFT) {
				state = SNES_LEFT_Pressed;
				setMenuIndex(menuIndex-1);				
				hoverMenuItem();
//				NokiaLCD_Render();
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

enum SNES_RIGHT_States {SNES_RIGHT_Released, SNES_RIGHT_Pressed} SNES_RIGHT_State;
int SNES_RIGHT_Tick(int state) {
	switch(state) {
		case SNES_RIGHT_Released:
			if (SNES_RIGHT) {
				state = SNES_RIGHT_Pressed;
				setMenuIndex(menuIndex+1);
				hoverMenuItem();
//				NokiaLCD_Render();
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