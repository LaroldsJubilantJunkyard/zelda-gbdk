#pragma bank 255

#include <gb/gb.h>
#include "common.h"
#include "camera.h"
#include "objects.h"
#include "collision.h"
#include "userinterface.h"
#include "link.h"
#include "graphics/Moblin.h"
#include "graphics/explosion.h"
#include "graphics/NPCMarin.h"

BANKREF(NpcsBank)
BANKREF_EXTERN(NPCMarin)

extern uint8_t gotSword;

void move_metasprite_marin(Object* object) NONBANKED{
    
    PUSH_NAMED_BANK(NPCMarin);

    object->currentMetasprite=NPCMarin_metasprites[0];

    move_object_with_camera(object);

    POP_BANK;

}

void UpdateMarin() BANKED{

    StopLinkFromOverlapping(currentObjectUpdating);

    if(CheckLinkInteractionWithObject(currentObjectUpdating,16,16)){
        if(gotSword)DrawText("Good luck on your adventure link");
        else DrawText("Its too dangerous to go out without a weapon");
    }    

    move_metasprite_marin(currentObjectUpdating);
}
