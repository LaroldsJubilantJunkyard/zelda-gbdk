#ifndef COMMON_HEADER
#define COMMON_HEADER set

#include "objects.h"

#define MIN(A,B) ((A)<(B)?(A):(B))
#define MAX(A,B) ((A)>(B)?(A):(B))

#define TILEMAP_TILES_START HUD_TILE_COUNT

extern uint8_t joypadCurrent,joypadPrevious;

extern const int8_t J_DIRECTIONS[9][2];

uint8_t move_metasprite_with_camera2(const metasprite_t * metasprite, uint8_t base_tile, uint8_t base_sprite, uint16_t x, uint16_t y);
uint8_t move_metasprite_with_camera(Object* object,uint8_t sprite);

uint8_t move_metasprite_with_palette(Object* object,uint8_t sprite,uint8_t palette);

#endif