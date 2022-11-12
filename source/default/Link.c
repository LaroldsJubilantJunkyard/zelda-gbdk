#pragma bank 255

#include <gb/gb.h>
#include <gb/metasprites.h>
#include "objects.h"
#include "common.h"
#include "world.h"
#include "camera.h"
#include "graphics/HUD.h"
#include "collision.h"
#include "graphics/LinkSpritesDown.h"
#include "graphics/LinkSpritesUp.h"
#include "graphics/LinkSpritesLeft.h"
#include "graphics/LinkSpritesRight.h"
//
#include "graphics/SwordSlashDown.h"
#include "graphics/SwordSlashUp.h"
#include "graphics/SwordSlashLeft.h"
#include "graphics/SwordSlashRight.h"
//
#include "graphics/LinkSwordSpritesDown.h"
#include "graphics/LinkSwordSpritesLeft.h"
#include "graphics/LinkSwordSpritesUp.h"
#include "graphics/LinkSwordSpritesRight.h"

BANKREF(LinkBank)

uint8_t linkLastBank;
int16_t linkSwordX, linkSwordY;
int16_t nextX, nextY, trueNextX, trueNextY;

int16_t linkSwordTrueX = 0;
int16_t linkSwordTrueY = 0;
int8_t linkSword = -1;
uint8_t linkMaxHealth = 5;

uint8_t frame = 0;

extern uint8_t gotSword;

const int8_t SwordOffsets[9][3][2] = {
    {{0, 0}, {0, 0}, {0, 0}},
    {{0, -16}, {8, -8}, {16, 0}},   // right
    {{0, -16}, {-8, -8}, {-16, 0}}, // left
    {{0, 0}, {0, 0}, {0, 0}},
    {{16, 0}, {8, -8}, {0, -16}}, // up
    {{0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}},
    {{0, 0}, {0, 0}, {0, 0}},
    {{-16, 0}, {-8, 8}, {0, 16}} // down
};

void UpdateLinkDefaultSprites(Object *object) NONBANKED
{

    switch (object->direction)
    {

    case J_DOWN:

        PUSH_NAMED_BANK(LinkSpritesDown);
        
        // Update our vram for the proper direction
        set_sprite_data(0, LinkSpritesDown_TILE_COUNT, LinkSpritesDown_tiles);

        POP_BANK;

        break;

    case J_LEFT:

        PUSH_NAMED_BANK(LinkSpritesLeft);
        

        // Update our vram for the proper direction
        set_sprite_data(0, LinkSpritesLeft_TILE_COUNT, LinkSpritesLeft_tiles);

        POP_BANK;

        break;

    case J_RIGHT:

        PUSH_NAMED_BANK(LinkSpritesRight);
        

        // Update our vram for the proper direction
        set_sprite_data(0, LinkSpritesRight_TILE_COUNT, LinkSpritesRight_tiles);

        POP_BANK;

        break;

    case J_UP:

        PUSH_NAMED_BANK(LinkSpritesUp);

        // Update our vram for the proper direction
        set_sprite_data(0, LinkSpritesUp_TILE_COUNT, LinkSpritesUp_tiles);

        POP_BANK;

        break;
    }
}

void SetLinkSwordTileData() NONBANKED
{
    switch (link->direction)
    {
    case J_DOWN:

        PUSH_NAMED_BANK(LinkSwordSpritesDown);

        // Update our vram for the proper direction
        set_sprite_data(0, LinkSwordSpritesDown_TILE_COUNT, LinkSwordSpritesDown_tiles);

        POP_BANK;

        break;
    case J_UP:

        PUSH_NAMED_BANK(LinkSwordSpritesUp);

        // Update our vram for the proper direction
        set_sprite_data(0, LinkSwordSpritesUp_TILE_COUNT, LinkSwordSpritesUp_tiles);

        POP_BANK;

        break;
    case J_LEFT:

        PUSH_NAMED_BANK(LinkSwordSpritesLeft);

        // Update our vram for the proper direction
        set_sprite_data(0, LinkSwordSpritesLeft_TILE_COUNT, LinkSwordSpritesLeft_tiles);

        POP_BANK;

        break;
    case J_RIGHT:

        PUSH_NAMED_BANK(LinkSwordSpritesRight);

        // Update our vram for the proper direction
        set_sprite_data(0, LinkSwordSpritesRight_TILE_COUNT, LinkSwordSpritesRight_tiles);

        POP_BANK;

        break;
    }

    switch (link->direction)
    {
    case J_DOWN:

        PUSH_NAMED_BANK(SwordSlashDown);

        // Update our vram for the proper direction
        set_sprite_data(8, SwordSlashDown_TILE_COUNT, SwordSlashDown_tiles);

        POP_BANK;

        break;
    case J_UP:

        PUSH_NAMED_BANK(SwordSlashUp);

        // Update our vram for the proper direction
        set_sprite_data(8, SwordSlashUp_TILE_COUNT, SwordSlashUp_tiles);

        POP_BANK;

        break;
    case J_LEFT:

        PUSH_NAMED_BANK(SwordSlashLeft);

        // Update our vram for the proper direction
        set_sprite_data(8, SwordSlashLeft_TILE_COUNT, SwordSlashLeft_tiles);

        POP_BANK;

        break;
    case J_RIGHT:

        PUSH_NAMED_BANK(SwordSlashRight);

        // Update our vram for the proper direction
        set_sprite_data(8, SwordSlashRight_TILE_COUNT, SwordSlashRight_tiles);

        POP_BANK;

        break;
    }
}

void HandleLinkInput(Object *object, uint8_t *frame)
{
    if (linkSword == -1 && cameraScrollDirection == 0)
    {
        if (joypadCurrent & J_RIGHT)
        {
            nextX += 8;
            *frame = universalBlinkerTrue;
            object->direction = J_RIGHT;
        }
        if (joypadCurrent & J_LEFT)
        {
            nextX -= 8;
            *frame = universalBlinkerTrue;
            object->direction = J_LEFT;
        }
        if (joypadCurrent & J_DOWN)
        {
            nextY += 8;
            *frame = universalBlinkerTrue;
            object->direction = J_DOWN;
        }
        if (joypadCurrent & J_UP)
        {
            nextY -= 8;
            *frame = universalBlinkerTrue;
            object->direction = J_UP;
        }

        trueNextX = nextX >> 4;
        trueNextY = nextY >> 4;
    }

    if ((joypadCurrent & J_A) && !(joypadPrevious & J_A) && linkSword == -1 && gotSword)
    {

        linkSword = 0;

        SetLinkSwordTileData();
    }
}

uint8_t move_object_with_camera_sword_slash(Object *object) NONBANKED
{

    uint8_t count = 0;

    uint8_t frame = linkSword >> 4;

    // The sword slash animations only have 3 frames
    // Limit the frame variable here so it stays in the final frame for a little bit
    if (frame > 2)
        frame = 2;

    switch (object->direction)
    {

    default:

        PUSH_NAMED_BANK(SwordSlashDown);

        move_metasprite_with_camera(SwordSlashDown_metasprites[frame], 8, linkSwordTrueX, linkSwordTrueY);

        POP_BANK;

        break;

    case J_LEFT:

        PUSH_NAMED_BANK(SwordSlashLeft);

        move_metasprite_with_camera(SwordSlashLeft_metasprites[frame], 8, linkSwordTrueX, linkSwordTrueY);

        POP_BANK;

        break;

    case J_RIGHT:

        PUSH_NAMED_BANK(SwordSlashRight);

        move_metasprite_with_camera(SwordSlashRight_metasprites[frame], 8, linkSwordTrueX, linkSwordTrueY);

        POP_BANK;

        break;

    case J_UP:

        PUSH_NAMED_BANK(SwordSlashUp);

        move_metasprite_with_camera(SwordSlashUp_metasprites[frame], 8, linkSwordTrueX, linkSwordTrueY);

        POP_BANK;

        break;
    }

    return count;
}

uint8_t move_object_with_camera_link_sword(Object *object) NONBANKED
{

    uint8_t frame = linkSword >> 4;

    // The sword slash animations only have 3 frames
    // Limit the frame variable here so it stays in the final frame for a little bit
    if (frame > 2)
        frame = 2;

    uint8_t count = 0;

    switch (object->direction)
    {

    case J_DOWN:

        PUSH_NAMED_BANK(LinkSwordSpritesDown);

        object->currentMetasprite = LinkSwordSpritesDown_metasprites[frame];

        move_object_with_camera(object);

        POP_BANK;

        break;

    case J_LEFT:

        PUSH_NAMED_BANK(LinkSwordSpritesLeft);

        object->currentMetasprite = LinkSwordSpritesLeft_metasprites[frame];

        move_object_with_camera(object);

        POP_BANK;

        break;

    case J_RIGHT:

        PUSH_NAMED_BANK(LinkSwordSpritesRight);

        object->currentMetasprite = LinkSwordSpritesRight_metasprites[frame];

        move_object_with_camera(object);

        POP_BANK;

        break;

    case J_UP:

        PUSH_NAMED_BANK(LinkSwordSpritesUp);

        object->currentMetasprite = LinkSwordSpritesUp_metasprites[frame];

        move_object_with_camera(object);

        POP_BANK;

        break;
    }

    return count;
}

uint8_t move_object_with_camera_link(Object *object, uint8_t moving) NONBANKED
{

    uint8_t count = 0;

    uint8_t frame = moving ? universalBlinkerTrue : 0;


    switch (object->direction)
    {

    default:

        PUSH_NAMED_BANK(LinkSpritesDown);

        object->currentMetasprite = LinkSpritesDown_metasprites[frame];

        move_object_with_camera(object);

        POP_BANK;

        break;

    case J_LEFT:

        PUSH_NAMED_BANK(LinkSpritesLeft);

        object->currentMetasprite = LinkSpritesLeft_metasprites[frame];
        
        move_object_with_camera(object);

        POP_BANK;

        break;

    case J_RIGHT:

        PUSH_NAMED_BANK(LinkSpritesRight);

        object->currentMetasprite = LinkSpritesRight_metasprites[frame];
        
        move_object_with_camera(object);

        POP_BANK;

        break;

    case J_UP:

        PUSH_NAMED_BANK(LinkSpritesUp);

        object->currentMetasprite = LinkSpritesUp_metasprites[frame];
        
        move_object_with_camera(object);

        POP_BANK;

        break;
    }

    return count;
}

void UpdateLink() BANKED
{

    // If we are damaged

    if (currentObjectUpdating->damageX != 0 || currentObjectUpdating->damageY != 0)
    {

        nextX = currentObjectUpdating->x + currentObjectUpdating->damageX;
        nextY = currentObjectUpdating->y + currentObjectUpdating->damageY;

        // Update for damaged
        DamagedNoMove(currentObjectUpdating);

        trueNextX = nextX >> 4;
        trueNextY = nextY >> 4;

        return;
    }

    nextX = currentObjectUpdating->x;
    nextY = currentObjectUpdating->y;

    frame = 0;

    if (cameraScrollDirection != 0)
    {

        currentObjectUpdating->direction = cameraScrollDirection;

        // Move in the scroll direction
        nextX += J_DIRECTIONS[cameraScrollDirection][0] * 4;
        nextY += J_DIRECTIONS[cameraScrollDirection][1] * 4;

        trueNextX = nextX >> 4;
        trueNextY = nextY >> 4;
    }
    else
    {

        uint8_t previousObjectDirection = currentObjectUpdating->direction;

        HandleLinkInput(currentObjectUpdating, &frame);

        if (currentObjectUpdating->direction != previousObjectDirection)
        {

            UpdateLinkDefaultSprites(currentObjectUpdating);
        }
    }
}

void HandleSlashPlants()
{

    uint8_t tile = GetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY);

    // Top Left
    if (tile == HUD_TILE_COUNT + worldNonSolidTileCount)
    {

        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, 0, 0, 0);
        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, 1, 0, 0);
        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, 0, 1, 0);
        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, 1, 1, 0);

        // Bottom Left
    }
    else if (tile == HUD_TILE_COUNT + worldNonSolidTileCount + 2)
    {

        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, 0, 0, 0);
        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, 1, 0, 0);
        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, 0, -1, 0);
        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, 1, -1, 0);
    }
    // Top Right
    else if (tile == HUD_TILE_COUNT + worldNonSolidTileCount + 1)
    {

        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, 0, 0, 0);
        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, -1, 0, 0);
        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, 0, 1, 0);
        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, -1, 1, 0);

        // Bottom Right
    }
    else if (tile == HUD_TILE_COUNT + worldNonSolidTileCount + 3)
    {

        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, 0, 0, 0);
        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, -1, 0, 0);
        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, 0, -1, 0);
        SetBackgroundTileUnderWorldPosition(linkSwordTrueX, linkSwordTrueY, -1, -1, 0);
    }
}

/**
 * @brief Called after all objects have updated
 * This should becalled after all objects have been updated. This method gives objects the ability to stop intersection/overlap with link. Without having to loop
 * through each object unneccesarily.
 * @param sprite The starting sprite to use when drawing metasprites
 * @return uint8_t How many sprites link has used
 */
uint8_t FinishLinkUpdate(uint8_t sprite) BANKED
{

    uint8_t moving = trueNextX != link->trueX || trueNextY != link->trueY;

    MoveToNextPosition(link, nextX, nextY, trueNextX, trueNextY);

    // If the camera is scrolling
    if (cameraScrollDirection != 0)
    {

        // If we are not facing the scrolling direction
        if (link->direction != cameraScrollDirection)
        {

            // Update our direction
            link->direction = cameraScrollDirection;

            // Update links sprites
            UpdateLinkDefaultSprites(link);
        }

        // set our metasprite based on the universal blinker
         move_object_with_camera_link(link, moving);

        return;
    }

    if (linkSword >= 0)
    {

        linkSword += 5;
        frame = linkSword >> 4;

        if (frame < 4)
        {

            // The sword slash animations only have 3 frames
            // Limit the frame variable here so it stays in the final frame for a little bit
            if (frame > 2)
                frame = 2;

            // Draw link's' sword slash animation
            move_object_with_camera_link_sword(link);

            // Position link's sword based on it's frame
            linkSwordX = link->x + (SwordOffsets[link->direction][frame][0] << 4);
            linkSwordY = link->y + (SwordOffsets[link->direction][frame][1] << 4);

            linkSwordTrueX = linkSwordX >> 4;
            linkSwordTrueY = linkSwordY >> 4;

            HandleSlashPlants();

            // Draw links sword
            move_object_with_camera_sword_slash(link);
        }
        else
        {
            linkSword = -1;
            frame = 0;

            UpdateLinkDefaultSprites(link);
        }
    }

    // If link is not using his sword
    if (linkSword == -1)
    {

        // draw his current metasprite
        move_object_with_camera_link(link, moving);
    }
}