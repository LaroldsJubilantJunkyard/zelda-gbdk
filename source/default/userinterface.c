#include <gb/gb.h>
#include "link.h"
#include "objects.h"


void UpdateBItem(){

    // Brackets
    set_win_tile_xy(0,0,22);
    set_win_tile_xy(0,1,24);
    set_win_tile_xy(4,0,25);
    set_win_tile_xy(4,1,26);

    
    
    // Level
    //set_win_tile_xy(2,1,11);
    //set_win_tile_xy(3,1,2);
}

void UpdateAItem(){

    // Brackets
    set_win_tile_xy(5,0,23);
    set_win_tile_xy(5,1,24);
    set_win_tile_xy(9,0,25);
    set_win_tile_xy(9,1,26);
    
    // Sword
    VBK_REG=1;
    set_win_tile_xy(6,0,2);
    set_win_tile_xy(6,1,2);
    VBK_REG=0;
    set_win_tile_xy(6,0,15);
    set_win_tile_xy(6,1,16);
    
    // Level
    set_win_tile_xy(7,1,11);
    set_win_tile_xy(8,1,2);
}

void UpdateZenny(){

    VBK_REG=1;
    set_win_tile_xy(10,0,3);
    VBK_REG=0;
    set_win_tile_xy(10,0,12);
    set_win_tile_xy(10,1,8);
    set_win_tile_xy(11,1,8);
    set_win_tile_xy(12,1,8);
}
void UpdateHearts(){

    for(uint8_t i=0;i<14;i++){

        uint8_t row = i/7;
        uint8_t column = i%7;
        
        if(i<=linkMaxHealth){
            VBK_REG=1;
            set_win_tile_xy(13+column,row,3);
            VBK_REG=0;
            set_win_tile_xy(13+column,row,i>=link->health?17:21);
        }
    }

}
void UpdateInterfaceFull(){

    UpdateAItem();
    UpdateBItem();
    UpdateZenny();
    UpdateHearts();

}