#include "objects.h"
#include "camera.h"
#include "userinterface.h"

uint8_t InteractWithObject(Object* object){

    uint8_t drawAtTop = ((link->y>>4)-(cameraY>>4)) > (CAMERA_VERTICAL_SIZE/2);

    if(object->type==MARIN_OBJECT_TYPE){

        DrawText("Your name is not zelda. Your name is in fact link. I dont get why",drawAtTop);

        return 1;
    }


    return 0;
}