#include <gb/gb.h>
#include <gb/metasprites.h>
#include "objects.h"
#include "camera.h"

uint8_t joypadCurrent=0,joypadPrevious=0;




const int8_t J_DIRECTIONS[9][2]={
    {0,0},
    {1,0},
    {-1,0},
    {0,0},
    {0,-1},
    {0,0},
    {0,0},
    {0,0},
    {0,1}
};

uint8_t move_metasprite_with_camera2(const metasprite_t * metasprite, uint8_t base_tile, uint8_t base_sprite, uint16_t x, uint16_t y) {

    
    return move_metasprite(metasprite,base_tile,base_sprite,8+((x-cameraX)>>4),16+((y-cameraY)>>4));
}

uint8_t move_metasprite_with_camera(Object* object,uint8_t sprite) {

    // If we are blinking
    if((object->damageX!=0||object->damageY!=0)&&(universalBlinker>>4)==0)return 0;
    
    return move_metasprite(object->currentMetasprite,object->type->startTile,sprite,8+((object->x-cameraX)>>4),16+((object->y-cameraY)>>4));
}