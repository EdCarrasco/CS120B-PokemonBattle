/*
 * statemachines.h
 *
 * Created: 5/20/2019 4:31:30 AM
 *  Author: Edward-Laptop
 */ 


#ifndef TASKS_H_
#define TASKS_H_

#define TASKS_SIZE	5
#define LCD_MENU_SELECTOR '*'

#define INPUT_UP (~PINA & 0x01)
#define INPUT_RIGHT ((~PINA & 0x02) >> 1)
#define INPUT_DOWN ((~PINA & 0x04) >> 2)
#define INPUT_LEFT ((~PINA & 0x08) >> 3)

unsigned long scenePeriod = 250; // ms
unsigned long buttonPeriod = 100; // ms
unsigned long tasksPeriod = 50; // ms

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

int ButtonUpTick(int state) {
	switch(state) {
		case BTN_Up_Release:
			if (INPUT_UP) {
				state = BTN_Up_Pressed;
				buttonUp = 1;
				buttonDown = 0;
			} else {
				state = BTN_Up_Release;
			}
			break;
		case BTN_Up_Pressed:
			if (!INPUT_UP) {
				state = BTN_Up_Release;
				buttonUp = 0;
			} else {
				state = BTN_Up_Pressed;
			}
			break;
	}
	return state;
};

int ButtonDownTick(int state) {
    switch(state) {
        case BTN_Down_Release:
			if (INPUT_DOWN) {
				state = BTN_Down_Pressed;
				buttonDown = 1;
				buttonUp = 0;
			} else {
				state = BTN_Down_Release;
			}
			break;
        case BTN_Down_Pressed:
			if (!INPUT_DOWN) {
				state = BTN_Down_Release;
				buttonDown = 0;
			} else {
				state = BTN_Down_Pressed;
			}
			break;
    }
    return state;
};

int ButtonRightTick(int state) {
    switch(state) {
        case BTN_Right_Release:
			if (INPUT_RIGHT) {
				state = BTN_Right_Pressed;
				buttonRight = 1;
				buttonLeft = 0;
			} else {
				state = BTN_Right_Release;
			}
			break;
        case BTN_Right_Pressed:
			if (!INPUT_RIGHT) {
				state = BTN_Right_Release;
				buttonRight = 0;
			} else {
				state = BTN_Right_Pressed;
			}
			break;
    }
    return state;
};

int ButtonLeftTick(int state) {
    switch(state) {
        case BTN_Left_Release:
			if (INPUT_LEFT) {
				state = BTN_Left_Pressed;
				buttonLeft = 1;
				buttonRight = 0;
			} else {
				state = BTN_Left_Release;
			}
			break;
        case BTN_Left_Pressed:
			if (!INPUT_LEFT) {
				state = BTN_Left_Release;
				buttonLeft = 0;
			} else {
				state = BTN_Left_Pressed;
			}
			break;
    }
    return state;
};

enum SceneStates {
	SCENE_Start,
	SCENE_MainMenu,
	SCENE_Battle,
	SCENE_BattleResult,
} sceneState;

void test(unsigned short snes, uint8_t bit, uint8_t cursor) {
	if (GetBit(snes, bit)) {
		LCD_Cursor(cursor);
		LCD_WriteData('1');
	} else {
		LCD_Cursor(cursor);
		LCD_WriteData('0');
	}
}

int SceneTick(int state) {
	//unsigned char customA[8] = {0x1f, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x11, 0x00};
	//unsigned char customB[8] = {0x1f, 0x11, 0x11, 0x1f, 0x11, 0x11, 0x1f, 0x00};
	static unsigned char cursor = 1;
	
	unsigned short snes = SNES_ReadController();
	//uint8_t i;
	if (SNES_ButtonB(snes)) {
		LCD_Cursor(cursor++);
		LCD_WriteData('B');
	}
	if (SNES_ButtonY(snes)) {
		LCD_Cursor(cursor++);
		LCD_WriteData('Y');
	}
	if (SNES_ButtonX(snes)) {
		LCD_Cursor(cursor++);
		LCD_WriteData('X');
	}
	if (SNES_ButtonA(snes)) {
		LCD_Cursor(cursor++);
		LCD_WriteData('A');
	}
	
	switch (state) {
		case SCENE_Start:
			state = SCENE_MainMenu;
			LCD_ClearScreen();
			break;
		case SCENE_MainMenu:
			state = SCENE_MainMenu;
			//LCD_DisplayString(1,  " Start  ");
			//LCD_DisplayString(9,  " Setting");
			//LCD_DisplayString(17, " Pokemon");
			//LCD_DisplayString(25, " Exit   ");
			/*if (buttonUp) {
				buttonUp = 0;
				cursor = (cursor <= 16) ? cursor : cursor - 16;
			} else if (buttonDown) {
				buttonDown = 0;
				cursor = (cursor <= 16) ? cursor + 16 : cursor;
			} else if (buttonRight) {
				buttonRight = 0;
				cursor = (cursor%16 <= 8) ? cursor + 8 : cursor;
			} else if (buttonLeft) {
				buttonLeft = 0;
				cursor = (cursor%16 <= 8) ? cursor : cursor - 8;
			}
			LCD_Cursor(cursor);
			LCD_WriteData(LCD_MENU_SELECTOR);
			LCD_Cursor(cursor);
			*/
			break;
		case SCENE_Battle:
			if (buttonUp) {
    			buttonUp = 0;
    			state = SCENE_BattleResult;
				LCD_ClearScreen();
				LCD_DisplayString(1, "Battle result");
    		} else {
    			state = SCENE_Battle;
			}
			break;
		case SCENE_BattleResult:
			if (buttonUp) {
    			buttonUp = 0;
    			state = SCENE_MainMenu;
				LCD_ClearScreen();
				LCD_DisplayString(1, "Main menu");
    		} else {
    			state = SCENE_BattleResult;
			}
			break;
		default:
			state = SCENE_Start;
	}
	switch (state) {
		case SCENE_Start:
			
			break;
		case SCENE_MainMenu:
			
			break;
		case SCENE_Battle:
			
			break;
		case SCENE_BattleResult:
			
			break;
		default:
			break;
	}
	return state;
}


#endif /* TASKS_H_ */