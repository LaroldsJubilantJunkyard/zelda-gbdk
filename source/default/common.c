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

     x = 8+((x-cameraX)>>4);
    
    if(x>255)return 0;

     y = 16+((y-cameraY)>>4);

    if(y>255)return 0;
    
    return move_metasprite(metasprite,base_tile,base_sprite,x,y);
}

uint8_t move_metasprite_with_palette(Object* object,uint8_t sprite,uint8_t palette){
        metasprite_t temp[8];


        const metasprite_t* currentMetasprite = object->currentMetasprite;

        uint8_t usedCount = 0;

        while(currentMetasprite->dy!=metasprite_end){

            // Retain sprite position, and tile
            temp[usedCount].dy=currentMetasprite->dy;
            temp[usedCount].dx=currentMetasprite->dx;
            temp[usedCount].dtile=currentMetasprite->dtile;
            temp[usedCount].props=currentMetasprite->props;

            // If we should blink for damage
            // Change the properties, but retain the flip
            temp[usedCount].props= palette | currentMetasprite->props;


            usedCount++;

            currentMetasprite++;
        }

        temp[usedCount].dy=metasprite_end;


        return move_metasprite_with_camera2(temp,object->type->startTile,sprite,object->x,object->y);
}

uint8_t move_metasprite_with_camera(Object* object,uint8_t sprite) {

    uint16_t x = 8+((object->x-cameraX)>>4);

    // The move_metasprite function's x parameter is a 8-bit integer. 
    // It's max value is 255. Afterthat it loops around
    // Hide if were offscreen
    if(x>168)return 0;

    uint16_t y = 16+((object->y-cameraY)>>4);

    // The move_metasprite function's y parameter is a 8-bit integer. 
    // It's max value is 255. Afterthat it loops around
    // Hide if were offscreen
    if(y>152)return 0;
    
    return move_metasprite(object->currentMetasprite,object->type->startTile,sprite,x,y);
}