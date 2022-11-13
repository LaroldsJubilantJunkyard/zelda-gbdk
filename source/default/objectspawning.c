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

#define CONCATENATE_VALUES(A,B) A ## B

#define set_banked_sprite_data(offset,OBJECT)SWITCH_ROM(BANK(OBJECT));\
                              set_sprite_data(offset,CONCATENATE_VALUES(OBJECT, _TILE_COUNT),CONCATENATE_VALUES(OBJECT, _tiles));

                        

void SpawnObjectsForArea() NONBANKED{

    _previous_bank = _current_bank;

    if(nextCameraSectionColumn>=0&&nextCameraSectionColumn<=1&&nextCameraSectionRow>=0&&nextCameraSectionRow<=1){

        set_banked_sprite_data(18,Moblin);
        set_banked_sprite_data(18+Moblin_TILE_COUNT,NPCMarin);
        set_banked_sprite_data(18+Moblin_TILE_COUNT+NPCMarin_TILE_COUNT,Chest);
        set_banked_sprite_data(18+Moblin_TILE_COUNT+NPCMarin_TILE_COUNT+Chest_TILE_COUNT,SwordAlone);

        if(nextCameraSectionColumn==0&&nextCameraSectionRow==0){

            SWITCH_ROM(BANK(ChestBank));

            FAR_PTR updateChestFarPtr = to_far_ptr(UpdateChest,BANK(ChestBank));
            
            SpawnObject(MOBLIN_OBJECT_TYPE,60,40,J_DOWN,0,18+Moblin_TILE_COUNT+NPCMarin_TILE_COUNT,updateChestFarPtr);
        }

        
        else if(nextCameraSectionColumn==1&&nextCameraSectionRow==0){

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
            



        }else if(nextCameraSectionColumn==1&&nextCameraSectionRow==1){

            if(!gotSword){

                SWITCH_ROM(BANK(SwordAloneBank));

                FAR_PTR updateSwordAloneFarPtr = to_far_ptr(UpdateSwordAlone,BANK(SwordAloneBank));
                
                SpawnObject(MOBLIN_OBJECT_TYPE,90,70,J_DOWN,0,18+Moblin_TILE_COUNT+NPCMarin_TILE_COUNT+Chest_TILE_COUNT,updateSwordAloneFarPtr);

            }
        }
    }

    POP_BANK;

}