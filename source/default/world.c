#include <gb/gb.h>
#include "common.h"
#include "userinterface.h"
#include "graphics/HUD.h"


uint8_t worldTilemapBank=0;
uint8_t worldTileSetOffset=0;
uint8_t* worldTilemap=0;
uint8_t* worldTilemapAttributes=0;
uint8_t worldTilemapWidthInTiles=0;

void set_world_bkg_submap(uint8_t x, uint8_t y, uint8_t width, uint8_t height){

    // TODO switch rom bank for 'set_world_bkg_submap'

    // VBK_REG=1;
    // set_bkg_based_submap(x,y,width,height,worldTilemapAttributes,worldTilemapWidthInTiles,TILEMAP_TILES_START);
    // VBK_REG=0;
    set_bkg_based_submap(x,y,width,height,worldTilemap,worldTilemapWidthInTiles,TILEMAP_TILES_START);
}

void init_world(uint8_t bank, uint8_t* tilemap, uint8_t* tilemapAttributes, uint8_t width, uint8_t tilesetCount, uint8_t* tilesetTiles){
    worldTilemapBank=bank;
    worldTilemap=tilemap;
    worldTilemapAttributes=tilemapAttributes;
    worldTilemapWidthInTiles=width;
    
    set_bkg_data(TILEMAP_TILES_START,tilesetCount,tilesetTiles);
    set_world_bkg_submap(0,0,21,19);
}