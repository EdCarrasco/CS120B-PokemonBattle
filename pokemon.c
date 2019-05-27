/*
 * Pokemon.c
 *
 * Created: 5/26/2019 12:04:52 PM
 *  Author: Edward-Laptop
 */ 

#include <stdint.h>
#include "definitions.h"
//#include "sprites.h"
#include "pokemon.h"
#include "move.h"

Pokemon new_Pokemon(uint8_t pokemonId) {
	switch(pokemonId) {
		case ID_BULBASAUR:
			return Pokemon_Bulbasaur();
		case ID_SQUIRTLE:
			return Pokemon_Squirtle();
		case ID_CHARMANDER:
			return Pokemon_Charmander();
	}
}

Pokemon Pokemon_Bulbasaur() {
	Pokemon pokemon;
	pokemon.spriteFrontIndex = BITMAP_BULBASAUR_FRONT;
	pokemon.spriteBackIndex = BITMAP_BULBASAUR_FRONT;
	pokemon.name = STR_BULBASAUR;
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
	
	pokemon.moves[0] = Move_Tackle();
	pokemon.moves[1] = Move_Ember();
	pokemon.moves[2] = Move_Bubble();
	pokemon.moves[3] = Move_VineWhip();
	
	pokemon.moveIds[0] = ID_TACKLE;
	pokemon.moveIds[1] = ID_VINEWHIP;
	pokemon.moveIds[2] = ID_BUBBLE;
	pokemon.moveIds[3] = ID_EMBER;
	
	return pokemon;
}

Pokemon Pokemon_Squirtle() {
	Pokemon pokemon;
	pokemon.spriteFrontIndex = BITMAP_BULBASAUR_FRONT;
	pokemon.spriteBackIndex = BITMAP_BULBASAUR_FRONT;
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
	
	pokemon.moves[0] = Move_Tackle();
	pokemon.moves[1] = Move_Ember();
	pokemon.moves[2] = Move_Bubble();
	pokemon.moves[3] = Move_VineWhip();
	
	pokemon.moveIds[0] = ID_TACKLE;
	pokemon.moveIds[1] = ID_VINEWHIP;
	pokemon.moveIds[2] = ID_BUBBLE;
	pokemon.moveIds[3] = ID_EMBER;
	return pokemon;
}

Pokemon Pokemon_Charmander() {
	Pokemon pokemon;
	pokemon.spriteFrontIndex = BITMAP_CHARMANDER_FRONT;
	pokemon.spriteBackIndex = BITMAP_CHARMANDER_FRONT;
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
	
	pokemon.moves[0] = Move_Tackle();
	pokemon.moves[1] = Move_Ember();
	pokemon.moves[2] = Move_Bubble();
	pokemon.moves[3] = Move_VineWhip();
	
	pokemon.moveIds[0] = ID_TACKLE;
	pokemon.moveIds[1] = ID_VINEWHIP;
	pokemon.moveIds[2] = ID_BUBBLE;
	pokemon.moveIds[3] = ID_EMBER;
	return pokemon;
}