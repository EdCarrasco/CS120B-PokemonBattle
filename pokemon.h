/*
 * pokemon.h
 *
 * Created: 5/20/2019 1:56:12 AM
 *  Author: Edward-Laptop
 */ 

#ifndef POKEMON_H_
#define POKEMON_H_

#include "definitions.h"
#include "move.h"

typedef struct _Pokemon {
	char* name;
	uint8_t spriteFrontIndex;
	uint8_t spriteBackIndex;
	uint8_t type;
	uint8_t level;
	int8_t health;
	int8_t healthMax;
	uint8_t status;
	uint8_t statusTime;
	uint8_t effect;
	uint8_t effectTime;
	int8_t attack;
	int8_t defense;
	int8_t agility;
	uint8_t moveIds[4];
	uint8_t isKnockedOut;
} Pokemon;

enum POKEMON_IDS {ID_BULBASAUR, ID_SQUIRTLE, ID_CHARMANDER};
	
Pokemon* new_Pokemon(uint8_t pokemonId);
Pokemon* Pokemon_Bulbasaur();
Pokemon* Pokemon_Squirtle();
Pokemon* Pokemon_Charmander();

#endif /* POKEMON_H_ */