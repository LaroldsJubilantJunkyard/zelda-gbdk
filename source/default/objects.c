#include <gb/gb.h>
#include "camera.h"
#include "collision.h"

#include "link.h"
#include "common.h"
#include "objects.h"
#include "graphics/Explosion.h"
#include "graphics/Moblin.h"
#include "graphics/SwordSlashDown.h"
#include "graphics/NPCMarinDown.h"
#include "graphics/LinkSpritesDown.h"

extern uint8_t UpdateExplosion(Object* object, uint8_t sprite);
extern uint8_t UpdateLink(Object* object, uint8_t sprite);
extern uint8_t UpdateMoblin(Object* object, uint8_t sprite);
extern uint8_t UpdateMarin(Object* object,uint8_t sprite);

ObjectType objectTypes[]={
    {.active=FALSE,.bank=0,.width=16,.height=16,.tileCount=LinkSpritesDown_TILE_COUNT,.paddedTileCount=18,.tileData=LinkSpritesDown_tiles,.count=0,.maxHealth=3,.next=0,.startTile=0,.update=&UpdateLink},
    {.active=FALSE,.bank=0,.width=16,.height=16,.tileCount=Explosion_TILE_COUNT,.paddedTileCount=0,.tileData=Explosion_tiles,.count=0,.maxHealth=0,.next=0,.startTile=0,.update=&UpdateExplosion},
    {.active=FALSE,.bank=0,.width=12,.height=12,.tileCount=Moblin_TILE_COUNT,.paddedTileCount=0,.tileData=Moblin_tiles,.count=0,.maxHealth=3,.next=0,.startTile=0,.update=&UpdateMoblin},
    {.active=FALSE,.bank=0,.width=16,.height=16,.tileCount=NPCMarinDown_TILE_COUNT,.paddedTileCount=0,.tileData=NPCMarinDown_tiles,.count=0,.maxHealth=3,.next=0,.startTile=0,.update=&UpdateMarin},
    
};

Object objects[MAX_NUMBER_OF_OBJECTS];
Object* firstObject=0,*lastObject=0;
ObjectType* firstObjectType=0,*lastObjectType=0;

uint16_t universalBlinker=0;
uint16_t universalBlinkerFast=0;
uint8_t spriteVRamIsDirty=FALSE;

void UpdateSpriteVRam(){

    spriteVRamIsDirty=FALSE;

    ObjectType* currentObjectType = firstObjectType;

    uint8_t spriteVramCounter=0;

    while(currentObjectType!=0){

        // TODO SWITCH TO BANK

        // Save what tile we are starting at in vram
        currentObjectType->startTile=spriteVramCounter;

        // Update the sprite vram data
        set_sprite_data(spriteVramCounter,currentObjectType->tileCount,currentObjectType->tileData);

        // TODO SWITCH FROM BANK

        spriteVramCounter+=MAX(currentObjectType->tileCount,currentObjectType->paddedTileCount);

        // Continue on in the linked list
        currentObjectType=currentObjectType->next;
    }


}

void AddObjectToVRam(ObjectType* type){

    // If this type is already active
    if(type->active==TRUE){

        // Increase how many objects exist for it
        type->count++;
        return;
    }

    type->count=1;

    // Continue the linkedl ist
    if(firstObjectType==0)firstObjectType=type;
    if(lastObjectType!=0)lastObjectType->next=type;
    lastObjectType=type;

    // This type is now active
    type->active=TRUE;
    type->next=0;

    // We need to update vram
    spriteVRamIsDirty=TRUE;
}

void RemoveObjectFromVRam(ObjectType* type){

    // If this type isn't active, stop
    if(type->active==FALSE)return;

    // If there are more than 1
    if(type->count>1){

        // We just decrease by one
        type->count--;

        return;
    }

    if(firstObjectType==type)firstObjectType=type->next;

    ObjectType* currentObjectType = firstObjectType;

    while(currentObjectType!=0){

        // If this is the previous item
        if(currentObjectType->next==type){

            // Pass on our next item, instead of us
            currentObjectType->next=type->next;

            // If we are the last object, use the one before us
            if(lastObjectType==type){
                lastObjectType=currentObjectType;
            }

            break;
        }

        currentObjectType=currentObjectType->next;
    }

    // This type is no longer active
    type->active=FALSE;
}

Object* SpawnObject(ObjectType* type, uint16_t x, uint16_t y, uint8_t direction,uint8_t id){
    uint8_t i=0;
    
    Object* nextObject=0;

    for( i=0;i<MAX_NUMBER_OF_OBJECTS;i++){
        if(objects[i].active)continue;
        nextObject=&objects[i];
        break;
    }

    nextObject->originalArea=CAMERA_CURRENT_AREA;
    nextObject->recycle=FALSE;
    nextObject->id=id;
    nextObject->blinkTimer=0;
    nextObject->currentMetasprite=0;
    nextObject->damageX=0;
    nextObject->damageY=0;
    nextObject->next=0;
    nextObject->active=TRUE;
    nextObject->x=((cameraSectionColumn*160)<<4)+(x<<4);
    nextObject->y=((cameraSectionRow*CAMERA_VERTICAL_SIZE)<<4)+(y<<4);
    nextObject->helper1=0;
    nextObject->health=type->maxHealth;
    nextObject->type=type;
    nextObject->direction=direction;
    nextObject->explosion=-1;

    if(firstObject==0)firstObject=nextObject;
    if(lastObject!=0)lastObject->next=nextObject;
    lastObject=nextObject;

    if(type->active==FALSE){
        AddObjectToVRam(type);
    }

    return nextObject;
}

uint8_t RemoveWhenOutOffscreen(Object* object){

    // If the camera isn't scrolling, we're fine
    if(cameraScrollDirection!=0)return 0;

    // If the player is in this object's section, were fine
    if(object->originalArea==cameraSection)return 0;

    if((object->x>>4)<(cameraX>>4)-8){

        // This object needs to be recycled
        object->recycle=TRUE;

        return 0;
    }

    if((object->x>>4)>(cameraX>>4)+168){

        // This object needs to be recycled
        object->recycle=TRUE;

        return 0;
    }

    if((object->y>>4)<(cameraY>>4)-8){

        // This object needs to be recycled
        object->recycle=TRUE;

        return 0;
    }

    if((object->y>>4)>(cameraY>>4)+152){

        // This object needs to be recycled
        object->recycle=TRUE;

        return 0;
    }


    return 0;
}

void RecycleObject(Object* objectToBeRecycled, Object* previousObject){

    if(firstObject==objectToBeRecycled)firstObject=objectToBeRecycled->next;

    Object* currentObject = firstObject;


    if(previousObject!=0){
        previousObject->next=objectToBeRecycled->next;
        if(lastObject==objectToBeRecycled)lastObject=previousObject;
    }

    objectToBeRecycled->active=FALSE;
    objectToBeRecycled->next=0;
    objectToBeRecycled->x=30000;
    objectToBeRecycled->y=30000;

    RemoveObjectFromVRam(objectToBeRecycled->type);

}

uint8_t MoveToNextPosition(Object* object, uint16_t nextX, uint16_t nextY){
    uint8_t any=0;
    
    // Twice for both directions and a "sliding" behavior
    if(!IntersectsSolidWorld((nextX>>4)-5,(object->y>>4)-5,10,10)){
        object->x=nextX;
        any=1;
    }
    if(!IntersectsSolidWorld((object->x>>4)-5,(nextY>>4)-5,10,10)){
        object->y=nextY;
        any=1;
    }
    return any;
}

void StopLinkFromOverlapping(Object* object){
    if(CheckObjectIntersection3(object,nextX,link->y,10,12)){
        nextX=link->x;
    }
    if(CheckObjectIntersection3(object,link->x,nextY,10,12)){
        nextY=link->y;
    }
}


    #define DAMAGE_DECREASE_AMOUNT 6




uint8_t DamagedNoMove(Object* object,uint8_t sprite){
    if(object->damageY>DAMAGE_DECREASE_AMOUNT){
        object->damageY-=DAMAGE_DECREASE_AMOUNT;
    }else if(object->damageY<-DAMAGE_DECREASE_AMOUNT){
        object->damageY+=DAMAGE_DECREASE_AMOUNT;
    }else {
        object->damageY=0;
    }

    if(object->damageX>DAMAGE_DECREASE_AMOUNT){
        object->damageX-=DAMAGE_DECREASE_AMOUNT;
    }else if(object->damageX<-DAMAGE_DECREASE_AMOUNT){
        object->damageX+=DAMAGE_DECREASE_AMOUNT;
    }else {
        object->damageX=0;
    }

    return object->damageX!=0||object->damageY!=0;
}

uint8_t Damaged(Object* object,uint8_t sprite){

    MoveToNextPosition(object,object->x+object->damageX,object->y+object->damageY);

    return DamagedNoMove( object, sprite);
}
void UpdateAllObjects(){

    // This will be used for anything flashing/blinking from damage
    if((++universalBlinker>>4)>1)universalBlinker=0;
    universalBlinkerFast+=3;
    if((universalBlinkerFast>>4)>1)universalBlinkerFast=0;

    Object* previousObject = 0;
    Object* currentObject = firstObject;

    uint8_t sprite=0;

    while(currentObject!=0){

        // Update for our type
        // Pass in which sprite
        sprite+=currentObject->type->update(currentObject,sprite);

        Object* nextObject = currentObject->next;

        // If we should be recycled
        if(currentObject->recycle){

            // Reycle us, but don't change the previous
            RecycleObject(currentObject,previousObject);
        }else{
             
            // Update the previous
            previousObject=currentObject;
        }

        currentObject = nextObject;


    }

    sprite+=FinishLinkUpdate(sprite);

    // Update sprite vram after everything has updated
    if(spriteVRamIsDirty){
        UpdateSpriteVRam();
    }

    // Hide the rest
    hide_sprites_range(sprite,MAX_HARDWARE_SPRITES);

}