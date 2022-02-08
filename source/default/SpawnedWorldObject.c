#include <gb/gb.h>
#include "SpawnedWorldObject.h"
#include "common.h"

uint8_t numberOfActiveObjects=0;
SpawnedWorldObject sortedSpawnedObjects[MAX_SPAWNED_OBJECTS_ONSCREEN];


void DefaultSpawnedWorldObject(SpawnedWorldObject *obj){
    obj->active=0;
    obj->update=0;
    obj->old=0;
}

void CopySpawnedWorldObject(SpawnedWorldObject *src, SpawnedWorldObject *dest){
    
    dest->active=src->active;
    dest->x=src->x;
    dest->y=src->y;
    dest->old=src->old;
    dest->index=src->index;
    dest->health=src->health;
    dest->frame=src->frame;
    dest->update=src->update;
    dest->hitX=src->hitX;
    dest->hitY=src->hitY;
    dest->firstMetasprite=src->firstMetasprite;
    dest->direction=src->direction;
    dest->startSprite=src->startTile;
    dest->startTile=src->startSprite;
    dest->active=src->active;
}

void ReOrderSpawnedWorldObjects(){

    SpawnedWorldObject temp[MAX_SPAWNED_OBJECTS_ONSCREEN];

    uint8_t counter=0;

    
    for(uint8_t i=0;i<MAX_SPAWNED_OBJECTS_ONSCREEN;i++){
        temp[i].active=0;
    }

    for(uint8_t i=0;i<MAX_SPAWNED_OBJECTS_ONSCREEN;i++){

        if(temp[i].active){
            CopySpawnedWorldObject(&sortedSpawnedObjects[i],&temp[counter++]);
        }
    }

    
    for(uint8_t i=0;i<counter;i++){
        CopySpawnedWorldObject(&temp[counter],&sortedSpawnedObjects[i]);
    }

}
uint8_t IsSpawnedWorldObjectInView(SpawnedWorldObject* spawnedWorldObject){
    return (spawnedWorldObject->x<<4)>backgroundLeftX&&(spawnedWorldObject->y<<4)>backgroundTopY&&(spawnedWorldObject->y<<4)<backgroundTopY+128&&(spawnedWorldObject->x<<4)<backgroundLeftX+160;
}

void UpdateAllSpawnedWorldObjects(){


    uint8_t reOrder=FALSE;

    for(uint8_t i=0;i<MAX_SPAWNED_OBJECTS_ONSCREEN;i++){

        SpawnedWorldObject* spawnedWorldObject = &sortedSpawnedObjects[i];

        if(spawnedWorldObject->active==0)break;

        if(spawnedWorldObject->old){

            // If this spawnedWorldObject is not in view
            // Decativate this spawnedWorldObject
            if(!IsSpawnedWorldObjectInView(spawnedWorldObject)){
                spawnedWorldObject->active=FALSE;
            }
        }else{

            // Call it's update function
            spawnedWorldObject->update(spawnedWorldObject);
        }


        uint16_t screenX = (spawnedWorldObject->x>>4) -backgroundLeftX;
        uint16_t screenY=(spawnedWorldObject->y>>4)-backgroundTopY;
        
        // hide or show the metasprites
        if(spawnedWorldObject->active==FALSE){
            hide_metasprite(spawnedWorldObject->firstMetasprite+spawnedWorldObject->frame,spawnedWorldObject->startSprite);
            reOrder=TRUE;
        }else move_metasprite(spawnedWorldObject->firstMetasprite+spawnedWorldObject->frame,spawnedWorldObject->startTile,spawnedWorldObject->startSprite,screenX+8,screenY+16);
    }


    if(reOrder){
        ReOrderSpawnedWorldObjects();
    }
}