#include <gb/gb.h>
#include <gb/cgb.h>
#include "Link.h"
#include "World.h"
#include "Common.h"
#include "HeadsUpDisplay.h"
#include "NPC.h"
#include "Enemy.h"
#include "SpawnedWorldObject.h"

uint8_t blinkTimer=0;
extern NPC npcs[];
extern Enemy enemies[];
extern Link link;

void main(void){

    DISPLAY_ON;
    SPRITES_8x16;
    SHOW_SPRITES;
    SHOW_BKG;
    SHOW_WIN;

    move_win(7,126);

    SetupLink();
    SetupHeadsUpDisplay();
    SetupWorld();
    SetupEnemies();
    SetupNPCs();
    ActivateEnemy(&enemies[0],3,80,80);
    ActivateNPC(&npcs[0],48,32);

    blinkTimer=0;
    blinkDamaged=0;

    // Infinite loop
    while(TRUE){
        blinkTimer++;
        if(blinkTimer>4){
            blinkTimer=0;
            blinkDamaged=!blinkDamaged;
        }

        // Save the state of the joypad
        joypadPrevious = joypadCurrent;
        joypadCurrent = joypad();

        UpdateWorld();
        UpdateLink();
        UpdateAllSpawnedWorldObjects();
        //UpdateAllNPCs();
        //UpdateAllEnemies();

        wait_vbl_done();

    }

    

}