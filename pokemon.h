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
	uint8_t type;
	uint8_t level;
	uint8_t healthMax;
	uint8_t health;
	uint8_t status;
	uint8_t statusTime;
	uint8_t effect;
	uint8_t effectTime;
	uint8_t attack;
	uint8_t defense;
	uint8_t agility;
	Move move1;
	Move move2;
	Move move3;
	Move move4;
} Pokemon;

Pokemon Pokemon_Squirtle() {
	Pokemon pokemon;
	pokemon.name = STR_SQUIRTLE;
	pokemon.type = T_WATER;
	pokemon.level = 1;
	pokemon.healthMax = HEALTH_NORMAL;
	pokemon.healthMax = HEALTH_NORMAL;
	pokemon.status = ST_NONE;
	pokemon.statusTime = 0;
	pokemon.effect = E_NONE;
	pokemon.effectTime = 0;
	pokemon.attack = ATTACK_NORMAL;
	pokemon.defense = DEFENSE_NORMAL;
	pokemon.agility = SPEED_NORMAL;
	pokemon.move1 = Move_Tackle();
	pokemon.move2 = Move_Ember();
	pokemon.move3 = Move_Bubble();
	pokemon.move4 = Move_VineWhip();
	return pokemon;
}

Pokemon Pokemon_Charmander() {
    Pokemon pokemon;
    pokemon.name = STR_CHARMANDER;
    pokemon.type = T_FIRE;
    pokemon.level = 1;
    pokemon.healthMax = HEALTH_NORMAL;
    pokemon.healthMax = HEALTH_NORMAL;
    pokemon.status = ST_NONE;
    pokemon.statusTime = 0;
    pokemon.effect = E_NONE;
    pokemon.effectTime = 0;
    pokemon.attack = ATTACK_NORMAL;
    pokemon.defense = DEFENSE_NORMAL;
    pokemon.agility = SPEED_NORMAL;
    pokemon.move1 = Move_Tackle();
    pokemon.move2 = Move_Ember();
    pokemon.move3 = Move_Bubble();
    pokemon.move4 = Move_VineWhip();
    return pokemon;
}

/*void pokemonInit(Pokemon *pokemon, char *name, uint8_t type, uint8_t health) {
	pokemon->name = name;
	pokemon->type = type;
	pokemon->healthMax = health;
	pokemon->health = health;
	pokemon->status = ST_NONE;
	pokemon->effect = E_NONE;
	pokemon->agility = AGILITY_NORMAL;

	pokemon->move1 = Move_Tackle();
}*/

#endif /* POKEMON_H_ */