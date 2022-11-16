#ifndef COMMON_HEADER
#define COMMON_HEADER set

#include "objects.h"
#include <gbdk/far_ptr.h>

#define MIN(A,B) ((A)<(B)?(A):(B))
#define MAX(A,B) ((A)>(B)?(A):(B))

#define TILEMAP_TILES_START HUD_TILE_COUNT

extern uint8_t joypadCurrent,joypadPrevious,_previous_bank;

extern const int16_t J_DIRECTIONS[9][2];

#define PUSH_NAMED_BANK(b) _previous_bank=_current_bank; SWITCH_ROM(BANK(b));
#define POP_BANK SWITCH_ROM(_previous_bank);

void move_metasprite_with_camera(const metasprite_t * metasprite, uint8_t base_tile, uint16_t x, uint16_t y) NONBANKED;
void move_object_with_camera(Object* object) NONBANKED;
void move_metasprite_with_palette(Object* object,uint8_t palette) NONBANKED;

#endif