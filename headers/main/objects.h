
#ifndef OBJECTS_HEADER
#define OBJECTS_HEADER set
#define MAX_NUMBER_OF_VRAM_TYPES 8
#define MAX_NUMBER_OF_OBJECTS 10

#include <gb/metasprites.h>
#include <gbdk/far_ptr.h>

typedef struct Object{

    int16_t x,y,trueX,trueY;
    int8_t explosion;
    uint8_t id;
    int16_t damageX, damageY;
    uint8_t active;
    uint8_t direction;
    uint8_t baseTileIndex;
    uint8_t recycle;
    uint16_t originalArea;
    uint8_t health;
    uint16_t helper1;
    FAR_PTR updateFarPtr;
    struct ObjectType *type;
    struct Object *next;
    const metasprite_t* currentMetasprite;
} Object;

typedef struct ObjectType{

    uint8_t active;
    uint8_t bank;
    uint8_t width;
    uint8_t height;
    uint8_t count;
    uint8_t startTile;
    uint8_t maxHealth;
    uint8_t *tileData;
    uint8_t tileCount;
    uint8_t paddedTileCount;
    uint8_t (*update)(Object*,uint8_t);
    struct ObjectType *next;

} ObjectType;


extern Object* link;
extern uint16_t universalBlinker,universalBlinkerTrue,universalBlinkerFast,universalBlinkerFastTrue;
extern ObjectType objectTypes[];

extern Object* firstObject,*lastObject;
extern metasprite_t  ramMetaspriteObjects[];


extern uint8_t currentUpdateSpriteCount;
extern Object* currentObjectUpdating;

Object* SpawnObject(ObjectType* type, int16_t x, int16_t y, uint8_t direction,uint8_t id,uint8_t baseTileIndex, FAR_PTR updateFarPtr);

void ResetAllObjects();
void RecycleObject(Object* objectToBeRecycled, Object* previousObject);
void UpdateAllObjects() NONBANKED;
uint8_t MoveToNextPosition(Object* object, int16_t nextXTemp, int16_t nextYTemp,int16_t trueNextXTemp, int16_t trueNextYTemp) NONBANKED;
uint8_t Damaged(Object* object);
uint8_t RemoveWhenOutOffscreen(Object* object);

void RecycleOutOfScreenObjects();
uint8_t CheckLinkInteractionWithObject(Object* currentObject);

uint8_t DamagedNoMove(Object* object);
void StopLinkFromOverlapping(Object* object);

void SetObjectX(Object* object, int16_t amount);
void SetObjectY(Object* object, int16_t amount);

#define MOBLIN_OBJECT_TYPE &objectTypes[2]
#define MARIN_OBJECT_TYPE &objectTypes[3]
#define EXPLOSION_OBJECT_TYPE &objectTypes[1]
#define LINK_OBJECT_TYPE &objectTypes[0]

#endif