typedef struct Enemy{
    uint16_t x, y;
    uint8_t active;
    uint8_t startSprite;
    uint8_t direction;
    uint8_t index;
    int8_t hitX,hitY;
    
    uint8_t health;
    uint8_t frameCounter;
    uint8_t movementType;
    uint8_t enemyType;
    uint8_t set;

} Enemy;


typedef struct EnemyLocation{
    uint8_t startX, startY;
} EnemyLocation;


void ActivateEnemy(Enemy *enemy, int health,uint16_t x,uint16_t y);
void UpdateEnemy(Enemy *enemy);
void ReOrderEnemies();
void UpdateAllEnemies();

#define MAX_ENEMIES_ONSCREEN 5

    