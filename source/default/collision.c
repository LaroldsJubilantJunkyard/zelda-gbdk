#include <gb/gb.h>
#include "graphics/TileMap.h"
#include "link.h"
#include "objects.h"

uint8_t CheckObjectIntersection2(Object* objectA, uint16_t x, uint16_t y){

    int16_t xd = (objectA->x>>4)-(x>>4);

    if(xd>10)return 0;
    if(xd<-10)return 0;

    int16_t yd = (objectA->y>>4)-(y>>4);

    if(yd>10)return 0;
    if(yd<-10)return 0;

    return 1;
}


uint8_t CheckCollisionAgainstLinksSword(Object* object){
    if(linkSword==-1)return 0;
    return CheckObjectIntersection2(object,linkSwordX,linkSwordY);
}


uint8_t CheckCollisionAgainstLinksSword2(uint16_t x, uint16_t y, uint8_t width, uint8_t height){
    if(linkSword==-1)return 0;
    return 0;// TODO return CheckObjectIntersection2(object,linkSwordX,linkSwordY);
}


uint8_t CheckObjectIntersection(Object* objectA, Object* objectB){

    int16_t xd = (objectA->x>>4)-(objectB->x>>4);

    uint8_t hw2=(objectA->type->width>>2)+(objectB->type->width>>2);

    if(xd>hw2)return 0;
    if(xd<-hw2)return 0;

    uint8_t hh2=(objectA->type->height>>2)+(objectB->type->height>>2);

    int16_t yd = (objectA->y>>4)-(objectB->y>>4);

    if(yd>hh2)return 0;
    if(yd<-hh2)return 0;

    return 1;
}


uint8_t IntersectsSolidWorld(uint16_t left, uint16_t top, uint16_t width,uint16_t height){

    uint16_t column=left/8;
    uint16_t row = top/8;

    uint16_t topLeft = column+row*(TileMap_WIDTH/8);

    if(TileMap_map[topLeft]<1)return 1;

    uint16_t column2=(left+width)/8;

    uint16_t topRight = column2+row*(TileMap_WIDTH/8);

    if(TileMap_map[topRight]<1)return 1;
    
    uint16_t row2 = (top+height)/8;

    uint16_t bottomRight = column2+row2*(TileMap_WIDTH/8);

    if(TileMap_map[bottomRight]<1)return 1;

    uint16_t bottomLeft = column+row2*(TileMap_WIDTH/8);

    if(TileMap_map[bottomLeft]<1)return 1;

    return 0;
}