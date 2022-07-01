#include <gb/gb.h>
#include "common.h"
#include "camera.h"
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
    if(object->damageX!=0||object->damageY!=0)return;

    if(CheckCollisionAgainstLinksSword(object)){

        object->health--;
        
        object->damageX = ((object->x>>4)-(link->x>>4))*3;
        object->damageY = ((object->y>>4)-(link->y>>4))*3;

    }
}

uint8_t UpdateDamagedEnemy(Object* object, uint8_t sprite,uint8_t* count){

    

    // If we are damaged
    if(object->damageX!=0||object->damageY!=0){

        // Update for damaged
        uint8_t done= Damaged(object,sprite);

        if(done&&object->health==0){

            object->recycle=TRUE;
            SpawnObject(EXPLOSION_OBJECT_TYPE,(object->x>>4)-(cameraX>>4),(object->y>>4)-(cameraY>>4),J_DOWN,0);
            return 0;
        }

        if((universalBlinkerFast>>4)==0){
            
            *count= move_metasprite_with_palette(object,sprite,1);
        }else{
            
            *count= move_metasprite_with_camera(object,sprite);
        }

        return 1;

    }

    return 0;
}

uint8_t UpdateMoblin(Object* object, uint8_t sprite){

    uint8_t count=0;
    if(UpdateDamagedEnemy(object,sprite,&count))return count;
    if(RemoveWhenOutOffscreen(object))return 0;

    uint8_t frame = 0;
    if(object->direction==J_UP)frame=2;
    if(object->direction==J_LEFT)frame=4;
    if(object->direction==J_RIGHT)frame=6;

    moveTimer+=5;
    if((moveTimer>>4)<20){

        // Our next location
        uint16_t nextX=object->x+((J_DIRECTIONS[object->direction][0]*ENEMY_MOVE_SPEED));
        uint16_t nextY=object->y+((J_DIRECTIONS[object->direction][1]*ENEMY_MOVE_SPEED));

        // If we cannot move in that direction
        // Reset our timer by setting it's value really high
        if(!MoveToNextPosition(object,nextX,nextY)){
            moveTimer=41<<4;
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