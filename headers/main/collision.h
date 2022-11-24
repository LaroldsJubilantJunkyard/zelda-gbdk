#ifndef COLLISION_HEADER
#define COLLISION_HEADER set
#include "objects.h"
uint8_t IntersectsSolidWorld(int16_t left, int16_t top, int16_t width,int16_t height);

uint8_t CheckObjectIntersectionAgainstLinksSword(Object* object, int16_t width,int16_t height);
uint8_t CheckObjectIntersection(Object* objectA, int16_t widthA,int16_t heightA, Object* objectB, int16_t widthB,int16_t heightB);
uint8_t CheckObjectIntersectionAgainstRectangle(Object* objectA, int16_t width,int16_t height, uint16_t x, uint16_t y, int16_t w, int16_t h);

#endif