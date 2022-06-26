#include <gb/gb.h>

#define CAMERA_VERTICAL_SIZE (144-16)
#define CAMERA_SCROLL_SPEED 16

extern uint8_t cameraScrollDirection;
extern uint16_t cameraX,cameraY;
extern uint16_t cameraSectionColumn,cameraSectionRow;
extern uint16_t lastCameraX, lastCameraY;
extern uint16_t lastCameraRow, lastCameraColumn;

void SetupCamera();
void UpdateCamera();
void UpdateCameraSubMap();