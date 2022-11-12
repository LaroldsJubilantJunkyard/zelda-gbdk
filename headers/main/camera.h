#include <gb/gb.h>
#include "graphics/BasicMap.h"

#define CAMERA_VERTICAL_SIZE (DEVICE_SCREEN_PX_HEIGHT-16)
#define CAMERA_SCROLL_SPEED 16

#define CAMERA_HORIZONTAL_SECTION_COUNT (BasicMap_WIDTH/DEVICE_SCREEN_PX_WIDTH)
#define CAMERA_CURRENT_AREA (nextCameraSectionColumn+nextCameraSectionRow*CAMERA_HORIZONTAL_SECTION_COUNT)

extern uint8_t cameraScrollDirection;
extern int16_t cameraX,cameraY,cameraTrueY,cameraTrueX;
extern uint16_t nextCameraSectionColumn,nextCameraSectionRow,currentCameraSectionColumn,currentCameraSectionRow,cameraSection;
extern uint16_t lastCameraX, lastCameraY;
extern uint16_t lastCameraRow, lastCameraColumn;

void SetupCamera(uint16_t x, uint16_t y, uint16_t column, uint16_t row);
uint8_t UpdateCamera();
void UpdateCameraSubMap();