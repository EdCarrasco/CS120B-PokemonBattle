/*
 * move.c
 *
 * Created: 5/26/2019 12:12:43 PM
 *  Author: Edward-Laptop
 */ 

#include <stdint.h>
#include "move.h"

Move Move_Tackle() {
	Move move;
	move.name = STR_TACKLE;
	move.description = "Deals NORMAL  damage.";
	move.type = T_NORMAL;
	move.baseDamage = DAMAGE_NORMAL;
	move.effect = E_NONE;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = 0;
	return move;
}

Move Move_VineWhip() {
	Move move;
	move.name = STR_VINEWHIP;
	move.description = "Deals high    GRASS damage.";
	move.type = T_GRASS;
	move.baseDamage = DAMAGE_NORMAL;
	move.effect = E_NONE;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = 0;
	return move;
}

Move Move_Bubble() {
	Move move;
	move.name = STR_BUBBLE;
	move.description = "Deals low     WATER damage  and SLOWS foe.";
	move.type = T_WATER;
	move.baseDamage = DAMAGE_LOW;
	move.effect = E_ENEMY_SPEED_LOW;
	move.hitChance = HIT_ALWAYS;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = EFF_HIT_NORMAL;
	return move;
}

Move Move_Ember() {
	Move move;
	move.name = STR_EMBER;
	move.description = "Deals low FIREdamage. High  chance to BURNfoe.";
	move.type = T_FIRE;
	move.baseDamage = DAMAGE_LOW;
	move.effect = E_ENEMY_BURN;
	move.hitChance = HIT_HIGH;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = EFF_HIT_NORMAL;
	return move;
}