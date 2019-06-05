/*
 * move.c
 *
 * Created: 5/26/2019 12:12:43 PM
 *  Author: Edward-Laptop
 */ 

#include <stdint.h>
#include "move.h"

#define STR_TACKLE_DESCRIPTION		"Deals NORMAL  damage."
#define STR_BITE_DESCRIPTION		"Deals DARK    damage."
#define STR_SCRATCH_DESCRIPTION		"Deals NORMAL  damage."
#define STR_IRONTAIL_DESCRIPTION	"Deals STEEL   damage."
									 //	3		   //	4		 //	5		   //	6		   //
#define STR_ABSORB_DESCRIPTION		"Deals low     GRASS damage. Restore some  health."
#define STR_BUBBLE_DESCRIPTION		"Deals low     WATER damage. Slows the foe."
#define STR_EMBER_DESCRIPTION		"Deals low     FIRE damage.  Burns the foe."
#define STR_SPARK_DESCRIPTION		"Deals low     ELECTRIC      damage. Chanceto PARALYZE foe."
									 //	3		   //	4		 //	5		   //	6		   //
#define STR_RAZORLEAF_DESCRIPTION	"Deals high    GRASS damage."
#define STR_WATERPUMP_DESCRIPTION	"Deals high    WATER damage."
#define STR_FIRESPIN_DESCRIPTION	"Deals high    FIRE damage."
#define STR_ELECTRIC_DESCRIPTION	"Deals high    ELECTRIC      damage."
									 //	3		   //	4		 //	5		   //	6		   //
#define STR_SLEEPPOWDER_DESCRIPTION	"Puts the foe  to SLEEP for  1 turn."
#define STR_WITHDRAW_DESCRIPTION	"Raises your   DEFENSE for 4 turns."
#define STR_GROWL_DESCRIPTION		"Lowers foe's  ATTACK for 4  turns."
#define STR_CHARGE_DESCRIPTION		"Sharply raisesyour ATTACK   for 1 turn."


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
	move.description = STR_TACKLE_DESCRIPTION;
	move.effectDescription = "ERROR";
	move.type = T_NORMAL;
	move.baseDamage = DAMAGE_NORMAL;
	move.effect = E_NONE;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = 0;
	return move;
}

Move Move_Absorb() {
	Move move;
	move.name = "Absorb";
	move.description = STR_ABSORB_DESCRIPTION;
	move.effectDescription = "HP was drained!";
	move.type = T_NORMAL;
	move.baseDamage = DAMAGE_LOW;
	move.effect = E_NONE;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = EFF_HIT_ALWAYS;
	return move;
}

Move Move_RazorLeaf() {
	Move move;
	move.name = "RzrLeaf";
	move.description = STR_RAZORLEAF_DESCRIPTION;
	move.effectDescription = "-ERROR-";
	move.type = T_GRASS;
	move.baseDamage = DAMAGE_VERYHIGH;
	move.effect = E_NONE;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = 0;
	return move;
}

Move Move_SleepPowder() {
	Move move;
	move.name = "SlpPwdr";
	move.description = STR_SLEEPPOWDER_DESCRIPTION;
	move.effectDescription = "fell asleep!";
	move.type = T_GRASS;
	move.baseDamage = 0;
	move.effect = E_NONE;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = EFF_HIT_ALWAYS;
	return move;
}

Move Move_Bite() {
	Move move;
	move.name = "Bite";
	move.description = STR_BITE_DESCRIPTION;
	move.effectDescription = "flinched!";
	move.type = T_NORMAL; // TODO: Create DARK type and expand getEffectiveness function
	move.baseDamage = DAMAGE_NORMAL;
	move.effect = E_NONE;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = EFF_HIT_NORMAL;
	return move;
}

Move Move_Bubble() {
	Move move;
	move.name = "Bubble";
	move.description = STR_BUBBLE_DESCRIPTION;
	move.effectDescription = "was slowed!";
	move.type = T_WATER;
	move.baseDamage = DAMAGE_LOW;
	move.effect = E_ENEMY_SPEED_LOW;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = EFF_HIT_ALWAYS;
	return move;
}

Move Move_WaterPump() {
	Move move;
	move.name = "WatrPmp";
	move.description = STR_BUBBLE_DESCRIPTION;
	move.effectDescription = "-ERROR-";
	move.type = T_WATER;
	move.baseDamage = DAMAGE_HIGH;
	move.effect = E_NONE;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = 0;
	return move;
}

Move Move_Withdraw() {
	Move move;
	move.name = "Withdrw";
	move.description = STR_WITHDRAW_DESCRIPTION;
	move.effectDescription = "DEFENSE raised!";
	move.type = T_WATER;
	move.baseDamage = 0;
	move.effect = E_NONE;
	move.hitChance = HIT_ALWAYS;
	move.criticalChance = 0;
	move.effectChance = EFF_HIT_ALWAYS;
	return move;
}

Move Move_Scratch() {
	Move move;
	move.name = "Scratch";
	move.description = STR_SCRATCH_DESCRIPTION;
	move.effectDescription = "-ERROR-";
	move.type = T_NORMAL;
	move.baseDamage = DAMAGE_NORMAL;
	move.effect = E_NONE;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = 0;
	return move;
}

Move Move_Ember() {
	Move move;
	move.name = "Ember";
	move.description = STR_EMBER_DESCRIPTION;
	move.effectDescription = "was burned!";
	move.type = T_FIRE;
	move.baseDamage = DAMAGE_LOW;
	move.effect = E_ENEMY_BURN;
	move.hitChance = HIT_HIGH;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = EFF_HIT_NORMAL;
	return move;
}

Move Move_FireSpin() {
	Move move;
	move.name = "FireSpn";
	move.description = STR_FIRESPIN_DESCRIPTION;
	move.effectDescription = "-ERROR-";
	move.type = T_FIRE;
	move.baseDamage = DAMAGE_HIGH;
	move.effect = E_NONE;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = CRIT_NORMAL;
	move.effectChance = 0;
	return move;
}

Move Move_Growl() {
	Move move;
	move.name = "Growl";
	move.description = STR_GROWL_DESCRIPTION;
	move.effectDescription = "ATTACK fell!";
	move.type = T_NORMAL;
	move.baseDamage = 0;
	move.effect = E_ENEMY_ATTACK_LOW;
	move.hitChance = HIT_NORMAL;
	move.criticalChance = 0;
	move.effectChance = EFF_HIT_ALWAYS;
	return move;
}