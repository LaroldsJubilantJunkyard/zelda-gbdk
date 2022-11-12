#ifndef COLLISION_HEADER
#define COLLISION_HEADER set
#include "objects.h"
uint8_t IntersectsSolidWorld(int16_t left, int16_t top, int16_t width,int16_t height);

uint8_t CheckObjectIntersectionAgainstLinksSword(Object* object);
uint8_t CheckObjectIntersection(Object* objectA, Object* objectB);
uint8_t CheckObjectIntersectionAgainstRectangle(Object* objectA, uint16_t x, uint16_t y, int16_t w, int16_t h);

#endif