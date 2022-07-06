#ifndef WORLD_HEADER
#define WORLD_HEADER set



void init_world(uint8_t bank, uint8_t* tilemap, uint8_t* tilemapAttributes,uint8_t width, uint8_t tilesetCount, uint8_t* tilesetTiles);
void set_world_bkg_submap(uint8_t x, uint8_t y, uint8_t width, uint8_t height);

#endif