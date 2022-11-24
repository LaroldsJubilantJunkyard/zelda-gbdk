#include <gb/gb.h>
#include <gbdk/far_ptr.h>
#include <gb/metasprites.h>
#include "objects.h"
#include "camera.h"

uint8_t joypadCurrent=0,joypadPrevious=0;
uint8_t _previous_bank;


const int16_t J_DIRECTIONS[9][2]={
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

void move_metasprite_with_camera(const metasprite_t * metasprite, uint8_t base_tile,  int16_t x, int16_t y) NONBANKED{

     x = 8+(x-cameraTrueX);
    
    if(x>168)return;
    if(x<-8)return;

     y = 16+(y-cameraTrueY);

    if(y>160)return;
    if(y<-8)return;
    
    currentUpdateSpriteCount+=move_metasprite(metasprite,base_tile,currentUpdateSpriteCount,(uint8_t) x,(uint8_t) y);
}

void move_metasprite_with_palette(Object* object,uint8_t palette)NONBANKED{
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


    move_metasprite_with_camera(temp,object->baseTileIndex,object->trueX,object->trueY);
}

void move_object_with_camera(Object* object) NONBANKED{
    uint8_t blink =  (object->damageX != 0 || object->damageY != 0)&&(universalBlinkerFastTrue == 0);

    if(blink)move_metasprite_with_palette(object,1);

    else move_metasprite_with_camera(object->currentMetasprite,object->baseTileIndex,object->trueX,object->trueY);
}
