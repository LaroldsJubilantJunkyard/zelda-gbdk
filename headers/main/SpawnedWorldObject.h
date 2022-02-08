#include <gb/gb.h>
#include <gb/metasprites.h>
typedef struct SpawnedWorldObject SpawnedWorldObject;

struct SpawnedWorldObject{
    uint16_t x, y;
    uint8_t active,old,health,startSprite, startTile,direction,index,frame;
    int8_t hitX,hitY;
    metasprite_t const *firstMetasprite;
    void (*update)(SpawnedWorldObject*);
} ;
 
#define MAX_SPAWNED_OBJECTS_ONSCREEN 10
extern SpawnedWorldObject sortedSpawnedObjects[MAX_SPAWNED_OBJECTS_ONSCREEN];


void UpdateAllSpawnedWorldObjects();