#include <gb/gb.h>
#include <gb/cgb.h>
#include "camera.h"
#include "objects.h"
#include "objectspawning.h"
#include "link.h"
#include "world.h"
#include "userinterface.h"
#include "common.h"
#include "graphics/MapSource.h"
#include "graphics/BasicMap.h"
#include "graphics/LinkSwordSpritesDown.h"
#include "graphics/LinkSpritesDown.h"
#include "graphics/HUD.h"
#include "graphics/AlphaNumeric.h"


Object* link=0;
BANKREF_EXTERN(LinkSpritesDown)
BANKREF_EXTERN(LinkSwordSpritesDown)
BANKREF_EXTERN(HUD)



void main() NONBANKED{
    

    DISPLAY_ON;
    SPRITES_8x16;
    SHOW_SPRITES;
    SHOW_BKG;
    SHOW_WIN;

    // This enables Sound
    // these registers must be in this specific order!
    NR52_REG = 0x80; // is 1000 0000 in binary and turns on sound
    NR50_REG = 0x77; // sets the volume for both left and right channel just set to max 0x77
    NR51_REG = 0xFF; // is 1111 1111 in binary, select which chanels we want to use in this case all of them. One bit for the L one bit for the R of all four channels

    move_win(7,128);

    SWITCH_ROM(BANK(LinkSwordSpritesDown));
    set_sprite_palette(0,LinkSwordSpritesDown_PALETTE_COUNT,LinkSwordSpritesDown_palettes);

    SWITCH_ROM(BANK(HUD));
    set_bkg_palette(0,HUD_PALETTE_COUNT,HUD_palettes);
    set_bkg_data(0,HUD_TILE_COUNT,HUD_tiles);

    SWITCH_ROM(BANK(LinkSpritesDown));
    set_sprite_data(0,LinkSpritesDown_TILE_COUNT,LinkSpritesDown_tiles);



    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*
     * Our tilemap and tileset may be in differnet banks. We need to switch to their banks, then get & save far pointers to
     * those tilemaps and tilesets. Normal primitive values can be passed as-is, but far pointers must be used in case of 
     * banked pointers.
     */
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    PUSH_NAMED_BANK(BasicMap);

    FAR_PTR tileMapFarPtr = TO_FAR_PTR(BasicMap_map,BANK(BasicMap));
    FAR_PTR tileMapAttributesFarPtr = TO_FAR_PTR(BasicMap_map_attributes,BANK(BasicMap));

    PUSH_NAMED_BANK(MapSource);
    
    FAR_PTR tileSetFarPtr = TO_FAR_PTR(MapSource_tiles,BANK(MapSource));

    init_world(

        // Which tile map 
        tileMapFarPtr,tileMapAttributesFarPtr,BasicMap_WIDTH>>3,BasicMap_HEIGHT>>3,
        
        // Which tileset
        MapSource_TILE_COUNT,tileSetFarPtr,19,

        // What area (column & row, where each column is 160px wide and each row is 128px high)
        0,0,

        // Where to place the player (respective to the starter area), and what direction to face.
        50,90,J_DOWN);

    POP_BANK;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    link->health=linkMaxHealth;

    UpdateInterfaceFull();



    while(TRUE){

        joypadPrevious = joypadCurrent;
        joypadCurrent = joypad();

        UpdateAllObjects();

        uint8_t cameraState = 0;

        // Update our camera and get it's state
        cameraState=UpdateCamera();

        // If it just started scrolling
        if(cameraState==CAMERA_STARTED_SCROLLING){
            
            RecycleOutOfScreenObjects();

            // Spawn objects for that area
            SpawnObjectsForArea();

        // If it just stopped scrolling
        }else if(cameraState==CAMERA_STOPPED_SCROLLING){
            
            RecycleOutOfScreenObjects();

        }

        wait_vbl_done();

       
    }

}