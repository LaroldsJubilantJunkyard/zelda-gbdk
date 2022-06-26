#include <gb/gb.h>
#include "common.h"
#include "objects.h"
#include "graphics/Explosion.h"

uint8_t UpdateExplosion(Object* object, uint8_t sprite){

    object->helper1+=4;

    uint8_t frame = object->helper1>>4;

    if(frame>=8){
        object->recycle=TRUE;
        return 0;
    }

    uint8_t spriteCount=0;

    switch (frame)
    {
        case 0: 
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[0],object->type->startTile,sprite+spriteCount,(4<<4)+(object->x),object->y-(4<<4));
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[0],object->type->startTile,sprite+spriteCount,(object->x)-(4<<4),(4<<4)+object->y);
            break;
        case 1: 
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[0],object->type->startTile,sprite+spriteCount,(4<<4)+(object->x),object->y-(4<<4));
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[0],object->type->startTile,sprite+spriteCount,(object->x)-(4<<4),(4<<4)+object->y);
            break;
        case 2: 
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[1],object->type->startTile,sprite+spriteCount,(4<<4)+(object->x),object->y-(4<<4));
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[1],object->type->startTile,sprite+spriteCount,(object->x)-(4<<4),(4<<4)+object->y);
            break;
        case 3: 
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[1],object->type->startTile,sprite+spriteCount,(4<<4)+(object->x),object->y-(4<<4));
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[1],object->type->startTile,sprite+spriteCount,(object->x)-(4<<4),(4<<4)+object->y);
            break;
        case 4: 
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[1],object->type->startTile,sprite+spriteCount,(4<<4)+(object->x),object->y-(4<<4));
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[1],object->type->startTile,sprite+spriteCount,(object->x)-(4<<4),(4<<4)+object->y);
            break;
        case 5: 
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[1],object->type->startTile,sprite+spriteCount,object->x,object->y);
            break;
        case 6: 
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[1],object->type->startTile,sprite+spriteCount,(4<<4)+(object->x),object->y-(4<<4));
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[1],object->type->startTile,sprite+spriteCount,(object->x)-(4<<4),(4<<4)+object->y);
            break;
        case 7: 
            spriteCount+=move_metasprite_with_camera2(Explosion_metasprites[2],object->type->startTile,sprite+spriteCount,object->x,object->y);
            break;
    }

    return spriteCount;
}