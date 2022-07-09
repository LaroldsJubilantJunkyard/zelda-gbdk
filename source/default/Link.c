#include <gb/gb.h>
#include <gb/metasprites.h>
#include "objects.h"
#include "common.h"
#include "camera.h"
#include "objectinteraction.h"
#include "collision.h"
#include "graphics/LinkSpritesDown.h"
#include "graphics/LinkSpritesUp.h"
#include "graphics/LinkSpritesLeft.h"
#include "graphics/LinkSpritesRight.h"
//
#include "graphics/SwordSlashDown.h"
#include "graphics/SwordSlashUp.h"
#include "graphics/SwordSlashLeft.h"
#include "graphics/SwordSlashRight.h"
//
#include "graphics/LinkSwordSpritesDown.h"
#include "graphics/LinkSwordSpritesLeft.h"
#include "graphics/LinkSwordSpritesUp.h"
#include "graphics/LinkSwordSpritesRight.h"


uint16_t linkSwordX,linkSwordY,nextX,nextY;
int8_t linkSword = -1;
uint8_t linkMaxHealth=5;

uint8_t frame = 0;


const int8_t SwordOffsets[9][3][2]={
    {{0,0},{0,0},{0,0}},
    {{0,-16},{8,-8},{16,0}}, // right
    {{0,-16},{-8,-8},{-16,0}}, // left
    {{0,0},{0,0},{0,0}},
    {{16,0},{8,-8},{0,-16}}, // up
    {{0,0},{0,0},{0,0}},
    {{0,0},{0,0},{0,0}},
    {{0,0},{0,0},{0,0}},
    {{-16,0},{-8,8},{0,16}} // down
};


const uint8_t LinkSpritesTileCounts[]={
    0,
    LinkSpritesRight_TILE_COUNT,
    LinkSpritesLeft_TILE_COUNT,
    0,
    LinkSpritesUp_TILE_COUNT,
    0,
    0,
    0,
    LinkSpritesDown_TILE_COUNT,

};


const uint8_t* LinkSpritesTiles[]={
    0,
    LinkSpritesRight_tiles,
    LinkSpritesLeft_tiles,
    0,
    LinkSpritesUp_tiles,
    0,
    0,
    0,
    LinkSpritesDown_tiles,

};


const metasprite_t** LinkMetasprites[]={
    0,
    LinkSpritesRight_metasprites,
    LinkSpritesLeft_metasprites,
    0,
    LinkSpritesUp_metasprites,
    0,
    0,
    0,
    LinkSpritesDown_metasprites,

};

const metasprite_t** LinkSwordMetasprites[]={
    0,
    LinkSwordSpritesRight_metasprites,
    LinkSwordSpritesLeft_metasprites,
    0,
    LinkSwordSpritesUp_metasprites,
    0,
    0,
    0,
    LinkSwordSpritesDown_metasprites,

};

const metasprite_t** SwordSlashMetasprites[]={
    0,
    SwordSlashRight_metasprites,
    SwordSlashLeft_metasprites,
    0,
    SwordSlashUp_metasprites,
    0,
    0,
    0,
    SwordSlashDown_metasprites,
};

void UpdateLinkDefaultSprites(Object* object){

    // Change our object's data for when vram is changed, so it doesn't default
    object->type->tileData = LinkSpritesDown_tiles;

    // Update our vram for the proper direction
    set_sprite_data(0,LinkSpritesTileCounts[object->direction],LinkSpritesTiles[object->direction]);
}

void HandleLinkInput(Object* object,uint16_t *nextX, uint16_t *nextY,uint8_t *frame){
    if(linkSword==-1 && cameraScrollDirection==0){
            if(joypadCurrent&J_RIGHT){
                *nextX+=8;
                *frame=universalBlinker>>4;
                object->direction=J_RIGHT;
            }
            if(joypadCurrent&J_LEFT){
                *nextX-=8;
                *frame=universalBlinker>>4;
                object->direction=J_LEFT;
            }
            if(joypadCurrent&J_DOWN){
                *nextY+=8;
                *frame=universalBlinker>>4;
                object->direction=J_DOWN;
            }
            if(joypadCurrent&J_UP){
                *nextY-=8;
                *frame=universalBlinker>>4;
                object->direction=J_UP;
            }
        }

        if((joypadCurrent&J_A)&&!(joypadPrevious&J_A)&&linkSword==-1){

            Object* currentObject = firstObject;

            uint16_t lx = (link->x)+((J_DIRECTIONS[link->direction][0]*16)<<4);
            uint16_t ly = (link->y)+((J_DIRECTIONS[link->direction][1]*16)<<4);

            while(currentObject!=0){

                if(CheckObjectIntersection3(currentObject,lx,ly,16,16)){
                    
                    // If we can interact with this object
                    if(InteractWithObject(currentObject)){

                        return;
                    }
                }
                currentObject=currentObject->next;
            }

            linkSword=0;

            switch(object->direction){
                case J_DOWN: set_sprite_data(0,LinkSwordSpritesDown_TILE_COUNT,LinkSwordSpritesDown_tiles);break;
                case J_UP: set_sprite_data(0,LinkSwordSpritesUp_TILE_COUNT,LinkSwordSpritesUp_tiles);break;
                case J_LEFT: set_sprite_data(0,LinkSwordSpritesLeft_TILE_COUNT,LinkSwordSpritesLeft_tiles);break;
                case J_RIGHT: set_sprite_data(0,LinkSwordSpritesRight_TILE_COUNT,LinkSwordSpritesRight_tiles);break;
            }

            switch(object->direction){
                case J_DOWN: set_sprite_data(8,SwordSlashDown_TILE_COUNT,SwordSlashDown_tiles);break;
                case J_UP: set_sprite_data(8,SwordSlashUp_TILE_COUNT,SwordSlashUp_tiles);break;
                case J_LEFT: set_sprite_data(8,SwordSlashLeft_TILE_COUNT,SwordSlashLeft_tiles);break;
                case J_RIGHT: set_sprite_data(8,SwordSlashRight_TILE_COUNT,SwordSlashRight_tiles);break;
            }
           
        }
}

uint8_t UpdateLink(Object* object, uint8_t sprite){

    // If we are damaged
    if(object->damageX!=0||object->damageY!=0){
        
        nextX=object->x+object->damageX;
        nextY=object->y+object->damageY;

        // Update for damaged
        DamagedNoMove(object,sprite);

        return 0;
    }


     nextX=object->x;
     nextY=object->y;

     frame = 0;

    if(cameraScrollDirection!=0){

        object->direction=cameraScrollDirection;

        // Move in the scroll direction
        nextX+=J_DIRECTIONS[cameraScrollDirection][0]*4;
        nextY+=J_DIRECTIONS[cameraScrollDirection][1]*4;
    }else{

        uint8_t previousObjectDirection=object->direction;

        HandleLinkInput(object,&nextX,&nextY,&frame);

        if(object->direction!=previousObjectDirection){

            UpdateLinkDefaultSprites(object);
           
        }
    }

    return 0;

}

/**
 * @brief Called after all objects have updated
 * This should becalled after all objects have been updated. This method gives objects the ability to stop intersection/overlap with link. Without having to loop
 * through each object unneccesarily.
 * @param sprite The starting sprite to use when drawing metasprites
 * @return uint8_t How many sprites link has used
 */
uint8_t FinishLinkUpdate(uint8_t sprite){

    Object* object = link;

    // Move to the next position for link
    MoveToNextPosition(object,nextX,nextY);

    // If we are damaged
    if(object->damageX!=0||object->damageY!=0){

        // Alternate between our normal palette and palette 1
        if(universalBlinkerFast>>4==0)return move_metasprite_with_palette(object,sprite,1);
        return move_metasprite_with_camera(object,sprite);
    }

    // If the camera is scrolling
    if(cameraScrollDirection!=0){

        // If we are not facing the scrolling direction
        if(object->direction!=cameraScrollDirection){

            // Update our direction
            object->direction=cameraScrollDirection;

            // Update links sprites
            UpdateLinkDefaultSprites(object);
        }
        
        // set our metasprite based on the universal blinker
        object->currentMetasprite=LinkMetasprites[object->direction][universalBlinker>>4];
        return move_metasprite_with_camera(object,sprite);
    }

    uint8_t spriteCount=0;

    if(linkSword>=0){

        linkSword+=5;
        frame=linkSword>>4;

        if(frame<4){

            // The sword slash animations only have 3 frames
            // Limit the frame variable here so it stays in the final frame for a little bit
            if(frame>2)frame=2;

            // Draw link's' sword slash animation
            object->currentMetasprite=LinkSwordMetasprites[object->direction][frame];
            spriteCount+=move_metasprite_with_camera(object,sprite);

            // Position link's sword based on it's frame
            linkSwordX=object->x+(SwordOffsets[object->direction][frame][0]<<4);
            linkSwordY=object->y+(SwordOffsets[object->direction][frame][1]<<4);

            // Draw links sword
            spriteCount+=move_metasprite_with_camera2(SwordSlashMetasprites[object->direction][frame],8,sprite+spriteCount,linkSwordX,linkSwordY);

        }else {
            linkSword=-1;
            frame=0;

            UpdateLinkDefaultSprites(object);
        }
    }
    
    // If link is not using his sword
    if(linkSword==-1){


        // draw his current metasprite
        object->currentMetasprite=LinkMetasprites[object->direction][frame];
        spriteCount+=move_metasprite_with_camera(object,sprite);
    }

    return spriteCount;

}