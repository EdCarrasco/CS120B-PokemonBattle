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

enum MOVE_IDS {ID_TACKLE, ID_VINEWHIP, ID_BUBBLE, ID_EMBER};

typedef struct _Move {
    char* name;
	char* description;
    uint8_t type;
    uint8_t baseDamage;
    uint8_t effect;
    uint8_t hitChance;
    uint8_t criticalChance;
    uint8_t effectChance;
} Move;

Move Move_Tackle();
Move Move_VineWhip();
Move Move_Bubble();
Move Move_Ember();

#endif /* MOVE_H_ */