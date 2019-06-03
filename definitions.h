/*
 * definitions.h
 *
 * Created: 5/20/2019 3:07:49 AM
 *  Author: Edward-Laptop
 */ 


#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define BITMAP_BULBASAUR_FRONT		0
#define BITMAP_SQUIRTLE_FRONT		1
#define BITMAP_CHARMANDER_FRONT		2

#define STR_BULBASAUR	"Bulbasaur"
#define STR_SQUIRTLE	"Squirtle"
#define STR_CHARMANDER	"Charmander"

#define STR_TACKLE		"Tackle"
#define STR_BITE		"Bite"

#define STR_VINEWHIP	"VineWhip"
#define STR_BUBBLE		"Bubble"
#define STR_EMBER		"Ember"

#define STR_SLEEPPOWDER	"SleepPowder"
#define STR_WITHDRAW	"Withdraw"
#define STR_SMOKESCREEN "SmokeScreen"

#define STR_RAZORLEAF	"RazorLeaf"
#define STR_WATERGUN	"WaterGun"
#define STR_FLAMETHROW	"FlameThrower"

#define STR_LEECHSEED	"LeechSeed"
#define STR_TAILWHIP	"TailWhip"
#define STR_SCARYFACE	"ScaryFace"

#define ATTACK_LOW		1
#define ATTACK_NORMAL	5
#define ATTACK_HIGH		10

#define DEFENSE_LOW		1
#define DEFENSE_NORMAL	5
#define DEFENSE_HIGH	10

#define SPEED_SLOW		8
#define SPEED_NORMAL	10
#define SPEED_FAST		12

#define HEALTH_LOW		80
#define HEALTH_NORMAL	90
#define HEALTH_HIGH		99

#define DAMAGE_LOWEST	1
#define DAMAGE_UBERLOW	5
#define DAMAGE_VERYLOW	10
#define DAMAGE_LOW		15
#define DAMAGE_NORMAL	20
#define DAMAGE_HIGH		25
#define DAMAGE_VERYHIGH	35
#define DAMAGE_UBERHIGH	50
#define DAMAGE_KILL		200

#define HIT_VERYLOW		50
#define HIT_LOW			75
#define HIT_NORMAL		90
#define HIT_HIGH		95
#define HIT_ALWAYS		100

#define CRIT_VERYLOW	1
#define CRIT_LOW		5
#define CRIT_NORMAL		10
#define CRIT_HIGH		15
#define CRIT_VERYHIGH	20

#define EFF_HIT_LOW			20
#define EFF_HIT_NORMAL		50
#define EFF_HIT_HIGH		80
#define EFF_HIT_ALWAYS		100

enum Pokemon_Types {
    T_NORMAL,
    T_FIRE,
    T_WATER,
    T_GRASS,
    T_ELECTRIC
};

enum Pokemon_Status {
    ST_NONE,
    ST_BURN,
    ST_PARALYSIS,
    ST_FREEZE,
    ST_POISON,
    ST_CONFUSION,
    ST_SLEEP
};

enum Pokemon_Effects {
    E_NONE,
	E_ENEMY_BURN,
    E_SELF_ATTACK_HIGH,
    E_ENEMY_ATTACK_LOW,
    E_SELF_DEFENSE_HIGH,
    E_ENEMY_DEFENSE_LOW,
    E_SELF_ACCURACY_HIGH,
    E_ENEMY_ACCURACY_LOW,
    E_SELF_SPEED_HIGH,
    E_ENEMY_SPEED_LOW,
	E_FLINCH
};

#endif /* DEFINITIONS_H_ */