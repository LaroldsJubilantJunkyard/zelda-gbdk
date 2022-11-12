#include <gb/gb.h>
#include "objects.h"
#include "userinterface.h"
#include "collision.h"
#include "camera.h"
#include "common.h"
#include "graphics/SwordAlone.h"

BANKREF_EXTERN(SwordAlone)

BANKREF(SwordAloneBank)

uint8_t gotSword=FALSE;

uint8_t move_metasprite_swordalone(Object* object,uint8_t sprite,uint8_t baseTileIndex) NONBANKED{

    uint16_t x = 8+(object->trueX-cameraTrueX);

    // The move_metasprite function's x parameter is a 8-bit integer. 
    // It's max value is 255. Afterthat it loops around
    // Hide if were offscreen
    if(x>168)return 0;

    uint16_t y = 16+(object->trueY-cameraTrueY);

    // The move_metasprite function's y parameter is a 8-bit integer. 
    // It's max value is 255. Afterthat it loops around
    // Hide if were offscreen
    if(y>152)return 0;

    uint8_t previous_bank = _current_bank;

    SWITCH_ROM(BANK(SwordAlone));
    
    uint8_t count = move_metasprite(SwordAlone_metasprites[0],baseTileIndex,sprite,x,y);

    SWITCH_ROM(previous_bank);

    return count;
}

void UpdateSwordAlone() BANKED{

    StopLinkFromOverlapping(currentObjectUpdating);

   

    if(CheckLinkInteractionWithObject(currentObjectUpdating)){

        DrawText("Link found a sword.");

        gotSword=TRUE;

        UpdateAItem();
        currentObjectUpdating->recycle=TRUE;
    }



    currentUpdateSpriteCount+=move_metasprite_swordalone(currentObjectUpdating,currentUpdateSpriteCount,currentObjectUpdating->baseTileIndex);
}