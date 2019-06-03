/*
 * move.c
 *
 * Created: 5/26/2019 12:12:43 PM
 *  Author: Edward-Laptop
 */ 

#include <stdint.h>
#include "move.h"

/*
How to add a new move:
1. Create a new Move_NewMove() function in this file
2. Add ID_NEWMOVE to MOVE_IDS enum in move.h
3. Increase moveList array by 1 in tasks.h
4. Add new element to moveList ( moveList[ID_NEWMOVE] = Move_NewMove() ) in initializeMoves() in tasks.h
*/

Move Move_Tackle() {
	Move move;
	move.name = "Tackle";
	move.description = "Deals NORMAL  damage.";
	move.effectDescription = "ERROR";
	move.type = T_NORMAL;
	move.baseDamage = DAMAGE_NORMAL;
	move.effect = E_NONE;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = 100;//CRIT_NORMAL;
	move.effectChance = 0;
	return move;
}

Move Move_Bite() {
	Move move;
	move.name = "Bite";
	move.description = "Deals DARK    damage. Chanceto make foe   FLINCH.";
	move.effectDescription = "flinched!";
	move.type = T_NORMAL; // TODO: Create DARK type and expand getEffectiveness function
	move.baseDamage = DAMAGE_NORMAL;
	move.effect = E_NONE;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = EFF_HIT_NORMAL;
	return move;
}

Move Move_Growl() {
	Move move;
	move.name = "Growl";
	move.description = "Deals DARK    damage. Chanceto make foe   FLINCH.";
	move.effectDescription = "flinched!";
	move.type = T_NORMAL;
	move.baseDamage = 0;
	move.effect = E_ENEMY_ATTACK_LOW;
	move.hitChance = 100;
	move.criticalChance = 0;
	move.effectChance = 100;
	return move;
}

Move Move_VineWhip() {
	Move move;
	move.name = "VineWhip";
	move.description = "Deals high    GRASS damage.";
	move.effectDescription = "ERROR";
	move.type = T_GRASS;
	move.baseDamage = DAMAGE_VERYHIGH;
	move.effect = E_NONE;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = 0;
	return move;
}

Move Move_Bubble() {
	Move move;
	move.name = "Bubble";
	move.description = "Deals low     WATER damage  and SLOWS foe.";
	move.effectDescription = "was slowed!";
	move.type = T_WATER;
	move.baseDamage = DAMAGE_LOW;
	move.effect = E_ENEMY_SPEED_LOW;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = EFF_HIT_ALWAYS;
	return move;
}

Move Move_Ember() {
	Move move;
	move.name = "Ember";
	move.description = "Deals low FIREdamage. High  chance to BURNfoe.";
	move.effectDescription = "was burned!";
	move.type = T_FIRE;
	move.baseDamage = DAMAGE_LOW;
	move.effect = E_ENEMY_BURN;
	move.hitChance = HIT_HIGH;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = EFF_HIT_NORMAL;
	return move;
}