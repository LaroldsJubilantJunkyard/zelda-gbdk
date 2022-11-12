#include <gb/gb.h>
#include "objects.h"
#include "userinterface.h"
#include "collision.h"
#include "camera.h"
#include "common.h"
#include "graphics/Chest.h"

BANKREF_EXTERN(Chest)

BANKREF(ChestBank)

uint8_t chestUsed=FALSE;

void move_metasprite_camera_chest(Object* object) NONBANKED{

    PUSH_NAMED_BANK(Chest);
    
    object->currentMetasprite = Chest_metasprites[1-chestUsed];
    move_object_with_camera(object);

    POP_BANK;

}

void UpdateChest() BANKED{

    StopLinkFromOverlapping(currentObjectUpdating);

    if(CheckLinkInteractionWithObject(currentObjectUpdating) && !chestUsed){

        DrawText("Link found something.");

        chestUsed=TRUE;
    }

    move_metasprite_camera_chest(currentObjectUpdating);
}