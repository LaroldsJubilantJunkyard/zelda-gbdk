#include <gb/gb.h>
#include "common.h"
#include "objects.h"
#include "collision.h"
#include "link.h"
#include "graphics/Moblin.h"
#include "graphics/explosion.h"

#define ENEMY_MOVE_SPEED 4

uint16_t moveTimer=0;

void DamageLinkWhenClose(Object* object){

    if(link->damageX==0&&link->damageY==0){

        if(CheckObjectIntersection(link,object)){

            link->damageX=((link->x>>4)-(object->x>>4))*5;
            link->damageY=((link->y>>4)-(object->y>>4))*5;
        }
    }
}

void ReceiveDamageFromLinksSword(Object* object){
    
    if(object->health==0)return;

    if(CheckCollisionAgainstLinksSword(object)){

        object->health--;
        
        object->damageX = ((object->x>>4)-(linkSwordX>>4))*5;
        object->damageY = ((object->y>>4)-(linkSwordY>>4))*5;

    }
}

uint8_t UpdateDamagedEnemy(Object* object, uint8_t sprite){

    

    // If we are damaged
    if(object->damageX!=0||object->damageY!=0){

        // Update for damaged
        uint8_t done= Damaged(object,sprite);

        if(object->health==0){

            object->recycle=TRUE;
            SpawnObject(EXPLOSION_OBJECT_TYPE,object->x,object->y,J_DOWN,0);
            return 1;
        }

        return move_metasprite_with_camera(object,sprite);
    }

    return 0;
}

uint8_t UpdateMoblin(Object* object, uint8_t sprite){

    if(UpdateDamagedEnemy(object,sprite))return 0;

    uint8_t frame = 0;
    if(object->direction==J_UP)frame=2;
    if(object->direction==J_LEFT)frame=4;
    if(object->direction==J_RIGHT)frame=6;

    moveTimer+=5;
    if((moveTimer>>4)<20){
        if(object->direction==J_DOWN){
            if(!MoveToNextPosition(object,object->x,object->y+ENEMY_MOVE_SPEED)){
                moveTimer=41<<4;
            }
        }
        if(object->direction==J_UP){
            if(!MoveToNextPosition(object,object->x,object->y-ENEMY_MOVE_SPEED)){
                moveTimer=41<<4;
            }
        }
        if(object->direction==J_LEFT){
            if(!MoveToNextPosition(object,object->x-ENEMY_MOVE_SPEED,object->y)){
                moveTimer=41<<4;
            }
        }
        if(object->direction==J_RIGHT){
            if(!MoveToNextPosition(object,object->x+ENEMY_MOVE_SPEED,object->y)){
                moveTimer=41<<4;
            }
        }
        frame+=universalBlinker>>4;
    }else if((moveTimer>>4)>25){
        moveTimer=0;
        uint8_t dirs[]={J_DOWN,J_RIGHT,J_LEFT,J_UP};
        object->direction=dirs[(DIV_REG%4)];
    }

    object->currentMetasprite=Moblin_metasprites[frame];

    DamageLinkWhenClose(object);
    ReceiveDamageFromLinksSword(object);

    return move_metasprite_with_camera(object,sprite);
}