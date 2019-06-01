/*
 * statemachines.h
 *
 * Created: 5/20/2019 4:31:30 AM
 *  Author: Edward-Laptop
 */ 


#ifndef TASKS_H_
#define TASKS_H_

#include <string.h>
#include "sprites.h"


#define TASKS_SIZE	7
#define LCD_MENU_SELECTOR	'*'

#define TIME_1SEC	1000
#define TIME_2SEC	2000
#define TIME_3SEC	3000
#define TIME_5SEC	5000

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

enum SNES_L_States {SNES_L_Released, SNES_L_Pressed} SNES_L_State;
enum SNES_R_States {SNES_R_Released, SNES_R_Pressed} SNES_R_State;

enum Scene_States {SCENE_Start, SCENE_MainMenu, SCENE_PokemonMenu, SCENE_SettingsMenu,
	SCENE_Battle_Intro, SCENE_Battle_ActionMenu, SCENE_Battle_MoveMenu, SCENE_Battle_MoveUsed, 
	SCENE_Battle_MoveMiss, SCENE_Battle_MoveEffective, SCENE_Battle_MoveCritical, SCENE_Battle_MoveEffect,
	SCENE_Battle_MoveResolution, /*SCENE_PlayerPokemonDead, SCENE_Battle_EnemyPokemonDead, */
	SCENE_Battle_TrainerVictory, SCENE_Battle_TrainerDefeat, 
	SCENE_Error
} sceneState;
enum MENU_ITEMS {MI_TopLeft, MI_TopRight, MI_BotLeft, MI_BotRight};

int8_t menuLength = 4;
uint8_t sceneIndex = SCENE_MainMenu;
uint8_t pressedB = 0; // TODO: Combine into one variable or queue
uint8_t pressedX = 0;
uint8_t pressedLeft = 0;
uint8_t pressedRight = 0;
uint8_t pressedUp = 0;
uint8_t pressedDown = 0;
long textDisplayTimer = 0;

Pokemon playerMainPokemon;
Pokemon enemyMainPokemon;
Move moveList[4];

uint8_t test_health = 50;
uint8_t gameDifficulty = 1;
uint8_t gameSound = 0;

typedef struct _Trainer {
	char* name;
	Pokemon pokemon[6];
	uint8_t pokemonLength;			// size of pokemon array
	uint8_t activeIndex;	// index of currently active pokemon
	uint8_t favoriteIndex;	// index of pokemon that goes first in battle
	uint8_t isPlayer;
} Trainer;

Trainer player, enemy;

enum TYPE_EFFECT {TE_NONE, TE_WEAK, TE_STRONG};
uint8_t getEffectiveness(uint8_t moveType, uint8_t targetType) {
	switch (moveType) {
		case T_NORMAL:
		return TE_NONE;
		case T_GRASS:
		switch (targetType) {
			case T_NORMAL:
			return TE_NONE;
			case T_GRASS:
			case T_FIRE:
			return TE_WEAK;
			case T_WATER:
			return TE_STRONG;
		}
		break;
		case T_WATER:
		switch (targetType) {
			case T_NORMAL:
			return TE_NONE;
			case T_GRASS:
			case T_WATER:
			return TE_WEAK;
			case T_FIRE:
			return TE_STRONG;
		}
		break;
		case T_FIRE:
		switch (targetType) {
			case T_NORMAL:
			return TE_NONE;
			case T_WATER:
			case T_FIRE:
			return TE_WEAK;
			case T_GRASS:
			return TE_STRONG;
		}
		break;
		default:
		return TE_NONE;
	}
	return TE_NONE;
}

typedef struct _MoveResult {
	char* moveName;
	uint8_t moveIndex;
	uint8_t isHit;
	uint8_t isCritical;
	uint8_t effectiveness;
	int8_t damage;
	uint8_t isEffectHit;
	uint8_t effect;
	Trainer* attacker;
	Trainer* defender;
	uint8_t isPlayerDead;
	uint8_t isEnemyDead;
} MoveResult;

MoveResult moveResult;

void generateMoveResult(uint8_t moveIndex, Trainer *attacker, Trainer *defender) {
	moveResult.attacker = attacker;
	moveResult.defender = defender;
	moveResult.moveIndex = moveIndex;
	moveResult.moveName = moveList[moveIndex].name;
	// Calculate if move hits
	int8_t hitModifier = 0; // TODO
	int8_t critModifier = 0; // TODO
	moveResult.isHit = ((rand() % 100 + 1) + hitModifier <= moveList[moveIndex].hitChance);
	moveResult.isCritical = ((rand() % 100 + 1) + critModifier <= moveList[moveIndex].criticalChance);
	moveResult.isEffectHit = ((rand() % 100 + 1) <= moveList[moveIndex].effectChance);
	
	// Calculate damage multipliers (critical, effectiveness)
	double multiplier = 1;
	multiplier += (moveResult.isCritical) ? 0.5 : 0;
	uint8_t moveType = moveList[moveIndex].type;
	uint8_t targetType = (*defender).pokemon[(*defender).activeIndex].type;
	moveResult.effectiveness = getEffectiveness(moveType, targetType);
	switch (moveResult.effectiveness) {
		case TE_STRONG: multiplier += 0.5; break;
		case TE_WEAK: multiplier -= 0.5; break;
		case TE_NONE: break;
		default: break;
	}
	
	// Calculate damage additives (attack bonus, defense bonus)
	double damage = moveList[moveIndex].baseDamage;
	uint8_t attack = (*attacker).pokemon[(*attacker).activeIndex].attack;
	uint8_t defense = (*defender).pokemon[(*defender).activeIndex].defense;
	moveResult.damage = (int8_t)(damage * multiplier) + attack - defense;
	if (moveResult.damage < 1) moveResult.damage = 1;

	if (moveResult.isHit) {
		// Change health
		int8_t health = moveResult.defender->pokemon[moveResult.defender->activeIndex].health;
		health -= moveResult.damage;
		if (health <= 0) {
			health = 0;
			moveResult.defender->pokemon[moveResult.defender->activeIndex].isKnockedOut = 1;
			if (defender->isPlayer) {
				moveResult.isPlayerDead = 1;
			} else {
				moveResult.isEnemyDead = 1;
			}
		}
		moveResult.defender->pokemon[moveResult.defender->activeIndex].health = health;
		moveResult.defender->pokemon[moveResult.defender->activeIndex].healthLost = moveResult.damage;
		
		// Effect
		switch(moveResult.effect) {
			case E_ENEMY_BURN:
			case E_ENEMY_ATTACK_LOW:
			case E_ENEMY_DEFENSE_LOW:
			case E_ENEMY_SPEED_LOW:
			case E_ENEMY_ACCURACY_LOW:
				moveResult.defender->pokemon[moveResult.defender->activeIndex].effect = moveResult.effect;
				break;
			case E_SELF_ATTACK_HIGH:
			case E_SELF_DEFENSE_HIGH:
			case E_SELF_SPEED_HIGH:
			case E_SELF_ACCURACY_HIGH:
				moveResult.attacker->pokemon[moveResult.defender->activeIndex].effect = moveResult.effect;
				break;
			case E_NONE:
				// do nothing
				break;
		}
	} else {
		// TODO: effects that still work if attack missed?
	}
}

void hideMenuCursors(uint8_t menuLength) {
	uint8_t i;
	for (i = 0; i < menuLength; i++) {
		uint8_t cursor = (i * 8) + 1;
		LCD_Cursor(cursor);
		LCD_WriteData(' ');
	}
}

void drawMenuCursor(int8_t menuIndex, uint8_t menuLength) {
	uint8_t cursor = (menuIndex * 8) + 1;
	hideMenuCursors(menuLength);
	LCD_Cursor(cursor);
	LCD_WriteData(LCD_MENU_SELECTOR);
	LCD_Cursor(cursor);
}

void reduceHealthLost(Trainer *trainer, uint8_t amount) {
	int8_t healthLost = (*trainer).pokemon[(*trainer).activeIndex].healthLost;
	healthLost -= amount;
	if (healthLost < 0) healthLost = 0;
	(*trainer).pokemon[(*trainer).activeIndex].healthLost = healthLost;
}

void drawPokemonSprite(Trainer *trainer, uint8_t isPlayer) {
	uint8_t xoffset, yoffset;
	if (isPlayer) {
		xoffset = 0;
		yoffset = 48-1-20;
	} else {
		xoffset = 84-1-24;
		yoffset = 0;
	}
	NokiaLCD_CustomBitmap(bitmaps[(*trainer).pokemon[(*trainer).activeIndex].spriteFrontIndex], xoffset, yoffset, isPlayer);
}

void drawPokemonUI(Trainer* trainer, uint8_t isPlayer) {
	uint8_t index = (*trainer).activeIndex;
	if (isPlayer) NokiaLCD_SetCursor(31, 8*4);
	else NokiaLCD_SetCursor(0, 0);
	NokiaLCD_WriteString((*trainer).pokemon[index].name);
	
	if (isPlayer) NokiaLCD_SetCursor(24+49, 8*5);
	else NokiaLCD_SetCursor(0, 8);
	NokiaLCD_WriteString("HP");
	
	uint8_t health = (*trainer).pokemon[index].health;
	uint8_t healthMax = (*trainer).pokemon[index].healthMax;
	uint8_t healthLost = (*trainer).pokemon[index].healthLost;
	if (isPlayer) NokiaLCD_HealthBar(31, 42, health, healthMax, healthLost);
	else NokiaLCD_HealthBar(12, 10, health, healthMax, healthLost);
	
	char* healthString[3];
	char* healthMaxString[4];
	//strncpy(buff,"xyz\0",4);
	itoa(healthLost, healthString, 10);
	itoa(healthMax, healthMaxString, 10);
	if (isPlayer) {
		NokiaLCD_SetCursor(84-1-7*6 ,3*8);
	} else {
		NokiaLCD_SetCursor(0,2*8);
	}
	NokiaLCD_WriteString((const char*)healthString);
	NokiaLCD_WriteString("/");
	NokiaLCD_WriteString((const char*)healthMaxString);
}

void initializeMoves() {
	moveList[ID_TACKLE] = Move_Tackle();
	moveList[ID_VINEWHIP] = Move_VineWhip();
	moveList[ID_BUBBLE] = Move_Bubble();
	moveList[ID_EMBER] = Move_Ember();
}

void initializePlayerTrainer() {
	player.name = "AshKetchum";
	player.pokemonLength = 0;
	player.pokemon[player.pokemonLength++] = new_Pokemon(ID_CHARMANDER);
	player.activeIndex = 0;
	player.favoriteIndex = 0;
	player.isPlayer = 1;
}

void initializeEnemyTrainer(uint8_t level) {
	enemy.name = "Trainer";
	enemy.pokemonLength = 0;
	enemy.pokemon[enemy.pokemonLength++] = new_Pokemon(ID_BULBASAUR);
	enemy.activeIndex = 0;
	enemy.favoriteIndex = 0;
	enemy.isPlayer = 0;
}

void draw_MainMenu(uint8_t menuIndex) {
	switch (menuIndex) {
		case MI_TopLeft:
			NokiaLCD_SetLine(4, 1);
			NokiaLCD_WriteString("Find Pokemon  in the tall   grass!  ");
			break;
		case MI_TopRight:
			NokiaLCD_SetLine(4, 1);
			NokiaLCD_WriteString("Battle other  trainers!             ");
			break;
		case MI_BotLeft:
			NokiaLCD_SetLine(4, 1);
			NokiaLCD_WriteString("View your     Pokemon!              ");
			break;
		case MI_BotRight:
			NokiaLCD_SetLine(4, 1);
			NokiaLCD_WriteString("Set difficultyand other     things! ");
			break;
		default:
			NokiaLCD_WriteString("ERROR: menuIndex");
	}
	drawMenuCursor(menuIndex, 4);
}

void init_MainMenu(uint8_t menuIndex) {
	LCD_ClearScreen();
	LCD_DisplayMenu4("Catch", "Battle", "Pokemon", "Settings");
	
	NokiaLCD_Clear();
	NokiaLCD_WriteString("Main Menu");
	
	draw_MainMenu(menuIndex);
}

char* getPokemonName(Trainer *trainer, uint8_t index) {
	return trainer->pokemon[index].name;
}

void draw_PokemonMenu(int8_t menuIndex, int8_t nokiaMenuIndex) {
	uint8_t m0 = player.pokemon[nokiaMenuIndex].moveIds[0];
	uint8_t m1 = player.pokemon[nokiaMenuIndex].moveIds[1];
	uint8_t m2 = player.pokemon[nokiaMenuIndex].moveIds[2];
	uint8_t m3 = player.pokemon[nokiaMenuIndex].moveIds[3];
	NokiaLCD_Clear();
	switch (menuIndex) {
		case MI_TopLeft:
		
			//NokiaLCD_WriteString(player.name);
			NokiaLCD_WriteString(player.pokemon[nokiaMenuIndex].name);
			NokiaLCD_SetLine(3, 0);
			NokiaLCD_WriteString(moveList[m0].name);
			NokiaLCD_SetLine(4, 0);
			NokiaLCD_WriteString(moveList[m1].name);
			NokiaLCD_SetLine(5, 0);
			NokiaLCD_WriteString(moveList[m2].name);
			NokiaLCD_SetLine(6, 0);
			NokiaLCD_WriteString(moveList[m3].name);
			drawPokemonSprite(&player, 0);
			break;
		case MI_TopRight:
			NokiaLCD_WriteString(player.pokemon[nokiaMenuIndex].name);
			NokiaLCD_SetLine(3, 0);
			NokiaLCD_WriteString("Location: ");
			NokiaLCD_SetLine(4,0);
			NokiaLCD_WriteString("  Main");
			drawPokemonSprite(&player, 0);
			break;
		default:
			break;
	}
	drawMenuCursor(menuIndex, 2);
}

void update_PokemonMenu(int8_t menuIndex, int8_t nokiaMenuIndex) {
	switch (menuIndex) {
		case MI_TopLeft:
			// TODO
			break;
		case MI_TopRight:
			player.favoriteIndex = nokiaMenuIndex;
			break;
	}
	draw_PokemonMenu(menuIndex, nokiaMenuIndex);
}

void init_PokemonMenu(int8_t menuIndex, int8_t nokiaMenuIndex) {
	LCD_ClearScreen();
	LCD_DisplayMenu2("Moves", "SetMain");
	
	NokiaLCD_Clear();
	NokiaLCD_WriteString("Pokemon List");
	
	draw_PokemonMenu(menuIndex, nokiaMenuIndex);
}

void draw_SettingsMenu(uint8_t menuIndex) {
	NokiaLCD_Clear();
	NokiaLCD_WriteString("Settings Menu");
	NokiaLCD_SetLine(5, 1);
	NokiaLCD_WriteString("Difficulty:  ");
	char* str;
	switch(gameDifficulty) {
		case 1: str = "1"; break;
		case 2: str = "2"; break;
		case 3: str = "3"; break;
		default: str = "e";
	}
	NokiaLCD_WriteString(str);
	if (gameSound) NokiaLCD_WriteString("Sound:      ON");
	else NokiaLCD_WriteString("Sound:     OFF");
	
	drawMenuCursor(menuIndex, 2);
}

void update_SettingsMenu(uint8_t menuIndex) {
	switch (menuIndex) {
		case MI_TopLeft:
			gameDifficulty++;
			if (gameDifficulty > 3) gameDifficulty = 1;
			break;
		case MI_TopRight:
			gameSound = (gameSound) ? 0 : 1;
			break;
	}
	draw_SettingsMenu(menuIndex);
}

void init_SettingsMenu(uint8_t menuIndex) {
	LCD_ClearScreen();
	LCD_DisplayMenu2("Level", "Sound");
	
	NokiaLCD_Clear();
	NokiaLCD_WriteString("Settings Menu");
	
	draw_SettingsMenu(menuIndex);
}

void init_BattleIntro(long displayTime) {
	uint8_t enemyLevel = 1;
	initializeEnemyTrainer(enemyLevel);
	
	textDisplayTimer = displayTime;
	LCD_ClearScreen();
	LCD_DisplayString(1, "Battle Intro");
	LCD_Cursor(32);
	
	NokiaLCD_Clear();
	NokiaLCD_WriteString("Intro");
}

void draw_Battle_ActionMenu(uint8_t menuIndex) {
	drawMenuCursor(menuIndex, 4);
}

void init_Battle_ActionMenu(uint8_t menuIndex) {
	LCD_ClearScreen();
	LCD_DisplayMenu4("Fight", "Pokedex", "Switch", "Escape");
	
	NokiaLCD_Clear();
	drawPokemonUI(&enemy, 0);
	drawPokemonSprite(&enemy, 0);
	drawPokemonUI(&player, 1);
	drawPokemonSprite(&player, 1);
	
	draw_Battle_ActionMenu(menuIndex);
}

void draw_Battle_MoveMenu(uint8_t menuIndex) {
	NokiaLCD_Clear();
	uint8_t moveIndex = player.pokemon[player.activeIndex].moveIds[menuIndex];
	NokiaLCD_WriteString(moveList[moveIndex].name);
	NokiaLCD_SetLine(3, 0);
	NokiaLCD_WriteString(moveList[moveIndex].description);
	
	drawMenuCursor(menuIndex, 4);
}

void init_Battle_MoveMenu(uint8_t menuIndex) {
	LCD_ClearScreen();
	uint8_t m0 = player.pokemon[player.activeIndex].moveIds[0];
	uint8_t m1 = player.pokemon[player.activeIndex].moveIds[1];
	uint8_t m2 = player.pokemon[player.activeIndex].moveIds[2];
	uint8_t m3 = player.pokemon[player.activeIndex].moveIds[3];
	LCD_DisplayMenu4(moveList[m0].name, moveList[m1].name, moveList[m2].name, moveList[m3].name);
	drawMenuCursor(menuIndex, 4);
	
	draw_Battle_MoveMenu(menuIndex);
}

void init_Battle_MoveUsed(uint8_t menuIndex, long displayTime) {
	uint8_t moveIndex = player.pokemon[player.activeIndex].moveIds[menuIndex];
	generateMoveResult(moveIndex, &player, &enemy);
	
	textDisplayTimer = displayTime;
	LCD_ClearScreen();
	char* pokemonName = player.pokemon[player.activeIndex].name;
	LCD_DisplayString(1, pokemonName);
	LCD_DisplayString(1+strlen(pokemonName), " used");
	LCD_DisplayString(17, moveResult.moveName);
	LCD_DisplayString(17+strlen(moveResult.moveName), "!");
	LCD_Cursor(32);
	
	NokiaLCD_Clear();
	drawPokemonUI(&enemy, 0);
	drawPokemonSprite(&enemy, 0);
	drawPokemonUI(&player, 1);
	drawPokemonSprite(&player, 1);
}

void init_Battle_MoveMiss(long displayTime) {
	LCD_ClearScreen();
	LCD_DisplayString(1, "But it missed!");
	LCD_Cursor(32);
}

void init_Battle_MoveEffective(long displayTime) {
	LCD_ClearScreen();
	if (moveResult.effectiveness == TE_STRONG) {
		LCD_DisplayString(1, "It is super");
		LCD_DisplayString(17, "effective!");
	} else if (moveResult.effectiveness == TE_WEAK) {
		LCD_DisplayString(1, "It is not very");
		LCD_DisplayString(17, "effective...");
	} else {
		LCD_DisplayString(1, "ERROR: moveResult.effectiveness");
	}
	LCD_Cursor(32);
}

void init_Battle_MoveCritical(long displayTime) {
	LCD_ClearScreen();
	LCD_DisplayString(1, "A critical hit!");
	LCD_Cursor(32);
}

void init_Battle_MoveEffect(long displayTime) {
	LCD_ClearScreen();
	char * effectText = moveList[moveResult.moveIndex].description;
	LCD_DisplayString(1, effectText);
	LCD_Cursor(32);
}

void init_Battle_TrainerVictory(long displayTime) {
	LCD_ClearScreen();
	LCD_DisplayString(1, "You won!");
	LCD_Cursor(32);
	
	NokiaLCD_Clear();
	drawPokemonUI(&player, 1);
	drawPokemonSprite(&player, 1);
}

void init_Battle_TrainerDefeat(long displayTime) {
	LCD_ClearScreen();
	LCD_DisplayString(1, "You lost!");
	LCD_Cursor(32);
	
	NokiaLCD_Clear();
	drawPokemonUI(&enemy, 0);
	drawPokemonSprite(&enemy, 0);
}

void init_ErrorScreen(char * error) {
	LCD_ClearScreen();
	LCD_DisplayString(1, "Error");
	LCD_DisplayString(17, error);
	LCD_DisplayString(30, "B+X");
	NokiaLCD_Clear();
	NokiaLCD_WriteString("Error");
	NokiaLCD_SetLine(2, 0);
	NokiaLCD_WriteString(error);
}

void clearButtons() {
	pressedB = 0;
	pressedX = 0;
	pressedLeft = 0;
	pressedRight = 0;
}

int8_t constraint(int8_t index, int8_t length) {
	if (index < 0) return length + index;
	if (index > length - 1) return index - length;
	return index;
}

int Scene_Tick(int state) {
	static int8_t menuIndex = 0;
	static int8_t nokiaMenuIndex = 0;
	
	switch (state) {
		case SCENE_Start:
			clearButtons();
			initializeMoves();
			initializePlayerTrainer();
			
			state = SCENE_MainMenu;
			menuIndex = MI_TopLeft;
			init_MainMenu(menuIndex);
			break;
		case SCENE_MainMenu:
			if (pressedB) {
				clearButtons();
				switch(menuIndex) {
					case MI_TopLeft:
						state = SCENE_Battle_Intro;
						init_BattleIntro(TIME_3SEC);
						break;
					case MI_TopRight:
						state = SCENE_Battle_Intro;
						init_BattleIntro(TIME_3SEC);
						break;
					case MI_BotLeft:
						state = SCENE_PokemonMenu;
						menuIndex = MI_TopLeft;
						init_PokemonMenu(menuIndex, 0);
						break;
					case MI_BotRight:
						state = SCENE_SettingsMenu;
						menuIndex = MI_TopLeft;
						init_SettingsMenu(menuIndex);
						break;
				}
			} else if (pressedLeft) {
				clearButtons();
				menuIndex = constraint(menuIndex-1, 4);
				draw_MainMenu(menuIndex);
			} else if (pressedRight) {
				clearButtons();
				menuIndex = constraint(menuIndex+1, 4);
				draw_MainMenu(menuIndex);
			} else if (pressedUp) {
				clearButtons();
				menuIndex = constraint(menuIndex-2, 4);
				draw_MainMenu(menuIndex);
			} else if (pressedDown) {
				clearButtons();
				menuIndex = constraint(menuIndex+2, 4);
				draw_MainMenu(menuIndex);
			}else {
				state = SCENE_MainMenu;
			}
			break;
		case SCENE_Battle_Intro:
			if (textDisplayTimer < 0 || pressedB) {
				clearButtons();
				state = SCENE_Battle_ActionMenu;
				menuIndex = MI_TopLeft;
				init_Battle_ActionMenu(menuIndex);
			} else {
				state = SCENE_Battle_Intro;
			}
			break;
		case SCENE_Battle_ActionMenu:
			if (pressedB) {
				clearButtons();
				switch(menuIndex) {
					case MI_TopLeft:
						state = SCENE_Battle_MoveMenu;
						menuIndex = MI_TopLeft;
						init_Battle_MoveMenu(menuIndex);
						break;
					case MI_TopRight:
						// TODO: Enemy view
						break;
					case MI_BotLeft:
						// TODO: Switch pokemon
						break;
					case MI_BotRight:
						state = SCENE_MainMenu;
						menuIndex = MI_TopLeft;
						init_MainMenu(menuIndex);
						// TODO: Escape instead
						break;
				}
			} else if (pressedLeft) {
				clearButtons();
				menuIndex = constraint(menuIndex-1, 4);
				draw_Battle_ActionMenu(menuIndex);
			} else if (pressedRight) {
				clearButtons();
				menuIndex = constraint(menuIndex+1, 4);
				draw_Battle_ActionMenu(menuIndex);
			} else if (pressedUp) {
				clearButtons();
				menuIndex = constraint(menuIndex-2, 4);
				draw_Battle_ActionMenu(menuIndex);
			} else if (pressedDown) {
				clearButtons();
				menuIndex = constraint(menuIndex+2, 4);
				draw_Battle_ActionMenu(menuIndex);
			} else {
				state = SCENE_Battle_ActionMenu;
			}
			break;
		case SCENE_Battle_MoveMenu:
			if (pressedB) {
				clearButtons();
				state = SCENE_Battle_MoveUsed;
				init_Battle_MoveUsed(menuIndex, TIME_3SEC);
			} else if (pressedX) {
				clearButtons();
				state = SCENE_Battle_ActionMenu;
				menuIndex = MI_TopLeft;
				init_Battle_ActionMenu(menuIndex);
			} else if (pressedLeft) {
				clearButtons();
				menuIndex = constraint(menuIndex-1, 4);
				draw_Battle_MoveMenu(menuIndex);
			} else if (pressedRight) {
				clearButtons();
				menuIndex = constraint(menuIndex+1, 4);
				draw_Battle_MoveMenu(menuIndex);
			} else if (pressedUp) {
				clearButtons();
				menuIndex = constraint(menuIndex-2, 4);
				draw_Battle_MoveMenu(menuIndex);
			} else if (pressedDown) {
				clearButtons();
				menuIndex = constraint(menuIndex+2, 4);
				draw_Battle_MoveMenu(menuIndex);
			} else {
			
			}
			break;
		case SCENE_Battle_MoveUsed:
			if (textDisplayTimer < 0 || pressedB) {
				clearButtons();
				if (!moveResult.isHit) {
					state = SCENE_Battle_MoveMiss;
					init_Battle_MoveMiss(TIME_3SEC);
				} else {
					uint8_t effective = moveResult.effectiveness != TE_NONE;
					if (!effective && !moveResult.isCritical && !moveResult.isEffectHit) {
						state = SCENE_Battle_MoveResolution;
					} else if (!effective && !moveResult.isCritical && moveResult.isEffectHit) {
						state = SCENE_Battle_MoveEffect;
						init_Battle_MoveEffect(TIME_3SEC);
					} else if (!effective && moveResult.isCritical) {
						state = SCENE_Battle_MoveCritical;
						init_Battle_MoveCritical(TIME_3SEC);
					} else if (effective) {
						state = SCENE_Battle_MoveEffective;
						init_Battle_MoveEffective(TIME_3SEC);
					} else {
						state = SCENE_Error;
						init_ErrorScreen("Battle_MoveUsed");
					}
				}
			} else {
				state = SCENE_Battle_MoveUsed;
			}
			break;
		case SCENE_Battle_MoveMiss:
			if (textDisplayTimer < 0 || pressedB) {
				clearButtons();
				state = SCENE_Battle_MoveResolution;
			} else {
				state = SCENE_Battle_MoveMiss;
			}
			break;
		case SCENE_Battle_MoveEffective:
			if (textDisplayTimer < 0 || pressedB) {
				clearButtons();
				if (moveResult.isCritical) {
					state = SCENE_Battle_MoveCritical;
					init_Battle_MoveCritical(TIME_3SEC);
				} else if (moveResult.isEffectHit) {
					state = SCENE_Battle_MoveEffect;
					init_Battle_MoveEffect(TIME_3SEC);
				} else {
					state = SCENE_Battle_ActionMenu;
					menuIndex = MI_TopLeft;
					init_Battle_ActionMenu(menuIndex);
				}
			} else {
				state = SCENE_Battle_MoveEffective;
			}
			break;
		case SCENE_Battle_MoveEffect:
			if (textDisplayTimer < 0 || pressedB) {
				clearButtons();
				state = SCENE_Battle_MoveResolution;
			} else {
				state = SCENE_Battle_MoveEffect;
			}
			break;
		case SCENE_Battle_MoveResolution:
			if (moveResult.isEnemyDead) {
				state = SCENE_Battle_TrainerVictory;
				init_Battle_TrainerVictory(TIME_5SEC);
			} else if (moveResult.isPlayerDead) {
				state = SCENE_Battle_TrainerDefeat;
				init_Battle_TrainerDefeat(TIME_5SEC);
			} else {
				state = SCENE_Battle_ActionMenu;
				menuIndex = MI_TopLeft;
				init_Battle_ActionMenu(menuIndex);
			}
			break;
		case SCENE_Battle_TrainerVictory:
			if (textDisplayTimer < 0 || pressedB) {
				clearButtons();
				state = SCENE_MainMenu;
				menuIndex = MI_TopRight;
				init_MainMenu(menuIndex);
			} else {
				state = SCENE_Battle_TrainerVictory;
			}
			break;
		case SCENE_Battle_TrainerDefeat:
			if (textDisplayTimer < 0 || pressedB) {
				clearButtons();
				state = SCENE_MainMenu;
				menuIndex = MI_TopRight;
				init_MainMenu(menuIndex);
			} else {
				state = SCENE_Battle_TrainerDefeat;
			}
			break;
		case SCENE_PokemonMenu:
			if (pressedB) {
				clearButtons();
				state = SCENE_PokemonMenu;
				update_PokemonMenu(menuIndex, nokiaMenuIndex);
				draw_PokemonMenu(menuIndex, nokiaMenuIndex);
			} else if (pressedX) {
				clearButtons();
				state = SCENE_MainMenu;
				menuIndex = MI_BotLeft;
				init_MainMenu(menuIndex);
			} else if (pressedLeft) {
				clearButtons();
				state = SCENE_PokemonMenu;
				menuIndex = constraint(menuIndex-1, 2);
				draw_PokemonMenu(menuIndex, nokiaMenuIndex);
			} else if (pressedRight) {
				clearButtons();
				state = SCENE_PokemonMenu;
				menuIndex = constraint(menuIndex+1, 2);
				draw_PokemonMenu(menuIndex, nokiaMenuIndex);
			} else if (pressedUp) {
				clearButtons();
				state = SCENE_PokemonMenu;
				nokiaMenuIndex = constraint(nokiaMenuIndex-1, player.pokemonLength);
				draw_PokemonMenu(menuIndex, nokiaMenuIndex);
			} else if (pressedDown) {
				clearButtons();
				state = SCENE_PokemonMenu;
				nokiaMenuIndex = constraint(nokiaMenuIndex+1, player.pokemonLength);
				draw_PokemonMenu(menuIndex, nokiaMenuIndex);
			} else {
				state = SCENE_PokemonMenu;
			}
			break;
		case SCENE_SettingsMenu:
			if (pressedB) {
				clearButtons();
				update_SettingsMenu(menuIndex);
			} else if (pressedX) {
				clearButtons();
				state = SCENE_MainMenu;
				menuIndex = MI_BotRight;
				init_MainMenu(menuIndex);
			} else if (pressedLeft) {
				clearButtons();
				state = SCENE_SettingsMenu;
				menuIndex = constraint(menuIndex-1, 2);
				draw_SettingsMenu(menuIndex);
			} else if (pressedRight) {
				clearButtons();
				state = SCENE_SettingsMenu;
				menuIndex = constraint(menuIndex+1, 2);
				draw_SettingsMenu(menuIndex);
			} else {
				state = SCENE_SettingsMenu;
			}
			break;
		case SCENE_Error:
			if (pressedB && pressedX) {
				clearButtons();
				state = SCENE_MainMenu;
				menuIndex = MI_BotRight;
				init_MainMenu(menuIndex);
			} else {
				state = SCENE_Error;
			}
			break;
		default:
			state = SCENE_Start;
			break;
	}
	
	switch (state) {
		case SCENE_Start:
		case SCENE_MainMenu:
		case SCENE_PokemonMenu:
		case SCENE_SettingsMenu:
			textDisplayTimer = 0;
			break;
		case SCENE_Battle_Intro:
			textDisplayTimer -= scenePeriod;
			break;
		case SCENE_Battle_ActionMenu:
		case SCENE_Battle_MoveMenu:
		case SCENE_Battle_MoveResolution:
			textDisplayTimer = 0;
			reduceHealthLost(&player, 2);
			reduceHealthLost(&enemy, 2);
			break;
		case SCENE_Battle_MoveUsed:
		case SCENE_Battle_MoveMiss:
		case SCENE_Battle_MoveEffective:
		case SCENE_Battle_MoveCritical:
		case SCENE_Battle_MoveEffect:
		case SCENE_Battle_TrainerDefeat:
		case SCENE_Battle_TrainerVictory:
			textDisplayTimer -= scenePeriod;
			reduceHealthLost(&player, 2);
			reduceHealthLost(&enemy, 2);
			drawPokemonUI(&player, 1);
			drawPokemonUI(&enemy, 0);
			break;
		case SCENE_Error:
		default:
			break;
	}
	return state;
};

enum SNES_B_States {SNES_B_Released, SNES_B_Pressed} SNES_B_State;
int SNES_B_Tick(int state) {
	switch(state) {
		case SNES_B_Released:
			if (SNES_B && textDisplayTimer > 0) {
				textDisplayTimer = 0;
				state = SNES_B_Pressed;
			} else if (SNES_B && textDisplayTimer <= 0) {
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
			if (SNES_X && textDisplayTimer <= 0) {
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
			if (SNES_LEFT && textDisplayTimer <= 0) {
				state = SNES_LEFT_Pressed;
				pressedLeft = 1;
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
			if (SNES_RIGHT && textDisplayTimer <= 0) {
				state = SNES_RIGHT_Pressed;
				pressedRight = 1;
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

int SNES_L_Tick(int state) {
	switch(state) {
		case SNES_L_Released:
			if (SNES_L && textDisplayTimer <= 0) {
				state = SNES_L_Pressed;
				//pressed_L = 1;
			} else {
				state = SNES_L_Released;
			}
			break;
		case SNES_L_Pressed:
			state = (SNES_L) ? SNES_L_Pressed : SNES_L_Released;
			break;
	}
	return state;
};

int SNES_R_Tick(int state) {
	switch(state) {
		case SNES_R_Released:
			if (SNES_R && textDisplayTimer <= 0) {
				state = SNES_R_Pressed;
				//pressed_R = 1;	
			} else {
				state = SNES_R_Released;
			}
			break;
		case SNES_R_Pressed:
			state = (SNES_R) ? SNES_R_Pressed : SNES_R_Released;
			break;
	}
	return state;
};

#endif /* TASKS_H_ */