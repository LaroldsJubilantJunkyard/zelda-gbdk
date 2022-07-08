#include <gb/gb.h>
#include "graphics/TileMap.h"
#include "link.h"
#include "objects.h"
#include "graphics/HUD.h"



uint8_t CheckObjectIntersection3(Object* objectA, int16_t x, int16_t y, int16_t w, int16_t h){

    int16_t xd = (objectA->x>>4)-(x>>4);

    int16_t hw2=((int16_t)objectA->type->width>>1)+(w>>1);

    if(xd>hw2)return 0;
    if(xd<-hw2)return 0;

    int16_t hh2=((int16_t)objectA->type->height>>1)+(h>>1);

    int16_t yd = (objectA->y>>4)-(y>>4);

    if(yd>hh2)return 0;
    if(yd<-hh2)return 0;

    return 1;
}


uint8_t CheckCollisionAgainstLinksSword(Object* object){

    // Make sure link is actually attacking 
    if(linkSword==-1)return 0;

    // Check against the swords rectangle
    return CheckObjectIntersection3(object,linkSwordX,linkSwordY,16,16);
}


uint8_t CheckObjectIntersection(Object* objectA, Object* objectB){

    return CheckObjectIntersection3(objectA,objectB->x,objectB->y,objectB->type->width,objectB->type->height);
}


uint8_t IntersectsSolidWorld(uint16_t left, uint16_t top, uint16_t width,uint16_t height){

    uint16_t column=left>>3;
    uint16_t row = top>>3;

    uint16_t topLeft = column+row*(TileMap_WIDTH/8);

    if(TileMap_map[topLeft]<1)return 1;

    uint16_t column2=(left+width)>>3;

    uint16_t topRight = column2+row*(TileMap_WIDTH>>3);

    if(TileMap_map[topRight]<1)return 1;
    
    uint16_t row2 = (top+height)>>3;

    uint16_t bottomRight = column2+row2*(TileMap_WIDTH>>3);

    if(TileMap_map[bottomRight]<1)return 1;

    uint16_t bottomLeft = column+row2*(TileMap_WIDTH>>3);

    if(TileMap_map[bottomLeft]<1)return 1;

    return 0;
}