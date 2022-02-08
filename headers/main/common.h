#include <gb/metasprites.h>
extern uint8_t joypadCurrent,joypadPrevious, blinkDamaged,backgroundScroll;
extern uint16_t backgroundLeftX,backgroundTopY;

#define BACKGROUND_IS_SCROLLING backgroundScroll!=0

void move_metasprite_props(const metasprite_t* current, uint8_t base_tile, uint8_t base_sprite, uint8_t x, uint8_t y,uint8_t props) ;