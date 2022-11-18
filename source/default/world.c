#include <gb/gb.h>
#include "common.h"
#include "camera.h"
#include "link.h"
#include "userinterface.h"
#include "graphics/HUD.h"

uint8_t worldTilemapHeightInTiles=0;
uint8_t worldNonSolidTileCount=0;
uint8_t worldTileSetOffset=0;
uint8_t worldTilemapWidthInTiles=0;

FAR_PTR worldTilemap=0;
FAR_PTR worldTilemapAttributes=0;

BANKREF_EXTERN(LinkBank)
extern uint8_t UpdateLink(Object* object, uint8_t sprite) BANKED;

void set_world_bkg_submap(uint8_t x, uint8_t y, uint8_t width, uint8_t height) NONBANKED{

    uint8_t previous_bank = _current_bank;

    SWITCH_ROM(FAR_SEG(worldTilemapAttributes));
    
    uint8_t* worldTileMapAttributesTemp = (uint8_t*)FAR_OFS(worldTilemapAttributes);
    uint8_t* worldTileMapTemp = (uint8_t*)FAR_OFS(worldTilemap);

    VBK_REG=1;
    set_bkg_submap(x,y,width,height,worldTileMapAttributesTemp,worldTilemapWidthInTiles);

    VBK_REG=0;
    set_bkg_based_submap(x,y,width,height,worldTileMapTemp,worldTilemapWidthInTiles,TILEMAP_TILES_START);

    SWITCH_ROM(previous_bank);
}

uint8_t SetBackgroundTileUnderWorldPosition(int16_t left, int16_t top,int8_t offsetColumn,int8_t offsetRow,uint8_t value) NONBANKED{

    
    int16_t screenLeft = ((left-(cameraTrueX)))+SCX_REG;
    int16_t screenTop =  ((top-(cameraTrueY)))+SCY_REG;

    uint8_t column23 = (screenLeft>>3);
    uint8_t row23 = (screenTop>>3);

    set_bkg_tile_xy(column23+offsetColumn,row23+offsetRow,value);
}

uint8_t GetWorldPositionIsSolid(int16_t left, int16_t top) NONBANKED{

    
    int16_t screenLeft = ((left-(cameraTrueX)))+SCX_REG;
    int16_t screenTop =  ((top-(cameraTrueY)))+SCY_REG;

    uint8_t column23 = (screenLeft>>3);
    uint8_t row23 = (screenTop>>3);

    return get_bkg_tile_xy(column23,row23)>worldNonSolidTileCount+HUD_TILE_COUNT;
}
uint8_t GetBackgroundTileUnderWorldPosition(int16_t left, int16_t top) NONBANKED{

    
    int16_t screenLeft = ((left-(cameraTrueX)))+SCX_REG;
    int16_t screenTop =  ((top-(cameraTrueY)))+SCY_REG;

    uint8_t column23 = (screenLeft>>3);
    uint8_t row23 = (screenTop>>3);

    return get_bkg_tile_xy(column23,row23);
}

void init_world(
     FAR_PTR tilemap, FAR_PTR tilemapAttributes, uint8_t width, uint8_t height,
     uint8_t tilesetCount, FAR_PTR tilesetTiles,uint8_t solidTileCount,
    uint16_t cameraNewSectionColumn, uint16_t cameraNewSectionRow,
    uint16_t linkRelativeX, uint16_t linkRelativeY, uint8_t linkDirection) NONBANKED{

worldNonSolidTileCount=solidTileCount;
    worldTilemap=tilemap;
    worldTilemapAttributes=tilemapAttributes;
    worldTilemapWidthInTiles=width;
    worldTilemapHeightInTiles=height;

    _previous_bank = _current_bank;

    SWITCH_ROM(FAR_SEG(tilesetTiles));

    uint8_t* worldTilesetTemp = (uint8_t*)FAR_OFS(tilesetTiles);
    
    set_bkg_data(TILEMAP_TILES_START,tilesetCount,worldTilesetTemp);


    set_world_bkg_submap(0,0,21,19);


    SetupCamera(cameraNewSectionColumn*DEVICE_SCREEN_PX_WIDTH,cameraNewSectionRow*CAMERA_VERTICAL_SIZE,cameraNewSectionColumn,cameraNewSectionRow);
    ResetAllObjects();
    
    SWITCH_ROM(BANK(LinkBank));
    FAR_PTR updateLinkFarPtr = TO_FAR_PTR(UpdateLink,BANK(LinkBank));

    link=SpawnObject(LINK_OBJECT_TYPE,linkRelativeX,linkRelativeY,linkDirection,0,0,updateLinkFarPtr);
    nextX=link->x;
    nextY=link->y;
    trueNextX=nextX>>4;
    trueNextY=nextY>>4;

    POP_BANK;
    
    SpawnObjectsForArea();

}