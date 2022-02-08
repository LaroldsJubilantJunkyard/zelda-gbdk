#include <gb/gb.h>
#include <gbdk/metasprites.h>
#include "Link.h"
#include "HeadsUpDisplay.h"
#include "NPC.h"
#include "common.h"
#include "graphics/LinkSprites.h"
#include "graphics/LinkPushSprites.h"
#include "graphics/LinkShieldSprites.h"
#include "graphics/LinkActiveShieldSprites.h"
#include "graphics/LinkSwordSprites.h"
#include "graphics/Map.h"
#include "graphics/SwordSlash.h"

Link link;
extern NPC npcs[MAX_NUMBER_NPCS];

extern uint8_t backgroundScroll;
extern uint16_t backgroundTopY, backgroundLeftX;

metasprite_t* const *LinkMetaSprites;

const int8_t SwordPositions[4][6]={
    // Down
    {-16,0,  -8,8,  0,16},
    // Up
    {16,0,  8,-8,  0,-16},
    // Left
    {0,-16,  -8,-8,  -16,0},
    // Right
    {0,-16,  8,-8,  16,0}
};

void UpdateLinkSpriteData(){
    if(link.attacking){
        
        set_sprite_data(0,LinkSwordSprites_TILE_COUNT,LinkSwordSprites_tiles);
        LinkMetaSprites=LinkSwordSprites_metasprites;
    }else if(link.pushCounter>=PUSH_COUNTER_MAX){
        
        
        set_sprite_data(0,LinkPushSprites_TILE_COUNT,LinkPushSprites_tiles);
        LinkMetaSprites=LinkPushSprites_metasprites;
    }else if(link.weapon & SHIELD){
        if(joypadCurrent&J_B){
            set_sprite_data(0,LinkActiveShieldSprites_TILE_COUNT,LinkActiveShieldSprites_tiles);
            LinkMetaSprites=LinkActiveShieldSprites_metasprites;
        }
        else {
            
            set_sprite_data(0,LinkShieldSprites_TILE_COUNT,LinkShieldSprites_tiles);
            LinkMetaSprites=LinkShieldSprites_metasprites;
        }
    }else{
        
        set_sprite_data(0,LinkSprites_TILE_COUNT,LinkSprites_tiles);
        LinkMetaSprites=LinkSprites_metasprites;
    }
    link.frameOffset=0;
    link.frameCounter=0;
}
void SetupLink(){
    
    link.pushCounter=0;
    link.x=80<<4;
    link.y=80<<4;
    link.health=10;
    link.hearts=4;
    link.moving=0;
    link.frameCounter=0;
    link.frameOffset=0;
    link.swordFrame=255;
    link.weapon=SWORD | SHIELD;
    link.lastFrame=0;
    link.attacking=0;
    link.frame=0;
    link.hitX=0;
    link.hitY=0;
    link.dir=J_DOWN;


    UpdateLinkSpriteData();

    set_sprite_data(32,SwordSlash_TILE_COUNT,SwordSlash_tiles);
    set_sprite_palette(0,3,LinkSprites_palettes);

}

void AnimateLink(){

    uint8_t walking = !link.attacking && link.moving;

     if(walking||link.attacking){
        link.frameCounter++;
     }

    uint8_t frameRate = 10;
    uint8_t animationLength = 2;
    if(link.attacking)animationLength=3;
    if(link.attacking)frameRate=4;

    if(link.frameCounter>=frameRate){
        link.frameCounter=0;
        link.frameOffset= (link.frameOffset+1)%animationLength;

        
        if(link.attacking && link.frameOffset==0){
            
            // We are not attacking anymore
            link.attacking=0;
            
            // Update which sprites are used
            UpdateLinkSpriteData();
        }
    }

}


void GetLinkMetaspriteFrame(){
    if(!link.attacking){

        // Set our frame based on direction
        // We don't handle down and up at the same time
        if(link.dir&J_DOWN)link.frame=0;
        else if(link.dir&J_UP)link.frame=2;
        // We don't handle left and right at the same time
        if(link.dir&J_LEFT)link.frame=4;
        else if(link.dir&J_RIGHT)link.frame=6;

        // Shield animations
        if(link.weapon & SHIELD){
            if((joypadCurrent&J_B)&&!(joypadPrevious&J_B)){
                UpdateLinkSpriteData();
            }else if(!(joypadCurrent&J_B)&&(joypadPrevious&J_B)){
                UpdateLinkSpriteData();
            }
        }

    }else{
        

        // Set our frame based on direction
        // We don't handle down and up at the same time
        if(link.dir&J_DOWN)link.frame=0;
        else if(link.dir&J_UP)link.frame=3;
        // We don't handle left and right at the same time
        if(link.dir&J_LEFT)link.frame=6;
        else if(link.dir&J_RIGHT)link.frame=9;
    }
        
       
}

void ShowHideLinkSwordMetaSprite(){
    if(link.attacking && link.frameOffset<3){

        uint8_t swordBaseFrame = 0;

        // use the sword animations
        if(link.dir&J_DOWN)swordBaseFrame = 0;
        else if(link.dir&J_UP)swordBaseFrame = 3;
        else if(link.dir&J_LEFT)swordBaseFrame = 6;
        else if(link.dir&J_RIGHT)swordBaseFrame = 9;

        int16_t swordX =(link.x>>4)-backgroundLeftX;
        int16_t swordY =(link.y>>4)-backgroundTopY;

        swordX+=SwordPositions[swordBaseFrame/3][link.frameOffset*2];
        swordY+=SwordPositions[swordBaseFrame/3][link.frameOffset*2+1];

        link.swordFrame=swordBaseFrame+link.frameOffset;

        move_metasprite(SwordSlash_metasprites[link.swordFrame],32,2,swordX+8,swordY+16);

    }else if(link.swordFrame!=255){
        link.swordFrame=255;
        link.attacking=0;
        move_sprite(3,0,0);
        move_sprite(2,0,0);
    }
}

void HandleJoypadInput(){
    

    uint8_t moveSpeed=10;

    // Use the joypad for movement by default
    uint8_t linkMove=joypadCurrent;

    if(backgroundScroll!=0){

        // Use the background scroll direction
        linkMove=backgroundScroll;
        moveSpeed=3;

    }else {

        if(link.hitX!=0||link.hitY!=0){

            linkMove=0;

            if(link.hitX!=0)link.x+=link.hitX;
            if(link.hitY!=0)link.y+=link.hitY;

            if(link.hitX>0)link.hitX--;
            else if(link.hitX<0)link.hitX++;
            if(link.hitY<0)link.hitY++;
            else if(link.hitY>0)link.hitY--;

        }else{
            

            // If we are holding shield and b isheld
            if((joypadCurrent&J_B)&&(link.weapon & SHIELD)){
                moveSpeed=5;
                link.shielding=1;

            }else{
                link.shielding=0;
            }

            // If we have the sword and a is pressed
            if((link.weapon & SWORD) &&(joypadCurrent & J_A) &&!(joypadPrevious & J_A)){

                // We are attacking
                link.attacking=1;
                    
                UpdateLinkSpriteData();
            }
        }
    }

    if(!link.attacking){

        uint8_t oldDir=link.dir;

        if(linkMove & J_LEFT){
            link.x-=moveSpeed;
            link.dir = J_LEFT;
        }else if(linkMove & J_RIGHT){
            link.x+=moveSpeed;
            link.dir = J_RIGHT;
        }else{
            link.dir=0;
        }

        if(linkMove & J_UP){
            link.y-=moveSpeed;
            link.dir |= J_UP;
        }else if(linkMove & J_DOWN){
            link.y+=moveSpeed;
            link.dir |= J_DOWN;
        }else if(link.dir==0){
            link.dir=oldDir;
        }
    }

    // Save if we are trying to move in a direction
    link.moving=(linkMove & J_UP || linkMove & J_DOWN || linkMove & J_LEFT || linkMove & J_RIGHT);
}

uint8_t CheckNPCCollisions(uint16_t oldX, uint16_t oldY){


    int16_t diffX,diffY;
    

    for(uint8_t i=0;i<MAX_NUMBER_NPCS;i++){

        // Expect an ordered array
        // If this entry is inactive, the rest should be too
        if(npcs[i].active==0)break;

        NPC* npc = &npcs[i];

        // Get the difference between our two bounding boxes
        diffX=(link.x>>4)-(npc->x>>4);
        diffY=(link.y>>4)-(npc->y>>4);

        // get the absolute value
        if(diffX<0)diffX=-diffX;
        if(diffY<0)diffY=-diffY;
        
        // If a is pressed
        if((joypadCurrent & J_A)&& !(joypadPrevious& J_A)){
            
            // Check if we are within interact distance
            if(diffX<=16&&diffY<=16){
                WriteText("Your name is not zelda",(link.y>>4)<64);
                return 1;
            }
        }
        
        // Check if we are colliding
        if(diffX<=8&&diffY<=12){
            int16_t oldDiffX,oldDiffY;

            // Get the difference between our two bounding boxes
            oldDiffX=(oldX>>4)-(npc->x>>4);
            oldDiffY=(oldY>>4)-(npc->y>>4);

            // get the absolute value
            if(oldDiffX<0)oldDiffX=-oldDiffX;
            if(oldDiffY<0)oldDiffY=-oldDiffY;

            uint8_t intercept =0;

            if(oldDiffX>8)intercept|=J_LEFT;
            if(oldDiffY>8)intercept|=J_UP;
/*
            if(link.pushCounter>=PUSH_COUNTER_MAX){

                // Set our frame based on direction
                // We don't handle down and up at the same time
                if(link.dir&J_DOWN){
                    npc->y+=1<<4;
                    link.y+=1<<4;
                }
                else if(link.dir&J_UP){
                    npc->y-=1<<4;
                    link.y-=1<<4;
                }

                // We don't handle left and right at the same time
                if(link.dir&J_LEFT){
                    npc->x-=1<<4;
                   link.x-=1<<4;
                }
                else if(link.dir&J_RIGHT){
                    npc->x+=1<<4;
                    link.x+=1<<4;
                }
            }*/

            return intercept;

        }

    }

    uint16_t lx = (link.x>>4);
    uint16_t ly = (link.y>>4);

    if(link.dir & J_RIGHT)lx+=4;
    if(link.dir & J_LEFT)lx-=4;

    if(link.dir & J_DOWN)ly+=6;
    if(link.dir & J_UP)ly-=3;

    uint16_t column=lx/8;
    uint16_t row=ly/8;

    uint8_t tile = get_bkg_tile_xy(column,row);

    if((tile>=0x1C&&tile<=0x1F)||(tile>=0x24&&tile<=0x27)||(tile>=0x29&&tile<=0x30)){

        column=lx/8;
        row=(link.y>>4)/8;
        uint8_t tileXOnly = get_bkg_tile_xy(column,row);

        column=(link.x>>4)/8;
        row=ly/8;
        uint8_t tileYOnly = get_bkg_tile_xy(column,row);

        uint8_t intercept = 0;

        if((tileXOnly>=0x1C&&tileXOnly<=0x1F)||(tileXOnly>=0x24&&tileXOnly<=0x27)||(tileXOnly>=0x29&&tileXOnly<=0x30))intercept|=J_LEFT;
        if((tileYOnly>=0x1C&&tileYOnly<=0x1F)||(tileYOnly>=0x24&&tileYOnly<=0x27)||(tileYOnly>=0x29&&tileYOnly<=0x30))intercept|=J_UP;

        return intercept;
    }

    return 0;
}

void UpdateLink(){
    
    uint16_t oldX=link.x;
    uint16_t oldY=link.y;
        
    HandleJoypadInput();
    GetLinkMetaspriteFrame();
    AnimateLink();

    uint8_t intercept=0;

    intercept=CheckNPCCollisions(oldX,oldY);

    if(intercept & J_LEFT){
        link.x=oldX;
    }
    
    if(intercept & J_UP){
        link.y=oldY;
    }

    if(intercept!=0&&link.x==oldX&&link.y==oldY){
        link.pushCounter++;
        if(link.pushCounter==PUSH_COUNTER_MAX){
            UpdateLinkSpriteData();
        }
    }else{
        if(link.pushCounter>=PUSH_COUNTER_MAX){
            link.pushCounter=0;
            UpdateLinkSpriteData();
        }
        link.pushCounter=0;
    }

    ShowHideLinkSwordMetaSprite();

    // Position according to the background's location
    uint16_t screenPosX = (link.x>>4)-backgroundLeftX;
    uint16_t screenPosY = (link.y>>4)-backgroundTopY;

    if((link.hitX==0&&link.hitY==0)||!blinkDamaged)move_metasprite(LinkMetaSprites[link.frame+link.frameOffset],0,0,screenPosX+8,screenPosY+16);
    else move_metasprite_props(LinkMetaSprites[link.frame+link.frameOffset],0,0,screenPosX+8,screenPosY+16,1);
}