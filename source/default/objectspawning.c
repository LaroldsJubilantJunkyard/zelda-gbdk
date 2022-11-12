#include <gb/gb.h>
#include "objects.h"
#include "common.h"
#include "camera.h"
#include "graphics/NPCMarin.h"
#include "graphics/Moblin.h"
#include "graphics/Chest.h"
#include "graphics/SwordAlone.h"
#include "graphics/SwordSlashDown.h"
#include "graphics/LinkSpritesDown.h"


BANKREF_EXTERN(Moblin)
BANKREF_EXTERN(SwordAlone)
BANKREF_EXTERN(SwordAloneBank)
BANKREF_EXTERN(Chest)
BANKREF_EXTERN(ChestBank)
BANKREF_EXTERN(NPCMarin)
BANKREF_EXTERN(NpcsBank)
BANKREF_EXTERN(EnemiesBank)

extern void UpdateSwordAlone() BANKED;
extern void UpdateChest() BANKED;
extern void UpdateMoblin() BANKED;
extern void UpdateMarin() BANKED;

extern uint8_t gotSword;

void SpawnObjectsForArea() NONBANKED{

    
    if(nextCameraSectionColumn==1&&nextCameraSectionRow==0){

        PUSH_NAMED_BANK(Moblin);

        set_sprite_data(18,Moblin_TILE_COUNT,Moblin_tiles);


        SWITCH_ROM(BANK(NPCMarin));

        set_sprite_data(18+Moblin_TILE_COUNT,NPCMarin_TILE_COUNT,NPCMarin_tiles);

        SWITCH_ROM(BANK(Moblin));

        FAR_PTR updateMoblinFarPtr = to_far_ptr(UpdateMoblin,BANK(Moblin));
        
        SpawnObject(MOBLIN_OBJECT_TYPE,120,40,J_DOWN,0,18,updateMoblinFarPtr);

        SWITCH_ROM(BANK(NPCMarin));

        FAR_PTR updateMarinFarPtr = to_far_ptr(UpdateMarin,BANK(NPCMarin));
        uint8_t marinX = 80;
        uint8_t marinY = 71;

        if(gotSword){
            marinX=64;
            marinY=84;
        }

        SpawnObject(MARIN_OBJECT_TYPE,marinX,marinY,J_DOWN,0,18+Moblin_TILE_COUNT,updateMarinFarPtr);
        

    }else if(nextCameraSectionColumn==0&&nextCameraSectionRow==0){

        PUSH_NAMED_BANK(Chest);

        set_sprite_data(18+Moblin_TILE_COUNT+NPCMarin_TILE_COUNT,Chest_TILE_COUNT,Chest_tiles);

        SWITCH_ROM(BANK(ChestBank));

        FAR_PTR updateChestFarPtr = to_far_ptr(UpdateChest,BANK(ChestBank));
        
        SpawnObject(MOBLIN_OBJECT_TYPE,60,40,J_DOWN,0,18+Moblin_TILE_COUNT+NPCMarin_TILE_COUNT,updateChestFarPtr);


    }else if(nextCameraSectionColumn==1&&nextCameraSectionRow==1){

        if(!gotSword){

            PUSH_NAMED_BANK(SwordAlone);

            set_sprite_data(18+Moblin_TILE_COUNT+NPCMarin_TILE_COUNT,SwordAlone_TILE_COUNT,SwordAlone_tiles);

            SWITCH_ROM(BANK(SwordAloneBank));

            FAR_PTR updateSwordAloneFarPtr = to_far_ptr(UpdateSwordAlone,BANK(SwordAloneBank));
            
            SpawnObject(MOBLIN_OBJECT_TYPE,90,70,J_DOWN,0,18+Moblin_TILE_COUNT+NPCMarin_TILE_COUNT,updateSwordAloneFarPtr);

        }
    }

    POP_BANK;

}