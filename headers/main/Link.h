#ifndef LINK_HEADER
#define LINK_HEADER set

#include <gb/gb.h>
#include "objects.h"

extern int16_t linkSwordX,linkSwordY,nextX,nextY,trueNextX,trueNextY,linkSwordTrueX,linkSwordTrueY;
extern int8_t linkSword;
extern uint8_t linkMaxHealth;


void FinishLinkUpdate(uint8_t sprite) BANKED;

#endif