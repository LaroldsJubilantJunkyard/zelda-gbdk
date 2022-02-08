#include <gb/gb.h>
#include <gb/metasprites.h>
uint8_t joypadCurrent,joypadPrevious,blinkDamaged;


void move_metasprite_props(const metasprite_t* current, uint8_t base_tile, uint8_t base_sprite, uint8_t x, uint8_t y,uint8_t props) {
    metasprite_t temp[4];

    if(props==0){
        move_metasprite(current,base_tile,base_sprite,x,y);
        return;

    }

    for(uint8_t i=0;i<4;i++){
    
        // Retain sprite position, and tile
        temp[i].dy=current->dy;
        temp[i].dx=current->dx;
        temp[i].dtile=current->dtile;
        temp[i].props=current->props;

        // If we should blink for damage
        // Change the properties, but retain the flip
        if(blinkDamaged){
            temp[i].props= props | (current->props & 128 ) | (current->props & 64);
        }
        current++;
    }

    const metasprite_t *tempPtr=temp;

    // hide or show the metasprites
    move_metasprite(tempPtr,base_tile,base_sprite,x,y);
}