#include <gb/gb.h>
#include "graphics/BasicMap.h"
#include "link.h"
#include "objects.h"
#include "world.h"
#include "camera.h"
#include "common.h"
#include "graphics/HUD.h"



uint8_t CheckObjectIntersectionAgainstRectangle(Object* objectA,int16_t width,int16_t height, int16_t x, int16_t y, int16_t w, int16_t h){

    int16_t xd = (objectA->trueX)-x;

    int16_t hw2=(width>>1)+(w>>1);

    if(xd>hw2)return 0;
    if(xd<-hw2)return 0;

    int16_t hh2=(height>>1)+(h>>1);

    int16_t yd = (objectA->trueY)-y;

    if(yd>hh2)return 0;
    if(yd<-hh2)return 0;

    return 1;
}


uint8_t CheckObjectIntersectionAgainstLinksSword(Object* object,int16_t width,int16_t height){

    // Make sure link is actually attacking 
    if(linkSword==-1)return 0;

    // Check against the swords rectangle
    return CheckObjectIntersectionAgainstRectangle(object,width,height,linkSwordTrueX,linkSwordTrueY,16,16);
}


uint8_t CheckObjectIntersection(Object* objectA,int16_t widthA,int16_t heightA, Object* objectB,int16_t widthB,int16_t heightB){

    return CheckObjectIntersectionAgainstRectangle(objectA,widthA,heightA,objectB->trueX,objectB->trueY,widthB,heightB);
}


uint8_t IntersectsSolidWorld(int16_t left, int16_t top, int16_t width,int16_t height){

    if(GetWorldPositionIsSolid(left,top))return 1;
    if(GetWorldPositionIsSolid(left+width,top))return 1;
    if(GetWorldPositionIsSolid(left,top+height))return 1;
    if(GetWorldPositionIsSolid(left+width,top+height))return 1;

    return 0;
}