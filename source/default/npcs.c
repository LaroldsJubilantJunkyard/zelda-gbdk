#include <gb/gb.h>
#include "common.h"
#include "camera.h"
#include "objects.h"
#include "collision.h"
#include "link.h"
#include "graphics/Moblin.h"
#include "graphics/explosion.h"
#include "graphics/NPCMarinDown.h"


uint8_t UpdateMarin(Object* object,uint8_t sprite){

    StopLinkFromOverlapping(object);

    object->currentMetasprite=NPCMarinDown_metasprites[0];

    return move_metasprite_with_camera(object,sprite);
}
