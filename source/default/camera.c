#include <gb/gb.h>
#include "camera.h"
#include "common.h"
#include "world.h"
#include "objects.h"
#include "graphics/HUD.h"
uint8_t cameraScrollDirection=0;
int16_t cameraX,cameraY,cameraTrueX,cameraTrueY;
uint16_t nextCameraSectionColumn,nextCameraSectionRow,currentCameraSectionColumn,currentCameraSectionRow, cameraSection;
uint16_t lastCameraX, lastCameraY;
uint16_t lastCameraRow, lastCameraColumn;


void vblank_isr() {
    move_bkg(cameraTrueX, cameraTrueY);
}

void SetupCamera(uint16_t x, uint16_t y, uint16_t column, uint16_t row){
    cameraX=x;
    cameraY=y;
    cameraTrueY=cameraY>>4;
    cameraTrueX=cameraX>>4;
    nextCameraSectionColumn=column;
    nextCameraSectionRow=row;
    currentCameraSectionColumn=column;
    currentCameraSectionRow=row;
    lastCameraColumn=column;
    lastCameraRow=row;
    lastCameraX=x;
    lastCameraY=y;
    cameraSection=CAMERA_CURRENT_AREA;

    disable_interrupts();
    add_VBL(vblank_isr);
    set_interrupts(VBL_IFLAG);
    enable_interrupts();


}

void UpdateCameraSubMap(){
    
    uint16_t cameraRow = (cameraTrueY>>3);
    uint16_t cameraColumn=(cameraTrueX>>3);

    // If our camera's column has changed
    if(lastCameraColumn!=cameraColumn){

        // Save which column we are in
        lastCameraColumn=cameraColumn;

        if(cameraX>lastCameraX){
            if ((worldTilemapWidthInTiles- 20u) > cameraColumn) set_world_bkg_submap(cameraColumn + 20u, cameraRow, 1, MIN(19u, worldTilemapHeightInTiles- cameraRow));  

        }else{
            set_world_bkg_submap(cameraColumn, cameraRow, 1, MIN(19u, worldTilemapHeightInTiles - cameraRow));   

        }
    }

    // If our camera's row has changed
    if(lastCameraRow!=cameraRow){

        // Save which row we are in
        lastCameraRow=cameraRow;

        if(cameraY>lastCameraY){
            if ((worldTilemapHeightInTiles - 18u) > cameraRow) set_world_bkg_submap(cameraColumn, cameraRow + 18u, MIN(21u, worldTilemapWidthInTiles-cameraColumn), 1); 

        }else{

            set_world_bkg_submap(cameraColumn, cameraRow, MIN(21u, worldTilemapWidthInTiles-cameraColumn), 1);
        }
    }

    // Update our camera position
    lastCameraX=cameraX;
    lastCameraY=cameraY;
}

void StopCameraScrolling(){

    // We are not scrolling anymore
    cameraScrollDirection=0;
    cameraSection=CAMERA_CURRENT_AREA;

    currentCameraSectionColumn=nextCameraSectionColumn;
    currentCameraSectionRow=nextCameraSectionRow;
}

void StartCameraScrolling(uint8_t direction){

    nextCameraSectionColumn+=J_DIRECTIONS[direction][0];
    nextCameraSectionRow+=J_DIRECTIONS[direction][1];
    cameraScrollDirection=direction;
}

uint8_t UpdateCamera(){

    // If the camera is not scrolling?
    if(cameraScrollDirection==0){

        int16_t linkScreenX= (link->trueX)-(cameraTrueX);

        // If link is on the right edge
        if(linkScreenX>(DEVICE_SCREEN_PX_WIDTH-8)){

            // If we are not on right section of the world
            if(nextCameraSectionColumn<worldTilemapHeightInTiles/20){

                StartCameraScrolling(J_RIGHT);

                return CAMERA_STARTED_SCROLLING;
            }

        // If link is on the left edge
        }else if(linkScreenX<8){

            // If we are not on left section of the world
            if(nextCameraSectionColumn>0){

                StartCameraScrolling(J_LEFT);

                return CAMERA_STARTED_SCROLLING;
            }
        }

        int16_t linkScreenY= (link->trueY)-(cameraTrueY);

        // If link is on the bottom edge
        if(linkScreenY>CAMERA_VERTICAL_SIZE){

            // If we are not on bottom section of the world
            if(nextCameraSectionRow<worldTilemapHeightInTiles>>4){

                StartCameraScrolling(J_DOWN);

                return CAMERA_STARTED_SCROLLING;
            }

        // If link is on the top edge
        }else if(linkScreenY<0){

            // If we are not on top section of the world
            if(nextCameraSectionRow>0){

                StartCameraScrolling(J_UP);

                return CAMERA_STARTED_SCROLLING;
            }
        }
    }else{

        int16_t targetCameraX = nextCameraSectionColumn*DEVICE_SCREEN_PX_WIDTH;
        int16_t targetCameraY = nextCameraSectionRow*CAMERA_VERTICAL_SIZE;
            
        // If the camera is not at it's desired x or y position
        if(cameraTrueY!=targetCameraY||cameraTrueX!=targetCameraX){

            cameraX+=J_DIRECTIONS[cameraScrollDirection][0]*CAMERA_SCROLL_SPEED;
            cameraY+=J_DIRECTIONS[cameraScrollDirection][1]*CAMERA_SCROLL_SPEED;
            cameraTrueY=cameraY>>4;
            cameraTrueX=cameraX>>4;

            UpdateCameraSubMap();

            return CAMERA_SCROLLING;
        }else{

            StopCameraScrolling();

            return CAMERA_STOPPED_SCROLLING;
        }

    }

    return CAMERA_STEADY;
}