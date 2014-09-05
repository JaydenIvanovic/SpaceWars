#include "geometry.h"

#define PROJ_MAX 50
#define CANNON_ROTATE_SPEED 0.25
#define RED_MIN_VEL -7.0
#define RED_MAX_VEL 7.0
#define BLUE_MIN_VEL -7.0
#define BLUE_MAX_VEL 7.0
#define SAIL_COLLISION_DIST 0.26
#define NO_COLLISION -1
#define GRAV_CONST 9.8
#define BASE_X_POS 2.6
#define BASE_Y_POS 0.0
#define BASE_Z_POS 0.0
#define BASE_X_ROT 4.5
#define BASE_Y_ROT 4.5
#define BASE_Z_ROT 0.0

struct projectile{
    float lastDistance;
    float gravconst;
    struct vertex position;
    struct vertex velocity;
    struct vertex gravity;
    char color; /*r = red and b = blue*/
};
 
   
struct{
    struct vertex redPos; /*Red team 'cannon' position.*/
    struct vertex redRot; /*Red team rotation.*/
    struct vertex bluePos; /*Blue team cannon position.*/
    struct vertex blueRot; /*Blue team rotation.*/
    struct vertex sunPos;
    struct projectile projectiles[PROJ_MAX];
    int pIndex;
}globalProjectile;


void initFiringVariables();
void redFiringInit();
void blueFiringInit();
void setProjectileGravConst(struct projectile *, char);
void setDefaultProjectileState(struct projectile *, char);
void createProjectile(char);
void rotateRedLeft();
void rotateRedRight();
void rotateBlueLeft();
void rotateBlueRight();
void updateProjectileGravity(struct projectile *);
void updateProjectilePos(float);
int isCollision(struct vertex, struct vertex, float, float);
int isBaseCollision(struct projectile, struct vertex, char);
int isSailCollision(struct vertex, struct vertex);
int projectileCollided(struct vertex, int);
void handleCollision(int);
void handleBaseCollision(int, char);
void checkForCollisions();
void drawProjectiles();
