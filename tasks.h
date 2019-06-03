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


#define TASKS_SIZE	9
#define LCD_MENU_SELECTOR	'*'

#define TIME_1SEC	1000
#define TIME_2SEC	2000
#define TIME_3SEC	3000
#define TIME_5SEC	5000

unsigned long scenePeriod = 50; // ms
unsigned long buttonPeriod = 50; // ms
unsigned long tasksPeriod = 50; // ms
unsigned long SNES_Button_Period = 50; // ms

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
	SCENE_Battle_Intro, SCENE_Battle_ActionMenu, SCENE_Battle_MoveMenu, 
	SCENE_Battle_MoveUsed, SCENE_Battle_MoveMiss, SCENE_Battle_MoveEffective, SCENE_Battle_MoveCritical, SCENE_Battle_MoveEffect,
	SCENE_Battle_MoveResolution, SCENE_Battle_TrainerVictory, SCENE_Battle_TrainerDefeat, 
	SCENE_Battle_RoundStart, SCENE_Battle_TurnStart, SCENE_Battle_PlayerTurn, SCENE_Battle_EnemyTurn, 
	SCENE_Error
} sceneState;
enum MENU_ITEMS {MI_TopLeft, MI_TopRight, MI_BotLeft, MI_BotRight};

uint8_t pressedB = 0; // TODO: Combine into one variable or queue
uint8_t pressedX = 0;
uint8_t pressedLeft = 0;
uint8_t pressedRight = 0;
uint8_t pressedUp = 0;
uint8_t pressedDown = 0;
long textDisplayTimer = 0;

Pokemon playerMainPokemon;
Pokemon enemyMainPokemon;
Move moveList[5];

uint8_t test_health = 50;
int8_t gameDifficulty = 1;
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

char* getPokemonName(Trainer *trainer) {
	return trainer->pokemon[trainer->activeIndex].name;
}

uint8_t getAgility(Trainer *trainer) {
	return trainer->pokemon[trainer->activeIndex].agility;
}

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

void clearMoveResult() {
	moveResult.moveName = "ERROR";
	moveResult.moveIndex = 0;
	moveResult.isHit = 0;
	moveResult.isCritical = 0;
	moveResult.effectiveness = 0;
	moveResult.isEffectHit = 0;
	moveResult.effect = E_NONE;
	moveResult.attacker = 0;
	moveResult.defender = 0;
	moveResult.isPlayerDead = 0;
	moveResult.isEnemyDead = 0;
	
}

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
		
		// Calculate health lost and set new health
		moveResult.defender->pokemon[moveResult.defender->activeIndex].healthLost = moveResult.defender->pokemon[moveResult.defender->activeIndex].health - health;
		moveResult.defender->pokemon[moveResult.defender->activeIndex].health = health;
		
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

void drawPokemonSprite(Trainer *trainer, uint8_t pokemonIndex, uint8_t isPlayer) {
	if (pokemonIndex < 0 || pokemonIndex >= trainer->pokemonLength) return;
	uint8_t xoffset, yoffset;
	if (isPlayer) {
		xoffset = 0;
		yoffset = 48-1-20;
	} else {
		xoffset = 86-1-24;
		yoffset = 0;
	}
	NokiaLCD_CustomBitmap(bitmaps[trainer->pokemon[pokemonIndex].spriteFrontIndex], xoffset, yoffset, isPlayer);
}

void drawPokemonBattleUI(Trainer* trainer, uint8_t isPlayer) {
	uint8_t index = (*trainer).activeIndex;
	char* pokemonName = trainer->pokemon[trainer->activeIndex].name;
	uint8_t length = strlen(pokemonName);
	uint8_t health = (*trainer).pokemon[index].health;
	uint8_t healthMax = (*trainer).pokemon[index].healthMax;
	uint8_t healthLost = (*trainer).pokemon[index].healthLost;
	char* healthString[3];
	itoa((health+healthLost), healthString, 10);
	
	// Pokemon name
	if (isPlayer) NokiaLCD_SetCursor(86-1-length*6, 8*4);
	else NokiaLCD_SetCursor(0, 0);
	NokiaLCD_WriteString(pokemonName);
	
	// Health number
	if (isPlayer) NokiaLCD_SetCursor(86-1-2*6, 8*5);
	else NokiaLCD_SetCursor(0, 8);
	NokiaLCD_WriteString((const char*)healthString);
	
	// Health bar
	if (isPlayer) NokiaLCD_HealthBar(31, 42, health, healthMax, healthLost);
	else NokiaLCD_HealthBar(12, 10, health, healthMax, healthLost);
}

void initializeMoves() {
	moveList[ID_TACKLE] = Move_Tackle();
	moveList[ID_VINEWHIP] = Move_VineWhip();
	moveList[ID_BUBBLE] = Move_Bubble();
	moveList[ID_EMBER] = Move_Ember();
	moveList[ID_BITE] = Move_Bite();
}

void initializePlayerTrainer() {
	player.name = "AshKetchum";
	player.pokemonLength = 0;
	player.pokemon[player.pokemonLength++] = new_Pokemon(ID_BULBASAUR);
	player.pokemon[player.pokemonLength++] = new_Pokemon(ID_SQUIRTLE);
	player.pokemon[player.pokemonLength++] = new_Pokemon(ID_CHARMANDER);
	player.activeIndex = 0;
	player.favoriteIndex = 0;
	player.isPlayer = 1;
}

void initializeEnemyTrainer(uint8_t level) {
	enemy.name = "Trainer";
	enemy.pokemonLength = 0;
	enemy.pokemon[enemy.pokemonLength++] = new_Pokemon(ID_CHARMANDER);
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

void draw_PokemonMenu(int8_t menuIndex, int8_t nokiaMenuIndex) {
	NokiaLCD_Clear();
	if (nokiaMenuIndex < 0 || nokiaMenuIndex >= player.pokemonLength) {
		NokiaLCD_WriteString("ERROR: nokiaMenuIndex out of bounds");
	}
	uint8_t m0 = player.pokemon[nokiaMenuIndex].moveIds[0];
	uint8_t m1 = player.pokemon[nokiaMenuIndex].moveIds[1];
	uint8_t m2 = player.pokemon[nokiaMenuIndex].moveIds[2];
	uint8_t m3 = player.pokemon[nokiaMenuIndex].moveIds[3];
	
	NokiaLCD_WriteString(player.pokemon[nokiaMenuIndex].name);
	switch (menuIndex) {
		case MI_TopLeft:
			NokiaLCD_SetLine(3, 0);
			NokiaLCD_WriteString(moveList[m0].name);
			NokiaLCD_SetLine(4, 0);
			NokiaLCD_WriteString(moveList[m1].name);
			NokiaLCD_SetLine(5, 0);
			NokiaLCD_WriteString(moveList[m2].name);
			NokiaLCD_SetLine(6, 0);
			NokiaLCD_WriteString(moveList[m3].name);
			break;
		case MI_TopRight:
			NokiaLCD_SetLine(3, 0);
			NokiaLCD_WriteString("Location: ");
			NokiaLCD_SetLine(4,0);
			NokiaLCD_WriteString("  Main");
			break;
		default:
			break;
	}
	drawPokemonSprite(&player, nokiaMenuIndex, 0);
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

void update_SettingsMenu(uint8_t menuIndex, int8_t direction) {
	switch (menuIndex) {
		case MI_TopLeft:
			gameDifficulty += direction;
			if (gameDifficulty > 3) gameDifficulty = 1;
			else if (gameDifficulty < 1) gameDifficulty = 3;
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

void restoreTrainer(Trainer *trainer) {
	// TODO: Heal all pokemon
	uint8_t i;
	for (i = 0; i < trainer->pokemonLength; i++) {
		trainer->pokemon[i].health = trainer->pokemon[i].healthMax;
		trainer->pokemon[i].isKnockedOut = 0;
		trainer->pokemon[i].effect = 0;
		trainer->pokemon[i].effectTime = 0;
		trainer->pokemon[i].status = 0;
		trainer->pokemon[i].statusTime = 0;
	}
}

void init_BattleIntro(long displayTime) {
	textDisplayTimer = displayTime;
	uint8_t enemyLevel = 1;
	initializeEnemyTrainer(enemyLevel);
	restoreTrainer(&player);
	clearMoveResult();
	
	LCD_ClearScreen();
	LCD_DisplayString(1, "A wild ");
	LCD_DisplayString(8, getPokemonName(&enemy));
	LCD_DisplayString(17, "appears!");
	LCD_Cursor(32);
	
	NokiaLCD_Clear();
}

void init_Battle_EnemyTurn(long displayTime) {
	textDisplayTimer = displayTime;
	LCD_ClearScreen();
	LCD_DisplayString(1, "Enemy Turn");
}

void draw_Battle_ActionMenu(uint8_t menuIndex) {
	drawMenuCursor(menuIndex, 4);
}

void drawDebugger() {
	char * str[3];
	itoa(player.pokemon[player.activeIndex].health, str, 10);
	
	NokiaLCD_Clear();
	NokiaLCD_WriteString("DEBUGGER");
	NokiaLCD_SetLine(2,0);
	NokiaLCD_WriteString("HP P ");
	NokiaLCD_WriteString(str);
	NokiaLCD_WriteString(" E ");
	itoa(enemy.pokemon[enemy.activeIndex].health, str, 10);
	NokiaLCD_WriteString(str);
	NokiaLCD_SetLine(3, 0);
	NokiaLCD_WriteString("Dead P ");
	itoa(moveResult.isPlayerDead, str, 10);
	NokiaLCD_WriteString(str);
	NokiaLCD_WriteString(" E ");
	itoa(moveResult.isEnemyDead, str, 10);
	NokiaLCD_WriteString(str);
}

void init_Battle_ActionMenu(uint8_t menuIndex) {
	LCD_ClearScreen();
	LCD_DisplayMenu4("Fight", "Pokedex", "Switch", "Escape");
	
	NokiaLCD_Clear();
	drawPokemonBattleUI(&enemy, 0);
	drawPokemonSprite(&enemy, enemy.activeIndex, 0);
	drawPokemonBattleUI(&player, 1);
	drawPokemonSprite(&player, player.activeIndex, 1);
	//drawDebugger();
	
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

void init_Battle_MoveUsed(long displayTime) {
	textDisplayTimer = displayTime;
	
	LCD_ClearScreen();
	char * pokemonName = moveResult.attacker->pokemon[moveResult.attacker->activeIndex].name;
	if (!moveResult.attacker->isPlayer) {
		LCD_DisplayString(1, "Enemy ");
		LCD_DisplayString(7, pokemonName);
	} else {
		LCD_DisplayString(1, pokemonName);
	}
	//LCD_DisplayString(1+strlen(pokemonName), " used");
	LCD_DisplayString(17, "used ");
	LCD_DisplayString(22, moveResult.moveName);
	LCD_DisplayString(22+strlen(moveResult.moveName), "!");
	LCD_Cursor(32);
	
	NokiaLCD_Clear();
	drawPokemonBattleUI(&enemy, 0);
	drawPokemonSprite(&enemy, enemy.activeIndex, 0);
	drawPokemonBattleUI(&player, 1);
	drawPokemonSprite(&player, player.activeIndex, 1);
}

void init_Battle_MoveMiss(long displayTime) {
	textDisplayTimer = displayTime;
	
	LCD_ClearScreen();
	LCD_DisplayString(1, "But it missed!");
	LCD_Cursor(32);
}

void init_Battle_MoveEffective(long displayTime) {
	textDisplayTimer = displayTime;
	
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
	textDisplayTimer = displayTime;
	
	LCD_ClearScreen();
	LCD_DisplayString(1, "A critical hit!");
	LCD_Cursor(32);
}

void init_Battle_MoveEffect(long displayTime) {
	textDisplayTimer = displayTime;
	
	LCD_ClearScreen();
	// TODO: Distinguish between offensive and defensive effects
	char * pokemonName = moveResult.defender->pokemon[moveResult.defender->activeIndex].name;
	char * effectText = moveList[moveResult.moveIndex].effectDescription;
	LCD_DisplayString(1, pokemonName);
	LCD_DisplayString(17, effectText);
	LCD_Cursor(32);
}

void init_Battle_TrainerVictory(long displayTime) {
	textDisplayTimer = displayTime;
	
	LCD_ClearScreen();
	LCD_DisplayString(1, "You won!");
	LCD_Cursor(32);
	
	NokiaLCD_Clear();
	drawPokemonBattleUI(&player, 1);
	drawPokemonSprite(&player, player.activeIndex, 1);
}

void init_Battle_TrainerDefeat(long displayTime) {
	textDisplayTimer = displayTime;
	
	LCD_ClearScreen();
	LCD_DisplayString(1, "You lost!");
	LCD_Cursor(32);
	
	NokiaLCD_Clear();
	drawPokemonBattleUI(&enemy, 0);
	drawPokemonSprite(&enemy, enemy.activeIndex, 0);
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
	pressedUp = 0;
	pressedDown = 0;
}

int8_t constraint(int8_t index, int8_t length) {
	if (index < 0) return length + index;
	if (index > length - 1) return index - length;
	return index;
}

uint8_t generateMoveIndex(Trainer *attacker, Trainer *defender) {
	uint8_t moveIndex = rand() % 4;
	return moveIndex;
}

void nokiaDebug(uint8_t state) {
	NokiaLCD_SetLine(1, 0);
	
	char str[1];
	
	NokiaLCD_WriteString("Dead P ");
	itoa(moveResult.isPlayerDead, str, 10);
	NokiaLCD_WriteString(str);
	NokiaLCD_WriteString(" E ");
	itoa(moveResult.isEnemyDead, str, 10);
	NokiaLCD_WriteString(str);
}

void nokiaDebug_OLD(uint8_t state) {
	NokiaLCD_SetLine(1, 0);
	switch (state) {
		case SCENE_Start: NokiaLCD_WriteString("Start "); break;
		case SCENE_MainMenu: NokiaLCD_WriteString("MainMenu "); break;
		case SCENE_PokemonMenu: NokiaLCD_WriteString("PokemonMenu "); break;
		case SCENE_SettingsMenu: NokiaLCD_WriteString("SettingsMenu "); break;
		case SCENE_Battle_Intro: NokiaLCD_WriteString("BattleIntro "); break;
		case SCENE_Battle_TurnStart: NokiaLCD_WriteString("TurnStart "); break;
		case SCENE_Battle_PlayerTurn: NokiaLCD_WriteString("PlayerTurn "); break;
		case SCENE_Battle_EnemyTurn: NokiaLCD_WriteString("EnemyTurn "); break;
		case SCENE_Battle_ActionMenu: NokiaLCD_WriteString("ActionMenu "); break;
		case SCENE_Battle_MoveMenu: NokiaLCD_WriteString("MoveMenu "); break;
		case SCENE_Battle_MoveUsed: NokiaLCD_WriteString("MoveUsed "); break;
		case SCENE_Battle_MoveMiss: NokiaLCD_WriteString("MoveMiss "); break;
		case SCENE_Battle_MoveEffective: NokiaLCD_WriteString("MoveEffective "); break;
		case SCENE_Battle_MoveCritical: NokiaLCD_WriteString("MoveCritical "); break;
		case SCENE_Battle_MoveEffect: NokiaLCD_WriteString("MoveEffect "); break;
		case SCENE_Battle_MoveResolution: NokiaLCD_WriteString("MoveResolution "); break;
		case SCENE_Battle_TrainerVictory: NokiaLCD_WriteString("TrainerVictory "); break;
		case SCENE_Battle_TrainerDefeat: NokiaLCD_WriteString("TrainerDefeat "); break;
		case SCENE_Error: NokiaLCD_WriteString("Scene:Error "); break;
		default: NokiaLCD_WriteString("SCN: ??? "); break;
	}
	NokiaLCD_SetLine(2, 0);
	NokiaLCD_WriteString("Time: ");
	if (textDisplayTimer >= TIME_3SEC) {
		NokiaLCD_WriteString("> 3sec ");
	} else if (textDisplayTimer >= TIME_2SEC) {
		NokiaLCD_WriteString("> 2sec ");
	} else if (textDisplayTimer >= TIME_1SEC) {
		NokiaLCD_WriteString("> 1sec ");
	} else if (textDisplayTimer >= 500) {
		NokiaLCD_WriteString(">.5sec ");
	} else if (textDisplayTimer > 0) {
		NokiaLCD_WriteString(">.1sec ");
	} else {
		NokiaLCD_WriteString(" 0 sec ");
	}
}

int Scene_Tick(int state) {
	static int8_t menuIndex = 0;
	static int8_t nokiaMenuIndex = 0;
	static uint8_t playerTurn = 1;
	uint8_t textSceneOver = textDisplayTimer <= 0 || pressedB;
	static uint8_t xscroll = 0;
	static uint8_t yscroll = 0;
	static uint8_t tickCounter = 0;
	static uint8_t roundCounter = 0;
	static uint8_t turnCounter = 0;
	tickCounter++;
	
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
						xscroll = 0;
						yscroll = 0;
						playerTurn = 1;
						break;
					case MI_TopRight:
						state = SCENE_Battle_Intro;
						init_BattleIntro(TIME_3SEC);
						xscroll = 0;
						yscroll = 0;
						playerTurn = 1;
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
			if (textSceneOver) {
				clearButtons();
				state = SCENE_Battle_RoundStart;
				roundCounter = 0;
				turnCounter = 0;
			} else {
				state = SCENE_Battle_Intro;
			}
			break;
		case SCENE_Battle_RoundStart:
			roundCounter++;
			playerTurn = getAgility(&player) >= getAgility(&enemy);
			state = SCENE_Battle_TurnStart;
			break;
		case SCENE_Battle_TurnStart:
			turnCounter++;
			if (turnCounter%3 == 0) {
				state = SCENE_Battle_RoundStart;
			} else if (playerTurn) {
				state = SCENE_Battle_PlayerTurn;
				playerTurn = !playerTurn;
			} else {
				state = SCENE_Battle_EnemyTurn;
				init_Battle_EnemyTurn(TIME_3SEC);
				playerTurn = !playerTurn;
			}
			break;
		case SCENE_Battle_PlayerTurn:
			state = SCENE_Battle_ActionMenu;
			menuIndex = MI_TopLeft;
			init_Battle_ActionMenu(menuIndex);
			break;
		case SCENE_Battle_EnemyTurn:
			if (textSceneOver) {
				clearButtons();
				generateMoveResult(generateMoveIndex(&enemy, &player), &enemy, &player);
				state = SCENE_Battle_MoveUsed;
				init_Battle_MoveUsed(TIME_3SEC);
			} else {
				state = SCENE_Battle_EnemyTurn;
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
				generateMoveResult(player.pokemon[player.activeIndex].moveIds[menuIndex], &player, &enemy);
				state = SCENE_Battle_MoveUsed;
				init_Battle_MoveUsed(TIME_3SEC);
			} else if (pressedX) {
				clearButtons();
				state = SCENE_Battle_ActionMenu;
				menuIndex = MI_TopLeft;
				init_Battle_ActionMenu(menuIndex);
			} else if (pressedLeft) {
				clearButtons();
				state = SCENE_Battle_MoveMenu;
				menuIndex = constraint(menuIndex-1, 4);
				draw_Battle_MoveMenu(menuIndex);
			} else if (pressedRight) {
				clearButtons();
				state = SCENE_Battle_MoveMenu;
				menuIndex = constraint(menuIndex+1, 4);
				draw_Battle_MoveMenu(menuIndex);
			} else if (pressedUp) {
				clearButtons();
				state = SCENE_Battle_MoveMenu;
				menuIndex = constraint(menuIndex-2, 4);
				draw_Battle_MoveMenu(menuIndex);
			} else if (pressedDown) {
				clearButtons();
				state = SCENE_Battle_MoveMenu;
				menuIndex = constraint(menuIndex+2, 4);
				draw_Battle_MoveMenu(menuIndex);
			} else {
				state = SCENE_Battle_MoveMenu;
			}
			break;
		case SCENE_Battle_MoveUsed:
			if (textSceneOver) {
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
			if (textSceneOver) {
				clearButtons();
				state = SCENE_Battle_MoveResolution;
			} else {
				state = SCENE_Battle_MoveMiss;
			}
			break;
		case SCENE_Battle_MoveEffective:
			if (textSceneOver) {
				clearButtons();
				if (moveResult.isCritical) {
					state = SCENE_Battle_MoveCritical;
					init_Battle_MoveCritical(TIME_3SEC);
				} else if (moveResult.isEffectHit) {
					state = SCENE_Battle_MoveEffect;
					init_Battle_MoveEffect(TIME_3SEC);
				} else {
					state = SCENE_Battle_MoveResolution;
				}
			} else {
				state = SCENE_Battle_MoveEffective;
			}
			break;
		case SCENE_Battle_MoveCritical:
			if (textSceneOver) {
				clearButtons();
				if (moveResult.isEffectHit) {
					state = SCENE_Battle_MoveEffect;
					init_Battle_MoveEffect(TIME_3SEC);
				} else {
					state = SCENE_Battle_MoveResolution;
				}
			} else {
				state = SCENE_Battle_MoveCritical;
			}
			break;
		case SCENE_Battle_MoveEffect:
			if (textSceneOver) {
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
				state = SCENE_Battle_TurnStart;
			}
			break;
		case SCENE_Battle_TrainerVictory:
			if (textSceneOver) {
				clearButtons();
				state = SCENE_MainMenu;
				menuIndex = MI_TopRight;
				init_MainMenu(menuIndex);
			} else {
				state = SCENE_Battle_TrainerVictory;
			}
			break;
		case SCENE_Battle_TrainerDefeat:
			if (textSceneOver) {
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
				update_SettingsMenu(menuIndex, 1);
			} else if (pressedUp) {
				clearButtons();
				update_SettingsMenu(menuIndex, 1);
			} else if (pressedDown) {
				clearButtons();
				update_SettingsMenu(menuIndex, -1);
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
	
	//textDisplayTimer -= scenePeriod;
	reduceHealthLost(&player, 2);
	reduceHealthLost(&enemy, 2);
	
	switch (state) {
		case SCENE_Start:
		case SCENE_MainMenu:
		case SCENE_PokemonMenu:
		case SCENE_SettingsMenu:
		case SCENE_Battle_ActionMenu:
		case SCENE_Battle_MoveMenu:
			break;
		case SCENE_Battle_Intro:
			{
			// Scrolls sprites into the scene
			xscroll += (tickCounter%2==0 && xscroll < 24) ? 2 : 0;
			int8_t playerX = -24 + xscroll;
			int8_t playerY = 48-1-20;
			NokiaLCD_CustomBitmap(bitmaps[player.pokemon[player.activeIndex].spriteFrontIndex], playerX, playerY, 1);
			int8_t enemyX = 86-1 - xscroll;
			int8_t enemyY = 0;
			NokiaLCD_CustomBitmap(bitmaps[enemy.pokemon[enemy.activeIndex].spriteFrontIndex], enemyX, enemyY, 0);
			}
			break;
		case SCENE_Battle_RoundStart:
		case SCENE_Battle_TurnStart:
		case SCENE_Battle_PlayerTurn:
		case SCENE_Battle_EnemyTurn:
		case SCENE_Battle_MoveUsed:
		case SCENE_Battle_MoveMiss:
		case SCENE_Battle_MoveEffective:
		case SCENE_Battle_MoveCritical:
		case SCENE_Battle_MoveEffect:
		case SCENE_Battle_MoveResolution:
		case SCENE_Battle_TrainerDefeat:
		case SCENE_Battle_TrainerVictory:
			NokiaLCD_Clear();
			drawPokemonBattleUI(&player, 1);
			drawPokemonBattleUI(&enemy, 0);
			drawPokemonSprite(&player, player.activeIndex, 1);
			drawPokemonSprite(&enemy, enemy.activeIndex, 0);
			break;
		case SCENE_Error:
		default:
			break;
	}
	
	//nokiaDebug(state);
	
	return state;
};

enum SNES_B_States {SNES_B_Released, SNES_B_Pressed} SNES_B_State;
int SNES_B_Tick(int state) {
	switch(state) {
		case SNES_B_Released:
			if (SNES_B/* && textDisplayTimer <= 0*/) {
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
			if (SNES_X/* && textDisplayTimer <= 0*/) {
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
			if (SNES_LEFT/* && textDisplayTimer <= 0*/) {
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
			if (SNES_RIGHT/* && textDisplayTimer <= 0*/) {
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

enum SNES_DOWN_States {SNES_DOWN_Released, SNES_DOWN_Pressed} SNES_DOWN_State;
int SNES_DOWN_Tick(int state) {
	switch (state) {
		case SNES_DOWN_Released:
			if (SNES_DOWN) {
				state = SNES_DOWN_Pressed;
				pressedDown = 1;
			} else {
				state = SNES_DOWN_Released;
			}
			break;
		case SNES_DOWN_Pressed:
			state = (SNES_DOWN) ? SNES_DOWN_Pressed : SNES_DOWN_Released;
			break;
	}
	return state;
}

enum SNES_UP_States {SNES_UP_Released, SNES_UP_Pressed} SNES_UP_State;
int SNES_UP_Tick(int state) {
	switch (state) {
		case SNES_UP_Released:
			if (SNES_UP) {
				state = SNES_UP_Pressed;
				pressedUp = 1;
				} else {
				state = SNES_UP_Released;
			}
			break;
		case SNES_UP_Pressed:
			state = (SNES_UP) ? SNES_UP_Pressed : SNES_UP_Released;
			break;
	}
	return state;
}

int SNES_L_Tick(int state) {
	switch(state) {
		case SNES_L_Released:
			if (SNES_L/* && textDisplayTimer <= 0*/) {
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
			if (SNES_R/* && textDisplayTimer <= 0*/) {
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