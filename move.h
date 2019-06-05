/*
 * move.h
 *
 * Created: 5/20/2019 3:10:54 AM
 *  Author: Edward-Laptop
 */ 

#ifndef MOVE_H_
#define MOVE_H_

#include <stdint.h>
#include "definitions.h"

enum MOVE_IDS {
	ID_TACKLE, ID_ABSORB, ID_RAZORLEAF, ID_SLEEPPOWDER, 
	ID_BITE, ID_BUBBLE, ID_WATERPUMP, ID_WITHDRAW, 
	ID_SCRATCH, ID_EMBER, ID_FIRESPIN, ID_GROWL
};

typedef struct _Move {
    char* name;
	char* description;
	char* effectDescription;
    uint8_t type;
    uint8_t baseDamage;
    uint8_t effect;
    uint8_t hitChance;
    uint8_t criticalChance;
    uint8_t effectChance;
} Move;

typedef struct _Buff {
	char* name;
	char* applyText;		// Text displayed when the move is used (on SCENE_Battle_MoveEffect)
	char* tickText;			// Text displayed if it has a ticking effect
	uint8_t tickScene;		// Determines when the tick effect happens. 0 = doesn't tick
	uint8_t type;			// Pokemon type
	int8_t chance;			// Probability 0-100 that the effect is applied to the target
	int8_t duration;		// Amount of rounds remaining before this Buff expires
	uint8_t durationMax;	// Amount of rounds this Buff start with when first applied
} Buff;

// Bulbasaur
Move Move_Tackle();			// NORMAL damage
Move Move_Absorb();			// Low GRASS damage. Heal self
Move Move_RazorLeaf();		// High GRASS damage
Move Move_SleepPowder();	// Target SLEEPS (1 turn)

// Squirtle
Move Move_Bite();			// DARK damage
Move Move_Bubble();			// Low WATER damage. SLOW target (4 turns)
Move Move_WaterPump();		// High WATER damage
Move Move_Withdraw();		// Increase self's DEFENSE (4 turns)

// Charmander
Move Move_Scratch();		// NORMAL damage
Move Move_Ember();			// Low FIRE damage. BURN target (4 turns, deal low FIRE damage at start of turn)
Move Move_FireSpin();		// High FIRE damage
Move Move_Growl();			// Reduce target's ATTACK (4 turns)

// Pikachu
/*Move Move_IronTail();		// STEEL damage
Move Move_Spark();			// Low ELECTRIC damage. PARALYZE target (4 turns, chance to skip turn)
Move Move_Thunder();		// High ELECTRIC damage
Move Move_Charge();			// Increase self ATTACK (1 turn). Remove negative effects
*/
Buff Buff_AttackLow();
Buff Buff_AgilityHigh();
Buff Buff_Burn();

#endif /* MOVE_H_ */