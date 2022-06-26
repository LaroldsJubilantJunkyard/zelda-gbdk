#include <gb/gb.h>
#include <gb/cgb.h>
#include "camera.h"
#include "objects.h"
#include "common.h"
#include "graphics/TileSet.h"
#include "graphics/TileMap.h"

Object* link=0;

void main(){
    

    DISPLAY_ON;
    SPRITES_8x16;
    SHOW_SPRITES;
    SHOW_BKG;
    SHOW_WIN;

    move_win(7,128);

    set_sprite_palette(0,1,TileSet_palettes);
    set_bkg_palette(0,1,TileSet_palettes);
    set_bkg_data(0,TileSet_TILE_COUNT,TileSet_tiles);
    set_bkg_submap(0,0,20,18,TileMap_map,TileMap_WIDTH/8);

    SetupCamera();

    link=SpawnObject(LINK_OBJECT_TYPE,40<<4,80<<4,J_DOWN,0);
    SpawnObject(MOBLIN_OBJECT_TYPE,120<<4,40<<4,J_DOWN,0);

    while(TRUE){

        joypadPrevious = joypadCurrent;
        joypadCurrent = joypad();

        UpdateAllObjects();
        UpdateCamera();

        wait_vbl_done();
    }

}