#include <gb/gb.h>
#include "link.h"
#include "objects.h"
#include "common.h"


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
        
        if(i<linkMaxHealth){
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

uint8_t tiles[72];

void DrawText(unsigned char *text, uint8_t top){

    uint8_t len =0;
    
    uint8_t column=0;
    uint8_t row=1;

    uint8_t baseY = top? 1: 10;
    uint8_t topY=baseY*8;
    uint8_t bottomY=topY+48;

    joypadCurrent=0;
    joypadPrevious=0;

    // Hide all sprites that would appear over the dialog box
    for(uint8_t i=0;i<MAX_HARDWARE_SPRITES;i++){

        // Offset since sprites are drawn -16 from their shadow OAM y coordinate
        if(shadow_OAM[i].y>=topY+8&&shadow_OAM[i].y<=bottomY+8){
            hide_sprite(i);
        }
    }

    // Copy background tiles to the window
    // So we don't have to 
    for(uint8_t i=0;i<18;i++){
        for(uint8_t j=0;j<6;j++){
            set_win_tile_xy(i+1,j+2,get_bkg_tile_xy(i+1,j+baseY));
        }
    }

    // Fill with black
    fill_bkg_rect(1,baseY,18,6,0x1B);

    while(text[len]!='\0'){

        // If we've scrolled enough
        if(row>3){


            joypadPrevious = joypadCurrent;
            joypadCurrent = joypad();

            // If A was just pressed
            if((joypadCurrent&J_A)&& !(joypadPrevious&J_A)){


                // Shit tiles upwards
                get_bkg_tiles(1,baseY+1,18,4,tiles);
                set_bkg_tiles(1,baseY,18,4,tiles);

                // Wait for a tiny bit
                wait_vbl_done();
                wait_vbl_done();
                wait_vbl_done();
                
                // Shift tiles upwards
                get_bkg_tiles(1,baseY+1,18,4,tiles);
                set_bkg_tiles(1,baseY,18,4,tiles);

                // Wait for a tiny bit
                wait_vbl_done();

                row=3;
            }


        }else{
            
            char c =text[len];

            uint8_t tile = 0;
            uint8_t isCurrentLetterNonAlphaNumeric=FALSE;

            if(c==' '){
                tile=0x1B;
                 isCurrentLetterNonAlphaNumeric=TRUE;
            }
            else if(c=='.'){
                tile=0x5A;
                 isCurrentLetterNonAlphaNumeric=TRUE;
            }
            else if(c=='\"'){
                tile=0x5B;
                 isCurrentLetterNonAlphaNumeric=TRUE;
            }
            else if(c==','){
                tile=0x5C;
                 isCurrentLetterNonAlphaNumeric=TRUE;
            }
            else if(c=='\''){
                tile=0x5D;
                 isCurrentLetterNonAlphaNumeric=TRUE;
            }
            else if(c>='0'&&c<='9')tile=0x50+(c-'0');
            else if(c>='a'&&c<='z')tile=0x36+(c-'a');
            else if(c>='A'&&c<='Z')tile=0x1C+(c-'A');
            else{
                tile=0x1B;
                isCurrentLetterNonAlphaNumeric=TRUE;
            }

            set_bkg_tile_xy(1+column,baseY+row,tile);

            if((++column)>=18){
                column=0;
                row+=2;
            }

            else if(isCurrentLetterNonAlphaNumeric){

                uint8_t distance=1;

                // How much space until the end of the column
                uint8_t left = 18-column;

                // We want to find the next non-alphanumeric character
                while(text[len+distance]!='\0'){

                    uint8_t c2 = text[len+distance];

                    uint8_t alphaNumeric=FALSE;

                    if(c2>='0'&&c2<='9')alphaNumeric=TRUE;
                    else if(c2>='a'&&c2<='z')alphaNumeric=TRUE;
                    else if(c2>='A'&&c2<='Z')alphaNumeric=TRUE;

                    // Stop the loop if this character isn't alphanumeric
                    if(!alphaNumeric)break;

                    distance++;
                }

                // If the next special character is on a new row
                if(distance>=left){

                    // Skip ahead
                    column=0;
                    row+=2;
                    
                }
            }

            len++;
        }
        

        wait_vbl_done();
    }


    // Wait until the a button is pressed
    while(1){

        joypadPrevious = joypadCurrent;
        joypadCurrent = joypad();

        // Break out of the loop when A is pressed
        if((joypadCurrent&J_A)&& !(joypadPrevious&J_A)){
            break;;
        }
    }

    
    // Restore tiles from the window to the background
    for(uint8_t i=0;i<18;i++){
        for(uint8_t j=0;j<6;j++){
            set_bkg_tile_xy(i+1,j+baseY,get_win_tile_xy(i+1,j+2));
        }
    }
}