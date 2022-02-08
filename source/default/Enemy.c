#include <gb/gb.h>
#include <gb/metasprites.h>
#include "common.h"
#include "Enemy.h"
#include "Link.h"
#include "graphics/Moblin.h"

extern Link link;

extern uint16_t backgroundLeftX,backgroundTopY;
extern const int8_t SwordPositions[4][6];


Enemy enemies[MAX_ENEMIES_ONSCREEN];

void SetupEnemies(){

    for(uint8_t i=0;i<MAX_ENEMIES_ONSCREEN;i++){
        enemies[i].active=FALSE;
    }
}
void CopyEnemy(Enemy *src, Enemy *dest){
    
    dest->active=src->active;
    dest->x=src->x;
    dest->y=src->y;
    dest->hitX=src->hitX;
    dest->hitY=src->hitY;
    dest->direction=src->direction;
    dest->health=src->health;
    dest->startSprite=src->startSprite;
    dest->frameCounter=src->frameCounter;
    dest->movementType=src->movementType;
    dest->enemyType=src->enemyType;
    dest->set=src->set;
    dest->active=src->active;
}

void ReOrderEnemies(){

    Enemy enemiesTemp[MAX_ENEMIES_ONSCREEN];

    uint8_t counter=0;

    
    for(uint8_t i=0;i<MAX_ENEMIES_ONSCREEN;i++){
        enemiesTemp[i].active=0;
    }

    for(uint8_t i=0;i<MAX_ENEMIES_ONSCREEN;i++){

        if(enemies[i].active){

            CopyEnemy(&enemies[i],&enemiesTemp[counter]);
        }
    }

    
    for(uint8_t i=0;i<counter;i++){
        CopyEnemy(&enemiesTemp[counter],&enemies[i]);
    }

}


uint8_t IsEnemyInView(Enemy* enemy){
    return (enemy->x<<4)>backgroundLeftX&&(enemy->y<<4)>backgroundTopY&&(enemy->y<<4)<backgroundTopY+128&&(enemy->x<<4)<backgroundLeftX+160;
}
void ActivateEnemy(Enemy *enemy, int health,uint16_t x,uint16_t y){

    set_sprite_data(62,Moblin_TILE_COUNT,Moblin_tiles);
    enemy->x=x<<4;
    enemy->y=y<<4;
    enemy->frameCounter=0;
    enemy->direction=J_DOWN;
    enemy->set=FALSE;
    enemy->hitX=0;
    enemy->hitY=0;
    enemy->active=TRUE;
    enemy->health=health;
}

void UpdateEnemy(Enemy *enemy){

    uint16_t screenX = (enemy->x>>4) -backgroundLeftX;
    uint16_t screenY=(enemy->y>>4)-backgroundTopY;
    
    uint8_t frame =0;
    
    // Select the base frame from the direction
    if(enemy->direction==J_DOWN)frame=0;
    else if(enemy->direction==J_UP)frame=2;
    else if(enemy->direction==J_LEFT)frame=4;
    else if(enemy->direction==J_RIGHT)frame=6;

    
    enemy->frameCounter++;

    if(enemy->hitX!=0||enemy->hitY!=0){

        if(enemy->hitX!=0)enemy->x+=enemy->hitX;
        if(enemy->hitY!=0)enemy->y+=enemy->hitY;

        if(enemy->hitY>0)enemy->hitY--;
        else if(enemy->hitY<0)enemy->hitY++;

        if(enemy->hitX>0)enemy->hitX--;
        else if(enemy->hitX<0)enemy->hitX++;

        if(enemy->health==0&&enemy->hitX==0&&enemy->hitY==0){
            enemy->active=0;
        }
    }

    else if((enemy->x>>4)%16==0&&(enemy->y>>4)%16==0){

        enemy->set++;

        if(enemy->set>20){

            uint8_t dirs[]={J_UP,J_DOWN,J_LEFT,J_RIGHT};
            enemy->direction=dirs[DIV_REG%4];
            enemy->set=0;

            // Move in the neccessary direction 1px
            if(enemy->direction==J_DOWN)enemy->y+=1<<4;
            else if(enemy->direction==J_UP)enemy->y-=1<<4;
            else if(enemy->direction==J_LEFT)enemy->x-=1<<4;
            else if(enemy->direction==J_RIGHT)enemy->x+=1<<4;
        }

    }else{
        enemy->set=0;

        // Alternate frames
        if((enemy->frameCounter/10)%2==0)frame++;

        // Move in the neccessary direction
        if(enemy->direction==J_DOWN)enemy->y+=2;
        else if(enemy->direction==J_UP)enemy->y-=2;
        else if(enemy->direction==J_LEFT)enemy->x-=2;
        else if(enemy->direction==J_RIGHT)enemy->x+=2;
    }

    int16_t diffX=0,diffY=0;

    // If link isn't damaged
    if(link.hitY==0&&link.hitX==0){

        diffX=(link.x>>4)-((enemy->x>>4));
        diffY=(link.y>>4)-((enemy->y>>4));

        if(diffX<0)diffX=-diffX;
        if(diffY<0)diffY=-diffY;

        if(diffX<12&&diffY<12&&enemy->hitX==0&&enemy->hitY==0){

            link.hitX=(link.x>>4)-(enemy->x>>4);
            link.hitY=(link.y>>4)-(enemy->y>>4);

            link.health--;

        }
    }

    if(link.attacking){

        uint8_t ld=0;

        // Set our frame based on direction
        if(link.dir==J_DOWN)ld=0;
        else if(link.dir==J_UP)ld=1;
        else if(link.dir==J_LEFT)ld=2;
        else if(link.dir==J_RIGHT)ld=3;
        
        uint16_t playerAttackX=(link.x>>4)+SwordPositions[ld][link.frameOffset*2];
        uint16_t playerAttackY=(link.y>>4)+SwordPositions[ld][link.frameOffset*2+1];

        diffX=playerAttackX-((enemy->x>>4));
        diffY=playerAttackY-((enemy->y>>4));

        if(diffX<0)diffX=-diffX;
        if(diffY<0)diffY=-diffY;

        if(diffX<12&&diffY<12&&enemy->hitX==0&&enemy->hitY==0){

            enemy->hitX=(enemy->x>>4)-(link.x>>4);
            enemy->hitY=(enemy->y>>4)-(link.y>>4);
            enemy->health--;

            // Prevent unsigned int problems :D
            if(enemy->health>250)enemy->health=0;

        }


    }
    

    const metasprite_t *current=Moblin_metasprites[frame];
    
    // If the enemy is damaged
    if(enemy->hitX!=0||enemy->hitY!=0){

        if(enemy->active==FALSE) hide_metasprite(current,4);
        else{
            move_metasprite_props(current,62,4,screenX+8,screenY+16,1);
        }
    }else{
        

        // hide or show the metasprites
        if(enemy->active==FALSE)hide_metasprite(current,4);
        else move_metasprite(current,62,4,screenX+8,screenY+16);
    }

}

void UpdateAllEnemies(){

    uint8_t reorder = FALSE;

    for(uint8_t i=0;i<MAX_ENEMIES_ONSCREEN;i++){

        // We expect enemies to be orded, active enemies in the front
        // Stop everything if this enemy is inactive
        if(!enemies[i].active)break;

        // Update this enemy
        UpdateEnemy(&enemies[i]);

        // If this enemy is no longer active
        if(enemies[i].active==FALSE){

            // We need to re order 
            reorder=TRUE;
        }
    }

    // Re order if neccessary
    if(reorder)ReOrderEnemies();
        
}
