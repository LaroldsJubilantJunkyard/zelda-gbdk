
#ifndef OBJECTS_HEADER
#define OBJECTS_HEADER set
#define MAX_NUMBER_OF_VRAM_TYPES 8
#define MAX_NUMBER_OF_OBJECTS 10

#include <gb/metasprites.h>

typedef struct Object{

    uint16_t x,y;
    int8_t explosion;
    uint8_t id;
    uint8_t blinkTimer;
    int16_t damageX, damageY;
    uint8_t active;
    uint8_t direction;
    uint8_t recycle;
    uint16_t originalArea;
    uint8_t health;
    uint16_t helper1;
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
extern uint16_t universalBlinker;
extern ObjectType objectTypes[];

extern ObjectType* firstObjectType,*lastObjectType;
extern Object* firstObject,*lastObject;

Object* SpawnObject(ObjectType* type, uint16_t x, uint16_t y, uint8_t direction,uint8_t id);

void RecycleObject(Object* objectToBeRecycled, Object* previousObject);
void UpdateAllObjects();
uint8_t MoveToNextPosition(Object* object, uint16_t nextX, uint16_t nextY);
uint8_t Damaged(Object* object,uint8_t sprite);
uint8_t RemoveWhenOutOffscreen(Object* object);


#define MOBLIN_OBJECT_TYPE &objectTypes[2]
#define EXPLOSION_OBJECT_TYPE &objectTypes[1]
#define LINK_OBJECT_TYPE &objectTypes[0]

#endif