#include <gb/gb.h>
#include <gb/metasprites.h>
#include "objects.h"
#include "common.h"
#include "camera.h"
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


uint16_t linkSwordX,linkSwordY;
int8_t linkSword = -1;


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
    switch(object->direction){
        case J_DOWN: set_sprite_data(0,LinkSpritesDown_TILE_COUNT,LinkSpritesDown_tiles);break;
        case J_UP: set_sprite_data(0,LinkSpritesUp_TILE_COUNT,LinkSpritesUp_tiles);break;
        case J_LEFT: set_sprite_data(0,LinkSpritesLeft_TILE_COUNT,LinkSpritesLeft_tiles);break;
        case J_RIGHT: set_sprite_data(0,LinkSpritesRight_TILE_COUNT,LinkSpritesRight_tiles);break;
    }
}

void HandleLinkInput(Object* object,uint16_t *nextX, uint16_t *nextY,uint8_t *frame){
    if(linkSword==-1){
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

        // Update for damaged
        uint8_t done= Damaged(object,sprite);

        return move_metasprite_with_camera(object,sprite);
    }


    uint16_t nextX=object->x;
    uint16_t nextY=object->y;

    uint8_t frame = 0;

    if(cameraScrollDirection!=0){
        if(cameraScrollDirection==J_DOWN)nextY+=4;
        if(cameraScrollDirection==J_UP)nextY-=4;
        if(cameraScrollDirection==J_LEFT)nextX-=4;
        if(cameraScrollDirection==J_RIGHT)nextX+=4;
    }else{

        uint8_t previousObjectDirection=object->direction;

        HandleLinkInput(object,&nextX,&nextY,&frame);

        if(object->direction!=previousObjectDirection){

            UpdateLinkDefaultSprites(object);
           
        }
    }

    MoveToNextPosition(object,nextX,nextY);


    uint8_t spriteCount=0;

    if(linkSword>=0){

        linkSword+=5;
        frame=linkSword>>4;

        if(frame<4){

            if(frame>2)frame=2;
            
            object->currentMetasprite=LinkSwordMetasprites[object->direction][frame];

            spriteCount+=move_metasprite_with_camera(object,sprite);

            linkSwordX=object->x+(SwordOffsets[object->direction][frame][0]<<4);
            linkSwordY=object->y+(SwordOffsets[object->direction][frame][1]<<4);

            spriteCount+=move_metasprite_with_camera2(SwordSlashMetasprites[object->direction][frame],8,sprite+spriteCount,linkSwordX,linkSwordY);

        }else {
            linkSword=-1;
            frame=0;

            UpdateLinkDefaultSprites(object);
        }
    }
    
    if(linkSword==-1){

        object->currentMetasprite=LinkMetasprites[object->direction][frame];

        spriteCount+=move_metasprite_with_camera(object,sprite);
    }

    return spriteCount;

}