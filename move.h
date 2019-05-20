/*
 * move.h
 *
 * Created: 5/20/2019 3:10:54 AM
 *  Author: Edward-Laptop
 */ 

#ifndef MOVE_H_
#define MOVE_H_

#include "definitions.h"

typedef struct _Move {
    char* name;
    uint8_t type;
    uint8_t baseDamage;
    uint8_t effect;
    uint8_t hitChance;
    uint8_t criticalChance;
    uint8_t effectChance;
} Move;

Move Move_Tackle() {
    Move move;
    move.name = STR_TACKLE;
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
	move.type = T_WATER;
	move.baseDamage = DAMAGE_LOW;
	move.effect = E_ENEMY_SPEED_LOW;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = EFF_NORMAL;
	return move;
}

Move Move_Ember() {
	Move move;
	move.name = STR_EMBER;
	move.type = T_FIRE;
	move.baseDamage = DAMAGE_LOW;
	move.effect = E_ENEMY_BURN;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = EFF_NORMAL;
	return move;
}

#endif /* MOVE_H_ */