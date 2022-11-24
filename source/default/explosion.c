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

    uint8_t y2 = object->y-(4<<4);
    uint8_t y3 = (4<<4)+object->y;
    uint8_t x2 = (4<<4)+(object->x);
    uint8_t x3 = (object->x)-(4<<4);

    switch (frame)
    {
        case 0: 
            move_metasprite_with_camera(Explosion_metasprites[0],object->baseTileIndex,x2,y2);
            move_metasprite_with_camera(Explosion_metasprites[0],object->baseTileIndex,x3,y3);
            break;
        case 1: 
            move_metasprite_with_camera(Explosion_metasprites[0],object->baseTileIndex,x2,y2);
            move_metasprite_with_camera(Explosion_metasprites[0],object->baseTileIndex,x3,y3);
            break;
        case 2: 
            move_metasprite_with_camera(Explosion_metasprites[1],object->baseTileIndex,x2,y2);
            move_metasprite_with_camera(Explosion_metasprites[1],object->baseTileIndex,x3,y3);
            break;
        case 3: 
            move_metasprite_with_camera(Explosion_metasprites[1],object->baseTileIndex,x2,y2);
            move_metasprite_with_camera(Explosion_metasprites[1],object->baseTileIndex,x3,y3);
            break;
        case 4: 
            move_metasprite_with_camera(Explosion_metasprites[1],object->baseTileIndex,x2,y2);
            move_metasprite_with_camera(Explosion_metasprites[1],object->baseTileIndex,x3,y3);
            break;
        case 5: 
            move_metasprite_with_camera(Explosion_metasprites[1],object->baseTileIndex,object->x,object->y);
            break;
        case 6: 
            move_metasprite_with_camera(Explosion_metasprites[1],object->baseTileIndex,x2,y2);
            move_metasprite_with_camera(Explosion_metasprites[1],object->baseTileIndex,x3,y3);
            break;
        case 7: 
            move_metasprite_with_camera(Explosion_metasprites[2],object->baseTileIndex,object->x,object->y);
            break;
    }

    return spriteCount;
}