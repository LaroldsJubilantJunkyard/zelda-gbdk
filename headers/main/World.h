typedef struct Area{
    uint8_t widthInTiles;
    uint8_t heightInTiles;
    uint8_t bank;
    uint8_t tileCount;
    const uint8_t *Map_tiles;
    const unsigned char *MapPlane0;
    const unsigned char *MapPlane1;
} Area;

typedef struct WorldObject{

    uint8_t type;
    uint8_t index;
    uint16_t x,y;
}WorldObject;

typedef struct Section{

    uint8_t count,offset;

} Section;

void SetupWorld();
void UpdateWorld();