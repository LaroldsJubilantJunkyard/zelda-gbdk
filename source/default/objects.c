#include <gb/gb.h>
#include <gbdk/far_ptr.h>
#include "camera.h"
#include "collision.h"

#include "link.h"
#include "common.h"
#include "objects.h"
#include "graphics/Explosion.h"
#include "graphics/Moblin.h"
#include "graphics/NPCMarin.h"
#include "graphics/SwordSlashDown.h"
#include "graphics/LinkSpritesDown.h"


Object objects[MAX_NUMBER_OF_OBJECTS];
Object *firstObject = 0, *lastObject = 0;

uint16_t universalBlinker = 0;
uint16_t universalBlinkerFast = 0;
uint16_t universalBlinkerTrue = 0;
uint16_t universalBlinkerFastTrue = 0;
uint8_t spriteVRamIsDirty = FALSE;

BANKREF_EXTERN(LinkBank)

void ResetAllObjects()
{

    for (uint8_t i = 0; i < MAX_NUMBER_OF_OBJECTS; i++)
    {
        objects[i].next = 0;
        objects[i].active = 0;
    }

    lastObject = 0;
    firstObject = 0;
}

Object *SpawnObject( int16_t x, int16_t y, uint8_t direction, uint8_t id, uint8_t baseTileIndex, FAR_PTR updateFarPtr)
{
    uint8_t i = 0;

    Object *nextObject = 0;

    for (i = 0; i < MAX_NUMBER_OF_OBJECTS; i++)
    {
        if (objects[i].active)
            continue;
        nextObject = &objects[i];
        break;
    }

    nextObject->updateFarPtr = updateFarPtr;
    nextObject->baseTileIndex = baseTileIndex;
    nextObject->originalArea = CAMERA_CURRENT_AREA;
    nextObject->recycle = FALSE;
    nextObject->id = id;
    nextObject->currentMetasprite = 0;
    nextObject->damageX = 0;
    nextObject->damageY = 0;
    nextObject->next = 0;
    nextObject->active = TRUE;
    nextObject->x = ((nextCameraSectionColumn * 160) << 4) + (x << 4);
    nextObject->y = ((nextCameraSectionRow * CAMERA_VERTICAL_SIZE) << 4) + (y << 4);
    nextObject->trueX = nextObject->x >> 4;
    nextObject->trueY = nextObject->y >> 4;
    nextObject->helper1 = 0;
    nextObject->health = 3;
    nextObject->direction = direction;
    nextObject->explosion = -1;

    if (firstObject == 0)
        firstObject = nextObject;
    if (lastObject != 0)
        lastObject->next = nextObject;
    lastObject = nextObject;

    return nextObject;
}

void SetObjectY(Object *object, int16_t amount)
{
    object->y = amount;
    object->trueY = object->y >> 4;
}
void SetObjectX(Object *object, int16_t amount)
{
    object->x = amount;
    object->trueX = object->x >> 4;
}

void RecycleOutOfScreenObjects()
{

    for (uint8_t i = 0; i < MAX_NUMBER_OF_OBJECTS; i++)
    {
        if (!objects[i].active)
            continue;
        RemoveWhenOutOffscreen(&objects[i]);
    }
}

uint8_t RemoveWhenOutOffscreen(Object *object)
{

    // If the camera isn't scrolling, we're fine
    if (cameraScrollDirection != 0)
        return 0;

    // If the player is in this object's section, were fine
    if (object->originalArea == cameraSection)
        return 0;

    if ((object->trueX) < (cameraTrueX)-8)
    {

        // This object needs to be recycled
        object->recycle = TRUE;

        return 0;
    }

    if ((object->trueX) > (cameraTrueX) + 168)
    {

        // This object needs to be recycled
        object->recycle = TRUE;

        return 0;
    }

    if ((object->trueY) < (cameraTrueY)-8)
    {

        // This object needs to be recycled
        object->recycle = TRUE;

        return 0;
    }

    if ((object->trueY) > (cameraTrueY) + 152)
    {

        // This object needs to be recycled
        object->recycle = TRUE;

        return 0;
    }

    return 0;
}

void RecycleObject(Object *objectToBeRecycled, Object *previousObject)
{

    if (firstObject == objectToBeRecycled)
        firstObject = objectToBeRecycled->next;

    Object *currentObject = firstObject;

    if (previousObject != 0)
    {
        previousObject->next = objectToBeRecycled->next;
        if (lastObject == objectToBeRecycled)
            lastObject = previousObject;
    }

    objectToBeRecycled->active = FALSE;
    objectToBeRecycled->next = 0;
    objectToBeRecycled->x = 30000;
    objectToBeRecycled->y = 30000;
}

uint8_t CheckLinkInteractionWithObject(Object *currentObject,int16_t width,int16_t height) NONBANKED
{

    if ((joypadCurrent & J_A) && !(joypadPrevious & J_A))
    {

        uint16_t lx = (link->trueX) + (J_DIRECTIONS[link->direction][0] * 16);
        uint16_t ly = (link->trueY) + (J_DIRECTIONS[link->direction][1] * 16);

        if (CheckObjectIntersectionAgainstRectangle(currentObject,width,height, lx, ly, 16, 16))
        {

            return 1;
        }
    }

    return 0;
}

uint8_t MoveToNextPosition(Object *object, int16_t nextXTemp, int16_t nextYTemp, int16_t trueNextXTemp, int16_t trueNextYTemp) NONBANKED
{
    uint8_t any = 1;

    


    if(object->x!=nextXTemp){
        // Twice for both directions and a "sliding" behavior
        if (!IntersectsSolidWorld(trueNextXTemp-5, object->trueY-5, 10, 10))
        {
            object->x = nextXTemp;
            object->trueX = trueNextXTemp;
            any = 1;
        }
    }

    if(object->y!=trueNextYTemp){
        
        if (!IntersectsSolidWorld(object->trueX-5, trueNextYTemp-5, 10, 10))
        {
            object->y = nextYTemp;
            object->trueY = trueNextYTemp;
            any = 1;
        }
    }

    return any;
}

void StopLinkFromOverlapping(Object *object) NONBANKED
{
    if (CheckObjectIntersectionAgainstRectangle(object,16,16, trueNextX, link->trueY, 10, 12))
    {
        nextX = link->x;
        trueNextX = nextX >> 4;
    }
    if (CheckObjectIntersectionAgainstRectangle(object,16,16, link->trueX, trueNextY, 10, 12))
    {
        nextY = link->y;
        trueNextY = nextY >> 4;
    }
}

#define DAMAGE_DECREASE_AMOUNT 6

uint8_t DamagedNoMove(Object *object)
{
    if (object->damageY > DAMAGE_DECREASE_AMOUNT)
    {
        object->damageY -= DAMAGE_DECREASE_AMOUNT;
    }
    else if (object->damageY < -DAMAGE_DECREASE_AMOUNT)
    {
        object->damageY += DAMAGE_DECREASE_AMOUNT;
    }
    else
    {
        object->damageY = 0;
    }

    if (object->damageX > DAMAGE_DECREASE_AMOUNT)
    {
        object->damageX -= DAMAGE_DECREASE_AMOUNT;
    }
    else if (object->damageX < -DAMAGE_DECREASE_AMOUNT)
    {
        object->damageX += DAMAGE_DECREASE_AMOUNT;
    }
    else
    {
        object->damageX = 0;
    }

    return object->damageX != 0 || object->damageY != 0;
}

uint8_t Damaged(Object *object)
{

    MoveToNextPosition(object, (object->x + object->damageX), (object->y + object->damageY), (object->x + object->damageX) >> 4, (object->y + object->damageY) >> 4);

    return DamagedNoMove(object);
}

typedef void (*BankedObjectUpdate)() BANKED;

uint8_t currentUpdateSpriteCount = 0;
Object *currentObjectUpdating = 0;

void UpdateAllObjects() NONBANKED
{

    // This will be used for anything flashing/blinking from damage
    universalBlinker++;
    universalBlinkerFast += 3;

    if ((universalBlinker >> 4) > 1)
        universalBlinker = 0;
    if ((universalBlinkerFast >> 4) > 1)
        universalBlinkerFast = 0;

    universalBlinkerTrue = universalBlinker >> 4;
    universalBlinkerFastTrue = universalBlinkerFast >> 4;

    Object *previousObject = 0;
    currentObjectUpdating = firstObject;
    currentUpdateSpriteCount = 0;

    uint8_t _previous_bank = _current_bank;

    while (currentObjectUpdating != 0)
    {

        FAR_CALL(currentObjectUpdating->updateFarPtr, BankedObjectUpdate);

        Object *nextObject = currentObjectUpdating->next;

        // If we should be recycled
        if (currentObjectUpdating->recycle)
        {

            // Reycle us, but don't change the previous
            RecycleObject(currentObjectUpdating, previousObject);
        }
        else
        {

            // Update the previous
            previousObject = currentObjectUpdating;
        }

        currentObjectUpdating = nextObject;
    }

    FinishLinkUpdate();

    // Hide the rest
    hide_sprites_range(currentUpdateSpriteCount, MAX_HARDWARE_SPRITES);
}