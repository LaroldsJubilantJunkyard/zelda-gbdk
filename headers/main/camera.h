#include <gb/gb.h>
#include "graphics/TileMap.h"

#define CAMERA_VERTICAL_SIZE (144-16)
#define CAMERA_SCROLL_SPEED 16

#define CAMERA_HORIZONTAL_SECTION_COUNT (TileMap_WIDTH/160)
#define CAMERA_CURRENT_AREA (cameraSectionColumn+cameraSectionRow*CAMERA_HORIZONTAL_SECTION_COUNT)

extern uint8_t cameraScrollDirection;
extern uint16_t cameraX,cameraY,cameraSection;
extern uint16_t cameraSectionColumn,cameraSectionRow;
extern uint16_t lastCameraX, lastCameraY;
extern uint16_t lastCameraRow, lastCameraColumn;

void SetupCamera();
uint8_t UpdateCamera();
void UpdateCameraSubMap();