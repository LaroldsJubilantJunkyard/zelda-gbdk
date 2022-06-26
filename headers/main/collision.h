#ifndef COLLISION_HEADER
#define COLLISION_HEADER set
#include "objects.h"
uint8_t IntersectsSolidWorld(uint16_t top, uint16_t left, uint16_t width,uint16_t height);

uint8_t CheckCollisionAgainstLinksSword2(uint16_t x, uint16_t y, uint8_t width, uint8_t height);
uint8_t CheckCollisionAgainstLinksSword(Object* object);
uint8_t CheckObjectIntersection(Object* objectA, Object* objectB);
uint8_t CheckObjectIntersection2(Object* objectA, uint16_t x, uint16_t y);

#endif