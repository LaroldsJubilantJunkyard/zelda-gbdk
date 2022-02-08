#include <gb/gb.h>
#include <gb/metasprites.h>
#include "common.h"
#include "HeadsUpDisplay.h"
#include "link.h"
#include "NPC.h"
#include "graphics/NPCMarin.h"

NPC npcs[MAX_NUMBER_NPCS];

extern Link link;

extern uint16_t backgroundTopY, backgroundLeftX;

void SetupNPCs(){
    set_sprite_data(94,NPCMarin_TILE_COUNT,NPCMarin_tiles);

    for(uint8_t i=0;i<MAX_NUMBER_NPCS;i++){
        npcs[i].active=0;
    }
}

void CopyNPC(NPC* src, NPC* dest){
    dest->active=src->active;
    dest->x=src->x;
    dest->y=src->y;
    dest->dir=src->dir;
    dest->index=src->index;
    dest->sprite=src->sprite;
    dest->motionType=src->motionType;
}

void ActivateNPC(NPC *npc,uint8_t x, uint8_t y){
    npc->active=1;
    npc->x=x<<4;
    npc->y=y<<4;
}

uint8_t IsNPCInView(NPC* npc){
    return (npc->x<<4)>backgroundLeftX&&(npc->y<<4)>backgroundTopY&&(npc->y<<4)<backgroundTopY+128&&(npc->x<<4)<backgroundLeftX+160;
}

void UpdateNPC(NPC* npc){

    uint16_t screenX, screenY;

    screenX=(npc->x>>4)-backgroundLeftX;
    screenY=(npc->y>>4)-backgroundTopY;

    move_metasprite(NPCMarin_metasprites[0],94,6,screenX+8,screenY+16);
}

void ReOrderNPCs(){

    NPC npcsTemp[MAX_NUMBER_NPCS];

    uint8_t counter=0;

    // For each possible NPC
    for(uint8_t i=0;i<MAX_NUMBER_NPCS;i++){

        // If this is active, copy to the front of the temp array
        if(npcs[i].active){
            CopyNPC(&npcs[i],&npcsTemp[counter++]);
        }
    }

    // Default all the NPCs to inactive
    for(uint8_t i=0;i<MAX_NUMBER_NPCS;i++){
        npcs[i].active=0;
    }

    // Copy the active NPCs to the actual array
    for(uint8_t i=0;i<counter;i++){
        CopyNPC(&npcsTemp[counter++],&npcs[i]);
    }
}

void UpdateAllNPCs(){

    uint8_t reorder = FALSE;

    for(uint8_t i=0;i<MAX_NUMBER_NPCS;i++){

        // Expect an ordered array
        // If this entry is inactive, the rest should be too
        if(npcs[i].active==0)break;

        // Update this npC
        UpdateNPC(&npcs[i]);

        // We need to re order if this NPC is inactive
        if(npcs[i].active==0){
            reorder=TRUE;
        }
    }

    if(reorder){
        ReOrderNPCs();
    }
}