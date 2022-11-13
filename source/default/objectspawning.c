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
                     

#define SPAWN_MOBLIN_AT(x,y,tile) SWITCH_ROM(BANK(NpcsBank));\
                            FAR_PTR updateMoblinFarPtr = to_far_ptr(UpdateMoblin,BANK(NpcsBank)); \
                            SpawnObject(MOBLIN_OBJECT_TYPE,x,y,J_DOWN,0,tile,updateMoblinFarPtr);   

#define SPAWN_MARIN_AT(x,y,tile) SWITCH_ROM(BANK(NpcsBank));\
                            FAR_PTR updateMarinFarPtr = to_far_ptr(UpdateMarin,BANK(NpcsBank)); \
                            SpawnObject(MOBLIN_OBJECT_TYPE,x,y,J_DOWN,0,tile,updateMarinFarPtr);   

#define SPAWN_CHEST_AT(x,y,tile) SWITCH_ROM(BANK(ChestBank));\
                            FAR_PTR updateChestFarPtr = to_far_ptr(UpdateChest,BANK(ChestBank)); \
                            SpawnObject(MOBLIN_OBJECT_TYPE,x,y,J_DOWN,0,tile,updateChestFarPtr);   


#define SPAWN_SWORD_AT(x,y,tile) SWITCH_ROM(BANK(SwordAloneBank));\
                            FAR_PTR updateSwordAloneFarPtr = to_far_ptr(UpdateSwordAlone,BANK(SwordAloneBank)); \
                            SpawnObject(MOBLIN_OBJECT_TYPE,x,y,J_DOWN,0,tile,updateSwordAloneFarPtr);   

void SpawnObjectsForArea() NONBANKED{

    _previous_bank = _current_bank;

    if(nextCameraSectionColumn>=0&&nextCameraSectionColumn<=1&&nextCameraSectionRow>=0&&nextCameraSectionRow<=1){

        uint8_t moblinTileIndex = 18;
        uint8_t marinTileIndex = moblinTileIndex+Moblin_TILE_COUNT;
        uint8_t chestTileIndex = marinTileIndex+NPCMarin_TILE_COUNT;
        uint8_t swordTileIndex = chestTileIndex+Chest_TILE_COUNT;

        set_banked_sprite_data(moblinTileIndex,Moblin);
        set_banked_sprite_data(marinTileIndex,NPCMarin);
        set_banked_sprite_data(chestTileIndex,Chest);
        set_banked_sprite_data(swordTileIndex,SwordAlone);

        if(nextCameraSectionColumn==0&&nextCameraSectionRow==0){

            SPAWN_CHEST_AT(60,40,chestTileIndex);

        }else if(nextCameraSectionColumn==1&&nextCameraSectionRow==0){
            
            SPAWN_MOBLIN_AT(120,40,18);

            uint8_t marinX = gotSword ? 64 : 80;
            uint8_t marinY = gotSword ? 84 : 71;

            SPAWN_MARIN_AT(marinX,marinY,marinTileIndex);

        }else if(nextCameraSectionColumn==1&&nextCameraSectionRow==1){

            if(!gotSword){

                SPAWN_SWORD_AT(90,70,swordTileIndex);

            }
        }
    }

    POP_BANK;

}