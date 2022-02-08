#include <gb/gb.h>
#include "Common.h"
#include "Link.h"
#include "graphics/HUD.h"
#include "graphics/Map.h"
#include "graphics/AlphaNumeric.h"

extern Link link;

#define TEXT_TILES_START HUD_TILE_COUNT+Map_TILE_COUNT

void WriteText(unsigned char *text, uint8_t bottom){

    uint8_t startRow = bottom?10 : 1;

    // Save the previous background tiles for both planes
    unsigned char previous[90];
    unsigned char previous1[90];

    VBK_REG=1;
    get_bkg_tiles(1,startRow,18,5,previous1);
    VBK_REG=0;
    get_bkg_tiles(1,startRow,18,5,previous);

    // A array for our character => VRAM mapping
    // 0-25 aphabet uppercase
    // 26-51 alphabet lowercase
    // 52-61 numbers
    // 62-63 some punctuation
    uint8_t character2VRAMMapping[64];

    // Default our character2VRAMMapping
    for(uint8_t j=0;j<64;j++)character2VRAMMapping[j]=0;

    int numberOfChars = 0;

    uint8_t i=0;

    while(text[i]!='\0'){

        uint16_t offset = 0;

        // Is it a captial letter
        if(text[i]>='A'&&text[i]<='Z'){

            // If we do not have a character in the character2VRAMMapping for this
            if(character2VRAMMapping[text[i]-'A']==0){
                character2VRAMMapping[text[i]-'A']=TEXT_TILES_START+numberOfChars;
                // 16 values per tile
                offset=(text[i]-'A')*16;

                // Set in our last VRAM spot the tile rom the AlphaNumeric tileset with the provided ofset
                set_bkg_data(TEXT_TILES_START+numberOfChars,1,&AlphaNumeric_tiles[offset]);
                numberOfChars++;
            }

        // Is it a lowercase letter
        }else if(text[i]>='a'&&text[i]<='z'){

            // If we do not have a character in the character2VRAMMapping for this
            if(character2VRAMMapping[text[i]-'a'+26]==0){
                character2VRAMMapping[text[i]-'a'+26]=TEXT_TILES_START+numberOfChars;
                // Skip the first 26
                // 26 * 16 = 416
                // 16 values per tile
                offset=416+(text[i]-'a')*16;

                // Set in our last VRAM spot the tile rom the AlphaNumeric tileset with the provided ofset
                set_bkg_data(TEXT_TILES_START+numberOfChars,1,&AlphaNumeric_tiles[offset]);
                numberOfChars++;
            }

        // Is it a letter
        }else if(text[i]>='0'&&text[i]<='0'){

            // If we do not have a character in the character2VRAMMapping for this
            if(character2VRAMMapping[text[i]-'0'+52]==0){
                character2VRAMMapping[text[i]-'0'+52]=TEXT_TILES_START+numberOfChars;
                // Skip the first 52
                // 52 * 16 = 832
                // 16 values per tile
                offset=832+(text[i]-'0')*16;

                // Set in our last VRAM spot the tile rom the AlphaNumeric tileset with the provided ofset
                set_bkg_data(TEXT_TILES_START+numberOfChars,1,&AlphaNumeric_tiles[offset]);
                numberOfChars++;
            }
        }
        


        i++;
    }

    // Fill our background with black tiles.
    // tile 27 is my blank/black tile
    VBK_REG=1;
    fill_bkg_rect(1,startRow,18,5,0);
    VBK_REG=0;
    fill_bkg_rect(1,startRow,18,5,27);

    uint8_t column=0;
    uint8_t row=0;
    i=0;

    // Until we reach the string termination character
    while(text[i]!='\0'){

        uint8_t tile = 0;

        if(text[i]>='A'&&text[i]<='Z'){
            tile=character2VRAMMapping[text[i]-'A'];
        }else if(text[i]>='a'&&text[i]<='z'){
            tile=character2VRAMMapping[text[i]-'a'+26];
        }else if(text[i]>='0'&&text[i]<='9'){
            tile=character2VRAMMapping[text[i]-'0'+52];
        }

        // if a tile was found for the mapping
        if(tile!=0){

            // Put the tile on the background
            VBK_REG=1;
            set_bkg_tile_xy(2+column,startRow+1+row,0);
            VBK_REG=0;
            set_bkg_tile_xy(2+column,startRow+1+row,tile);

            // Wait a lil'bit for the typerwriter effect
            wait_vbl_done();
        }

        // Move onto the next character
        // Move onto the next column
        // Reset to column 0 if we go over 16 characters
        i++;
        column++;
        if(column>=16){
            column=0;
            row+=2;
        }
    }

    // TODO Maybe utilize waitpad and waitpad?
    while(1){

        joypadPrevious=joypadCurrent;
        joypadCurrent=joypad();

        // If the A button was JUST pressed
        // This avoids holding A to skip through too fast and missing text
        if((joypadCurrent & J_A) && !(joypadPrevious & J_A))break;
    }


    // Restore our previous background state
    VBK_REG=1;
    set_bkg_tiles(1,startRow,18,5,previous1);
    VBK_REG=0;
    set_bkg_tiles(1,startRow,18,5,previous);

}

void SetupHeadsUpDisplay(){
    
    set_bkg_data(0,HUD_TILE_COUNT,HUD_tiles);

    UpdateHeadsUpDisplay();
}

void UpdateHeadsUpDisplay(){

    set_win_tile_xy(0,0,22);
    set_win_tile_xy(0,1,24);
    
    set_win_tile_xy(4,0,25);
    set_win_tile_xy(4,1,26);

    set_win_tile_xy(5,0,23);
    set_win_tile_xy(5,1,24);
    
    set_win_tile_xy(9,0,25);
    set_win_tile_xy(9,1,26);


    //Shield= 13, Sword=15

    // Left
    VBK_REG=1;
    set_win_tile_xy(1,0,2);
    set_win_tile_xy(1,1,2);
    VBK_REG=0;
    set_win_tile_xy(1,0,13);
    set_win_tile_xy(1,1,14);

    // Right
    VBK_REG=1;
    set_win_tile_xy(6,0,2);
    set_win_tile_xy(6,1,2);
    VBK_REG=0;
    set_win_tile_xy(6,0,15);
    set_win_tile_xy(6,1,16);



    // rupee    
    VBK_REG=1;
    set_win_tile_xy(10,0,3);
    VBK_REG=0;
    set_win_tile_xy(10,0,12);


    for(uint8_t i=0;i<link.hearts;i++){

        uint8_t frame=17;
        uint8_t hp = 4*(i+1);

        if(link.health>=4*(i+1))frame=21;
        else if(link.health<4*i)frame=17;
        else frame =17+(link.health-4*i);

        // hearts
        VBK_REG=1;
        set_win_tile_xy(13+i,0,3);
        VBK_REG=0;
        set_win_tile_xy(13+i,0,frame);
    }

}