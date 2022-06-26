#include <gb/gb.h>
#include "camera.h"
#include "common.h"
#include "objects.h"
#include "graphics/TileMap.h"
uint8_t cameraScrollDirection=0;
uint16_t cameraX,cameraY;
uint16_t cameraSectionColumn,cameraSectionRow;
uint16_t lastCameraX, lastCameraY;
uint16_t lastCameraRow, lastCameraColumn;

void SetupCamera(){
    cameraX=0;
    cameraY=0;
    cameraSectionColumn=0;
    cameraSectionRow=0;
    lastCameraColumn=0;
    lastCameraRow=0;
    lastCameraX=0;
    lastCameraY=0;

}

void UpdateCameraSubMap(){

    // update hardware scroll position
    SCY_REG = cameraY>>4; SCX_REG = cameraX>>4; 
    
    uint16_t cameraRow = (cameraY>>4)/8;
    uint16_t cameraColumn=(cameraX>>4)/8;

    // If our camera's column has changed
    if(lastCameraColumn!=cameraColumn){

        // Save which column we are in
        lastCameraColumn=cameraColumn;

        if(cameraX>lastCameraX){
            if ((TileMap_WIDTH/8 - 20u) > cameraColumn) set_bkg_submap(cameraColumn + 20u, cameraRow, 1, MIN(19u, TileMap_HEIGHT/8 - cameraRow), TileMap_map, TileMap_WIDTH/8);  

        }else{
            set_bkg_submap(cameraColumn, cameraRow, 1, MIN(19u, TileMap_HEIGHT/8 - cameraRow), TileMap_map, TileMap_WIDTH/8);   

        }
    }

    // If our camera's row has changed
    if(lastCameraRow!=cameraRow){

        // Save which row we are in
        lastCameraRow=cameraRow;

        if(cameraY>lastCameraY){
            if ((TileMap_HEIGHT/8 - 18u) > cameraRow) set_bkg_submap(cameraColumn, cameraRow + 18u, MIN(21u, TileMap_WIDTH/8-cameraColumn), 1, TileMap_map, TileMap_WIDTH/8); 

        }else{

            set_bkg_submap(cameraColumn, cameraRow, MIN(21u, TileMap_WIDTH/8-cameraColumn), 1, TileMap_map, TileMap_WIDTH/8);
        }
    }

    // Update our camera position
    lastCameraX=cameraX;
    lastCameraY=cameraY;
}


void UpdateCamera(){

    // If the camera is not scrolling?
    if(cameraScrollDirection==0){

        int16_t linkScreenX= (link->x>>4)-(cameraX>>4);

        // If link is on the right edge
        if(linkScreenX>152){
            if(cameraSectionColumn<TileMap_HEIGHT/CAMERA_VERTICAL_SIZE){
                cameraSectionColumn++;
                cameraScrollDirection=J_RIGHT;
            }

        // If link is on the left edge
        }else if(linkScreenX<8){
            if(cameraSectionColumn>0){
                cameraSectionColumn--;
                cameraScrollDirection=J_LEFT;
            }
        }

        int16_t linkScreenY= (link->y>>4)-(cameraY>>4);

        // If link is on the bottom edge
        if(linkScreenY>CAMERA_VERTICAL_SIZE-8){
            if(cameraSectionRow<TileMap_HEIGHT/CAMERA_VERTICAL_SIZE){
                cameraSectionRow++;
                cameraScrollDirection=J_DOWN;
            }

        // If link is on the top edge
        }else if(linkScreenY<8){
            if(cameraSectionRow>0){
                cameraSectionRow--;
                cameraScrollDirection=J_UP;
            }
        }
    }else{
        
        // If the camera is not at it's desired y position
        if((cameraY>>4)!=cameraSectionRow*CAMERA_VERTICAL_SIZE){

            // Is the camera moving downward or upward?
            if((cameraY>>4)<cameraSectionRow*CAMERA_VERTICAL_SIZE){
                cameraY+=CAMERA_SCROLL_SPEED;
            }else {
                cameraY-=CAMERA_SCROLL_SPEED;
            }

            // Update the submap
            UpdateCameraSubMap();

        // If the camera is not at it's desired x position
        }else if((cameraX>>4)!=cameraSectionColumn*160){

            // Is the camera moving to the left or right?
            if((cameraX>>4)<cameraSectionColumn*160){
                cameraX+=CAMERA_SCROLL_SPEED;
            }else {
                cameraX-=CAMERA_SCROLL_SPEED;
            }

            // Update the submap
            UpdateCameraSubMap();
        }else{

            // We are not scrolling anymore
            cameraScrollDirection=0;
        }

    }
}