#ifndef LINK_HEADER
#define LINK_HEADER set

void SetupLink();
void UpdateLink();
void PostUpdateLink();

#define NO_WEAPON 2
#define SHIELD 4
#define SWORD 8

#define PUSH_COUNTER_MAX 4

typedef struct Link{

    uint16_t x,y;
    int16_t hitX,hitY;
    uint8_t weapon,moving,health,hearts,pushCounter;
    uint8_t frameCounter,attacking,shielding;
    uint8_t dir,frame,frameOffset,lastFrame,flip,swordFrame;

} Link;



#endif