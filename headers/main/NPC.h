#ifndef NPC_HEADER
#define NPC_HEADER set
#include <gb/gb.h>

typedef struct NPC{

    uint16_t x,y;
    uint8_t active;
    uint8_t sprite;
    uint8_t direction;
    uint8_t index;

    uint8_t dir;
    uint8_t motionType;
    uint8_t tileStart;

}NPC;


#define MAX_NUMBER_NPCS 3


void UpdateAllNPCs();
void ActivateNPC(NPC *npc,uint8_t x, uint8_t y);

    
#endif
