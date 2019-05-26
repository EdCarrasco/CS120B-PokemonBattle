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

enum Scene_States {SCENE_Start, SCENE_MainMenu, SCENE_Battle, SCENE_Pokemon, SCENE_Settings} sceneState;
enum MENU_ITEMS {MI_TopLeft, MI_TopRight, MI_BotLeft, MI_BotRight};

int8_t menuIndex = MI_TopLeft;
int8_t menuLength = 4;
uint8_t sceneIndex = SCENE_MainMenu;

Pokemon player[6];

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
					NokiaLCD_SetLine(3);
					NokiaLCD_WriteString("CATCH         ");
					NokiaLCD_SetLine(4);
					NokiaLCD_WriteString("Find Pokemon  in the tall   grass! ");
					break;
				case MI_TopRight:
					NokiaLCD_SetLine(3);
					NokiaLCD_WriteString("GYM           ");
					NokiaLCD_SetLine(4);
					NokiaLCD_WriteString("Battle other  trainers!            ");
					break;
				case MI_BotLeft:
					NokiaLCD_SetLine(3);
					NokiaLCD_WriteString("POKEMON           ");
					NokiaLCD_SetLine(4);
					NokiaLCD_WriteString("View your     Pokemon!             ");
					break;
				case MI_BotRight:
					NokiaLCD_SetLine(3);
					NokiaLCD_WriteString("SETTINGS           ");
					NokiaLCD_SetLine(4);
					NokiaLCD_WriteString("Set difficultyand other     things!");
					break;
			}
			break;
		case SCENE_Battle:
			// do nothing
			break;
		case SCENE_Pokemon:
			// TODO
			break;
		case SCENE_Settings:
			// TODO
			break;
	}
}

void setScene(uint8_t index) {
	sceneIndex = index;
	NokiaLCD_Clear();
	switch (index) {
		case SCENE_MainMenu:
			NokiaLCD_WriteString("  Main Menu   ");
			LCD_DisplayMenu4("Catch", "Gym", "Pokemon", "Setting");
			menuLength = 4;
			
			break;
		case SCENE_Battle:
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
			
			LCD_DisplayMenu4("Fight", "View", "Swap", "Escape");
			menuLength = 4;
			
			break;
		case SCENE_Pokemon:
			LCD_DisplayMenu2("Main", "Back");
			menuLength = 2;
			NokiaLCD_WriteString("    Pokemon   ");
			break;
		case SCENE_Settings:
			LCD_DisplayMenu2("Diff", "Sound");
			menuLength = 2;
			NokiaLCD_WriteString("   Settings   ");
			break;
		default:
			break;
	}
	setMenuIndex(0);
	hoverMenuItem();
}

uint8_t pressedB = 0;


int Scene_Tick(int state) {
	switch (state) {
		case SCENE_Start:
			state = SCENE_MainMenu;
			setScene(SCENE_MainMenu);
			break;
		case SCENE_MainMenu:
			if (pressedB) {
				pressedB = 0;
				switch(menuIndex) {
					case MI_TopLeft:
						state = SCENE_Battle;
						setScene(SCENE_Battle);
						break;
					case MI_TopRight:
						state = SCENE_Battle;
						setScene(SCENE_Battle);
						break;
					case MI_BotLeft:
						state = SCENE_Pokemon;
						setScene(SCENE_Pokemon);
						break;
					case MI_BotRight:
						state = SCENE_Settings;
						setScene(SCENE_Settings);
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
						break;
					case MI_TopRight:
						break;
					case MI_BotLeft:
						break;
					case MI_BotRight:
						state = SCENE_MainMenu;
						setScene(SCENE_MainMenu);
						break;
				}
			}
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