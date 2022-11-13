#pragma bank 255

#include <gb/gb.h>
#include "common.h"
#include "camera.h"
#include "objects.h"
#include "userinterface.h"
#include "collision.h"
#include "link.h"
#include "graphics/Moblin.h"
#include "graphics/explosion.h"

#define ENEMY_MOVE_SPEED 4

BANKREF(EnemiesBank)


void DamageLinkWhenClose(Object* object){

    if(link->damageX==0&&link->damageY==0){

        if(CheckObjectIntersection(link,object)){

            if(link->health>0)link->health--;

            NR10_REG = 0x5A;
            NR11_REG = 0x81;
            NR12_REG = 0x47;
            NR13_REG = 0x69;
            NR14_REG = 0x86;

            // Update the hearts shown
            UpdateHearts();

            link->damageX=((link->trueX)-(object->trueX))*5;
            link->damageY=((link->trueY)-(object->trueY))*5;
        }
    }
}

void ReceiveDamageFromLinksSword(Object* object){
    
    if(object->health==0)return;
    if(object->damageX!=0||object->damageY!=0)return;

    if(CheckObjectIntersectionAgainstLinksSword(object)){

        object->health--;

        NR10_REG = 0x5A;
        NR11_REG = 0x81;
        NR12_REG = 0x47;
        NR13_REG = 0x69;
        NR14_REG = 0x86;
        
        object->damageX = ((object->trueX)-(link->trueX))*3;
        object->damageY = ((object->trueY)-(link->trueY))*3;

    }
}

uint8_t UpdateDamagedEnemy(Object* object) NONBANKED{

    

    // If we are damaged
    if(object->damageX!=0||object->damageY!=0){

        // Update for damaged
        uint8_t done= Damaged(object);

        if(done&&object->health==0){

            object->recycle=TRUE;
            return 0;
        }

        return 0;

    }

    return 1;
}

uint8_t DirectionalFrames[9]={
    0,
    6,
    4,
    0,
    2,
    0,
    0,
    0,
    0
};

void move_object_with_camera_moblin(uint8_t frame) NONBANKED{
    

    PUSH_NAMED_BANK(Moblin);

    currentObjectUpdating->currentMetasprite=Moblin_metasprites[frame];

    move_object_with_camera(currentObjectUpdating);
    
    POP_BANK;
}

void UpdateMoblin() BANKED{

    uint8_t count=0;

    uint8_t frame = DirectionalFrames[currentObjectUpdating->direction];

    if(UpdateDamagedEnemy(currentObjectUpdating)){

        currentObjectUpdating->helper1+=5;

        if(currentObjectUpdating->helper1<20<<4){

            // Our next location
            uint16_t nextXEnemy=currentObjectUpdating->x+((J_DIRECTIONS[currentObjectUpdating->direction][0]*ENEMY_MOVE_SPEED));
            uint16_t nextYEnemy=currentObjectUpdating->y+((J_DIRECTIONS[currentObjectUpdating->direction][1]*ENEMY_MOVE_SPEED));

            // If we cannot move in that direction
            // Reset our timer by setting it's value really high
            if(!MoveToNextPosition(currentObjectUpdating,nextXEnemy,nextYEnemy,nextXEnemy>>4,nextYEnemy>>4)){
                currentObjectUpdating->helper1=41<<4;
            }
        
            frame+=universalBlinkerTrue;
        }else if(currentObjectUpdating->helper1>(25<<4)){
            currentObjectUpdating->helper1=0;
            uint8_t dirs[]={J_DOWN,J_RIGHT,J_LEFT,J_UP};
            currentObjectUpdating->direction=dirs[(DIV_REG%4)];
        }
        DamageLinkWhenClose(currentObjectUpdating);
        ReceiveDamageFromLinksSword(currentObjectUpdating);

    }

    move_object_with_camera_moblin(frame);


}