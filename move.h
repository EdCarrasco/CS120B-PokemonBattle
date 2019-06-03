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

enum MOVE_IDS {ID_TACKLE, ID_VINEWHIP, ID_BUBBLE, ID_EMBER, ID_BITE};

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

Move Move_Tackle();
Move Move_VineWhip();
Move Move_Bubble();
Move Move_Ember();
Move Move_Bite();
Move Move_Growl();

Buff Buff_AttackLow();
Buff Buff_AgilityHigh();
Buff Buff_Burn();

#endif /* MOVE_H_ */