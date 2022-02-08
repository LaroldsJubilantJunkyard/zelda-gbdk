#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/metasprites.h>
#include "Link.h"
#include "SpawnedWorldObject.h"
#include "World.h"
#include "common.h"
#include "graphics/HUD.h"
#include "graphics/AlphaNumeric.h"
#include "graphics/Map.h"


#define BACKGROUND_SCROLL_SPEED 2

uint8_t backgroundScroll;

uint16_t backgroundTopY, backgroundLeftX;
uint16_t backgroundTopRow, backgroundLeftColumn;
uint16_t backgroundNextRow, backgroundNextColumn;


extern Link link;

Section allSections[]={
    {3,0},
    {3,3},
    {3,6},
    {3,9}
};

WorldObject allObjects[]={
    {1,0,30,50},
    {1,0,50,50},
    {1,0,30,70},
    
    {1,0,30,50},
    {1,0,50,50},
    {1,0,30,70},
    
    {1,0,30,50},
    {1,0,50,50},
    {1,0,30,70},
    
    {1,0,30,50},
    {1,0,50,50},
    {1,0,30,70},
};


void ScrollWorldCamera() {

    move_bkg(backgroundLeftX,backgroundTopY);

    uint8_t newColumn=backgroundLeftX/8;
    uint8_t newRow=backgroundTopY/8;

    if(newColumn!=backgroundLeftColumn){

        if(newColumn>backgroundLeftColumn){
            VBK_REG=1;
            set_bkg_based_submap(backgroundLeftColumn+20,backgroundTopRow,1,18,Map_map_attributes,40,0);
            VBK_REG=0;
            set_bkg_based_submap(backgroundLeftColumn+20,backgroundTopRow,1,18,Map_map,40,HUD_TILE_COUNT);
        }else{

            VBK_REG=1;
            set_bkg_based_submap(backgroundLeftColumn-1,backgroundTopRow,1,18,Map_map_attributes,40,0);
            VBK_REG=0;
            set_bkg_based_submap(backgroundLeftColumn-1,backgroundTopRow,1,18,Map_map,40,HUD_TILE_COUNT);
        }
        backgroundLeftColumn=newColumn;
    }

    if(newRow!=backgroundTopRow){

        if(newRow>backgroundTopRow){
            VBK_REG=1;
            set_bkg_based_submap(backgroundLeftColumn,backgroundTopRow+18,20,1,Map_map_attributes,40,0);
            VBK_REG=0;;
            set_bkg_based_submap(backgroundLeftColumn,backgroundTopRow+18,20,1,Map_map,40,HUD_TILE_COUNT);
        }else{

            VBK_REG=1;
            set_bkg_based_submap(backgroundLeftColumn,backgroundTopRow-1,20,1,Map_map_attributes,40,0);
            VBK_REG=0;;
            set_bkg_based_submap(backgroundLeftColumn,backgroundTopRow-1,20,1,Map_map,40,HUD_TILE_COUNT);
        }
        backgroundTopRow=newRow;
    }

}

void SetupWorld(){

    set_bkg_data(HUD_TILE_COUNT,Map_TILE_COUNT,Map_tiles);
    set_bkg_palette(0,7,Map_palettes);

    VBK_REG=1;
    set_bkg_based_submap(0,0,20,18,Map_map_attributes,40,0);
    VBK_REG=0;
    set_bkg_based_submap(0,0,20,18,Map_map,40,HUD_TILE_COUNT);

    backgroundTopY=0;
    backgroundLeftX=0;
    backgroundLeftColumn=0;
    backgroundTopRow=0;
    backgroundNextColumn=0;
    backgroundNextRow=0;
    backgroundScroll=0;
}

void ActivateNewArea(uint8_t areaIndex){

    uint8_t i=0;

    for(i=0;i<MAX_SPAWNED_OBJECTS_ONSCREEN;i++){

        if(sortedSpawnedObjects[i].active==0)break;

        // Mark this as active
        sortedSpawnedObjects[i].old=TRUE;
    }

    uint8_t offset = allSections[areaIndex].offset;
    uint8_t count = allSections[areaIndex].count;

    uint8_t max = i+count;

    uint8_t j=0;

    for(j=0;j<count;j++){

        sortedSpawnedObjects[i].active=1;
        sortedSpawnedObjects[i].old=0;
        sortedSpawnedObjects[i].x=allObjects[j+offset].x;
        sortedSpawnedObjects[i].y=allObjects[j+offset].y;
        sortedSpawnedObjects[i].index=allObjects[j+offset].index;

        i++;
    }
}

void UpdateWorld(){

    if(backgroundScroll!=0){

        // If we are going leftwards
        if(backgroundScroll==J_LEFT){

            backgroundLeftX-=BACKGROUND_SCROLL_SPEED;
            

        // If we are going downwards
        }else  if(backgroundScroll==J_DOWN){

            backgroundTopY+=BACKGROUND_SCROLL_SPEED;
            

        // If we are going rightwards
        }else if(backgroundScroll==J_RIGHT){

            backgroundLeftX+=BACKGROUND_SCROLL_SPEED;
            

        // If we are going upwards
        }else  if(backgroundScroll==J_UP){

            backgroundTopY-=BACKGROUND_SCROLL_SPEED;
        }

        // If we are at our desired position, we are no longer scrolling
        if(backgroundLeftX==backgroundNextColumn*8&&backgroundTopY==backgroundNextRow*8)backgroundScroll=0;

        ScrollWorldCamera();
        
    }else{
        if((link.x>>4)<=backgroundLeftX+8&& (link.dir & J_LEFT)){
            if(backgroundLeftColumn>0){
                backgroundScroll=J_LEFT;
                backgroundNextColumn=backgroundLeftColumn-20;
            }
        }
        if((link.x>>4)>=backgroundLeftX+152&& (link.dir & J_RIGHT)){
            backgroundScroll=J_RIGHT;
            backgroundNextColumn=backgroundLeftColumn+20;
        }
        if((link.y>>4)<=backgroundTopY+8&& (link.dir & J_UP)){
            if(backgroundTopRow>0){
                backgroundScroll=J_UP;
                backgroundNextRow =backgroundTopRow-16;
            }
        }
        if((link.y>>4)>=backgroundTopY+120&& (link.dir & J_DOWN)){
            backgroundScroll=J_DOWN;
            backgroundNextRow =backgroundTopRow+16;
        }
    }

    
}