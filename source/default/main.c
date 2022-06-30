#include <gb/gb.h>
#include <gb/cgb.h>
#include "camera.h"
#include "objects.h"
#include "objectspawning.h"
#include "link.h"
#include "userinterface.h"
#include "common.h"
#include "graphics/TileSet.h"
#include "graphics/TileMap.h"
#include "graphics/LinkSwordSpritesDown.h"
#include "graphics/HUD.h"
#include "graphics/AlphaNumeric.h"

Object* link=0;

void main(){
    

    DISPLAY_ON;
    SPRITES_8x16;
    SHOW_SPRITES;
    SHOW_BKG;
    SHOW_WIN;

    move_win(7,128);

    set_sprite_palette(0,LinkSwordSpritesDown_PALETTE_COUNT,LinkSwordSpritesDown_palettes);
    set_bkg_palette(0,HUD_PALETTE_COUNT,HUD_palettes);
    set_bkg_data(0,HUD_TILE_COUNT,HUD_tiles);
    set_bkg_data(TILEMAP_TILES_START,TileSet_TILE_COUNT,TileSet_tiles);
    set_bkg_data(HUD_TILE_COUNT,TileSet_TILE_COUNT,TileSet_tiles);
    set_bkg_based_submap(0,0,21,19,TileMap_map,TileMap_WIDTH/8,TILEMAP_TILES_START);

    SetupCamera();

    link=SpawnObject(LINK_OBJECT_TYPE,40,80,J_DOWN,0);

    SpawnObjectsForArea();

    link->health=linkMaxHealth;

    UpdateInterfaceFull();

    DrawText("Your name is not zelda. Your name is in fact link. I dont get why",TRUE);

    while(TRUE){

        joypadPrevious = joypadCurrent;
        joypadCurrent = joypad();

        UpdateAllObjects();

        // This function returns true, when we JUST started scrolling
        // If we just started scrolling to a new area
        if(UpdateCamera()){

            // Spawn objects for that area
            SpawnObjectsForArea();
        }

        wait_vbl_done();
    }

}