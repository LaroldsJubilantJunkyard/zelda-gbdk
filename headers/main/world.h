#ifndef WORLD_HEADER
#define WORLD_HEADER set

#include <gbdk/far_ptr.h>

extern uint8_t worldNonSolidTileCount;
extern uint8_t worldTilemapWidthInTiles;
extern uint8_t worldTilemapHeightInTiles;

void init_world(
     FAR_PTR tilemap, FAR_PTR tilemapAttributes, uint8_t width, uint8_t height,
     uint8_t tilesetCount, FAR_PTR tilesetTiles,uint8_t solidTileCount,
    uint16_t cameraNewSectionColumn, uint16_t cameraNewSectionRow,
    uint16_t linkRelativeX, uint16_t linkRelativeY, uint8_t linkDirection) NONBANKED;
void set_world_bkg_submap(uint8_t x, uint8_t y, uint8_t width, uint8_t height);


uint8_t GetWorldPositionIsSolid(int16_t left, int16_t top) NONBANKED;
uint8_t GetBackgroundTileUnderWorldPosition(int16_t left,uint16_t top) NONBANKED;
uint8_t SetBackgroundTileUnderWorldPosition(int16_t left, int16_t top,int8_t offsetColumn,int8_t offsetRow,uint8_t value) NONBANKED;

#endif